#include "import.h"

#include <chrono>

#include "storage/index/hash/strings_hash/strings_hash_bulk_import.h"

using namespace ImportRdf;

char* Import::ExternalString::strings = nullptr;

SerdStatus on_base(void* handle,
                   const SerdNode* uri) {
    OnDiskImport* importer = static_cast<OnDiskImport*>(handle);

    serd_env_set_base_uri(importer->env, uri);

    return SERD_SUCCESS;
}

SerdStatus on_prefix(void*           handle,
                     const SerdNode* name,
                     const SerdNode* uri) {
    OnDiskImport* importer = static_cast<OnDiskImport*>(handle);

    serd_env_set_prefix(importer->env, name, uri);

    return SERD_SUCCESS;
}

SerdStatus on_statement(void*              handle,
                        SerdStatementFlags /*flags*/,
                        const SerdNode*    /*graph*/,
                        const SerdNode*    subject,
                        const SerdNode*    predicate,
                        const SerdNode*    object,
                        const SerdNode*    object_datatype,
                        const SerdNode*    object_lang) {
    OnDiskImport* importer = static_cast<OnDiskImport*>(handle);

    // Handle objects first, as they could be a literal with a xsd datatype (e.g. xsd:dateTime)
    // that are not handled by the serd parser
    importer->handle_object(object, object_datatype, object_lang);
    if (importer->object_has_errors) {
        return SERD_FAILURE;
    } else {
        importer->handle_subject(subject);
        importer->handle_predicate(predicate);
        importer->save_triple();
        return SERD_SUCCESS;
    }
}

void OnDiskImport::start_import(const std::string& input_filename, const std::string& prefixes_filename) {
    auto start = std::chrono::system_clock::now();

    size_t external_strings_initial_size = (1024ULL * 1024ULL * 1024ULL * buffer_size_in_GB) / 2;
    external_strings                     = new char[external_strings_initial_size];
    Import::ExternalString::strings      = external_strings;
    external_strings_capacity            = external_strings_initial_size;
    external_strings_end                 = StringManager::METADATA_SIZE;

    if (prefixes_filename.empty()) {
        prefixes = { "" };
    } else {
        // Open file
        FILE* prefixes_file = fopen(prefixes_filename.c_str(), "r");
        // Read line by line and store in prefixes vector
        char* line = NULL;
        size_t len = 0;
        ssize_t nread;
        while ((nread = getline(&line, &len, prefixes_file)) != -1) {
            if (line[nread - 1] == '\n') {
                line[nread - 1] = '\0';
            }
            prefixes.push_back(line);
        }
        // Cleanup
        free(line);
        fclose(prefixes_file);
        // ALWAYS set last prefix as empty string
        prefixes.push_back("");
    }

    // Open file
    FILE* input_file = fopen(input_filename.c_str(), "r");
    // Initialize reader
    // It receives a pointer to this class for accessing its members in the callback function on_statement
    reader = serd_reader_new(SERD_TURTLE, this, NULL, on_base, on_prefix, on_statement, NULL);
    // Start reading file
    serd_reader_start_stream(reader, input_file, NULL, true);
    // Read until EOF
    while (serd_reader_read_chunk(reader) != SERD_FAILURE) {
        continue;
    }

    auto end_reader = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> reader_duration = end_reader - start;
    std::cout << "Reader duration: " << reader_duration.count() << " ms\n";

    // Finish reading
    serd_reader_end_stream(reader);
    // Cleanup
    serd_reader_free(reader);
    serd_env_free(env);
    fclose(input_file);
    // Materialize data
    triples.finish_appends();
    equal_spo.finish_appends();
    equal_sp.finish_appends();
    equal_so.finish_appends();
    equal_po.finish_appends();

    { // Destroy external_ids_map replacing it with an empty map
        robin_hood::unordered_set<Import::ExternalString> tmp;
        external_strings_set.swap(tmp);
    }

    { // Destroy blank_ids_map replacing it with an empty map
        robin_hood::unordered_map<std::string, uint64_t> tmp;
        blank_ids_map.swap(tmp);
    }

    {   // Write Strings
        // write end
        *reinterpret_cast<uint64_t*>(external_strings) = external_strings_end;

        std::fstream strings_file;
        strings_file.open(db_folder + "/strings.dat", std::ios::out|std::ios::binary);
        strings_file.write(external_strings, external_strings_end);

        // round up to PAGE_SIZE multiple
        auto remaining = StringManager::STRING_BLOCK_SIZE
                         - (external_strings_end % StringManager::STRING_BLOCK_SIZE);
        strings_file.write(external_strings, remaining); // copies anything, content doesn't matter
    }

    {   // Write StringsHash
        StringsHashBulkImport strings_hash(db_folder + "/str_hash");

        size_t current_offset = StringManager::METADATA_SIZE;
        while (current_offset < external_strings_end) {
            auto current_str = external_strings + current_offset;

            size_t bytes_for_len;
            size_t len = StringManager::get_string_len(current_str, &bytes_for_len);
            current_str += bytes_for_len;

            strings_hash.create_id(current_str, current_offset, len);

            current_offset += bytes_for_len + len;

            // skip alignment bytes
            size_t remaining_in_page = StringManager::STRING_BLOCK_SIZE
                                       - (current_offset % StringManager::STRING_BLOCK_SIZE);
            if (remaining_in_page < Import::ExternalString::MIN_PAGE_REMAINING_BYTES) {
                current_offset += remaining_in_page;
            }
        }

        delete[] external_strings;
    }

    auto end_obj_file = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> obj_duration = end_obj_file - end_reader;
    std::cout << "Write obj file and obj file hash duration: " << obj_duration.count() << " ms\n";

    size_t buffer_size = 1024ULL * 1024ULL * 1024ULL * buffer_size_in_GB;
    char*  buffer      = reinterpret_cast<char*>(std::aligned_alloc(Page::MDB_PAGE_SIZE, buffer_size));

    { // Triple B+Trees
        size_t COL_SUBJ = 0, COL_PRED = 1, COL_OBJ = 2;
        std::array<size_t, 3> original_permutation = { COL_SUBJ, COL_PRED, COL_OBJ };

        triples.start_indexing(buffer, buffer_size, original_permutation);

        Import::DistinctStat<3> subject_stat;
        Import::PredicateStat   predicate_stat;
        Import::DistinctStat<3> object_stat;
        Import::NoStat<3>       no_stat;

        catalog.triples_count = triples.total_tuples;
        triples.create_bpt(db_folder + "/spo", { COL_SUBJ, COL_PRED, COL_OBJ }, subject_stat);
        catalog.distinct_subjects = subject_stat.distinct;

        triples.create_bpt(db_folder + "/pos", { COL_PRED, COL_OBJ, COL_SUBJ }, predicate_stat);
        predicate_stat.end();
        catalog.distinct_predicates = predicate_stat.distinct_values;
        catalog.predicate2total_count = move(predicate_stat.map_predicate_count);

        triples.create_bpt(db_folder + "/osp", { COL_OBJ, COL_SUBJ, COL_PRED }, object_stat);
        catalog.distinct_objects = object_stat.distinct;

        triples.create_bpt(db_folder + "/pso", { COL_PRED, COL_SUBJ, COL_OBJ }, no_stat);

        triples.finish_indexing();
    }

    auto end_triple_index = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> triple_index_duration = end_triple_index - end_obj_file;
    std::cout << "Write triple index: " << triple_index_duration.count() << " ms\n";

    { // SUBJECT=PREDICATE=OBJECT
        size_t COL_SUBJ_PRED_OBJ = 0;
        std::array<size_t, 1> original_permutation = { COL_SUBJ_PRED_OBJ };
        equal_spo.start_indexing(buffer, buffer_size, original_permutation);

        Import::NoStat<1> no_stat;
        catalog.equal_spo_count = equal_spo.total_tuples;
        equal_spo.create_bpt(db_folder + "/equal_spo",
                             { COL_SUBJ_PRED_OBJ },
                             no_stat);

        equal_spo.finish_indexing();
    }

    { // SUBJECT=PREDICATE OBJECT
        size_t COL_SUBJ_PRED = 0, COL_OBJ = 1;
        std::array<size_t, 2> original_permutation = { COL_SUBJ_PRED, COL_OBJ };
        equal_sp.start_indexing(buffer, buffer_size, original_permutation);

        Import::NoStat<2> no_stat;
        catalog.equal_sp_count = equal_sp.total_tuples;
        equal_sp.create_bpt(db_folder + "/equal_sp",
                            { COL_SUBJ_PRED, COL_OBJ },
                            no_stat);

        Import::NoStat<2> no_stat_inverted;
        equal_sp.create_bpt(db_folder + "/equal_sp_inverted",
                            { COL_OBJ, COL_SUBJ_PRED },
                            no_stat_inverted);

        equal_sp.finish_indexing();
    }

    { // SUBJECT=OBJECT PREDICATE
        size_t COL_SUBJ_OBJ = 0, COL_PRED = 1;
        std::array<size_t, 2> original_permutation = { COL_SUBJ_OBJ, COL_PRED };
        equal_so.start_indexing(buffer, buffer_size, original_permutation);

        Import::NoStat<2> no_stat;
        catalog.equal_so_count = equal_so.total_tuples;
        equal_so.create_bpt(db_folder + "/equal_so",
                            { COL_SUBJ_OBJ, COL_PRED },
                            no_stat);
        Import::NoStat<2> no_stat_inverted;
        equal_so.create_bpt(db_folder + "/equal_so_inverted",
                            { COL_PRED, COL_SUBJ_OBJ },
                            no_stat_inverted);

        equal_so.finish_indexing();
    }

    { // PREDICATE=OBJECT SUBJECT
        size_t COL_PRED_OBJ = 0, COL_SUBJ = 1;
        std::array<size_t, 2> original_permutation = { COL_PRED_OBJ, COL_SUBJ };
        equal_po.start_indexing(buffer, buffer_size, original_permutation);

        Import::NoStat<2> no_stat;
        catalog.equal_po_count = equal_po.total_tuples;
        equal_po.create_bpt(db_folder + "/equal_po",
                            { COL_PRED_OBJ, COL_SUBJ },
                            no_stat);

        Import::NoStat<2> no_stat_inverted;
        equal_po.create_bpt(db_folder + "/equal_po_inverted",
                            { COL_SUBJ, COL_PRED_OBJ },
                            no_stat_inverted);

        equal_po.finish_indexing();
    }
    free(buffer);

    auto end_special_index = std::chrono::system_clock::now();
    std::chrono::duration<float, std::milli> special_index_duration = end_special_index - end_triple_index;
    std::cout << "Write special cases index: " << special_index_duration.count() << " ms\n";

    std::chrono::duration<float, std::milli> total_duration = end_triple_index - start;
    std::cout << "Total duration: " << total_duration.count() << " ms\n";

    // Store IRI prefixes and literal datatypes/languages into catalog
    catalog.prefixes = std::move(prefixes);
    catalog.datatypes.resize(datatype_ids_map.size());
    for (auto&& [datatype, id] : datatype_ids_map) {
        catalog.datatypes[id] = datatype;
    }
    catalog.languages.resize(language_ids_map.size());
    for (auto&& [language, id] : language_ids_map) {
        catalog.languages[id] = language;
    }

    catalog.print();
    catalog.save_changes();
}
