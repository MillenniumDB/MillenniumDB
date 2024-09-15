#include "import.h"

#include <chrono>
#include <cstdio>
#include <ctime>
#include <locale>
#include <map>
#include <regex>

#include "graph_models/rdf_model/rdf_model.h"
#include "import/disk_vector.h"
#include "import/import_helper.h"
#include "macros/aligned_alloc.h"
#include "misc/fatal_error.h"
#include "storage/index/hash/strings_hash/strings_hash_bulk_ondisk_import.h"


namespace Import { namespace Rdf {

SerdStatus on_base(void* handle, const SerdNode* uri) {
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

    importer->triple_has_errors = false;
    importer->handle_subject(subject);
    importer->handle_predicate(predicate);
    importer->handle_object(object, object_datatype, object_lang);
    if (importer->triple_has_errors) {
        return SERD_FAILURE;
    } else {
        importer->save_triple();
        return SERD_SUCCESS;
    }
}


void OnDiskImport::start_import(const std::string& input_filename, const std::string& prefixes_filename) {
    auto start = std::chrono::system_clock::now();
    auto import_start = start;

    pending_triples = std::make_unique<DiskVector<3>>(db_folder + "/tmp_pending_triples0");
    std::string pending_strings_filename = db_folder + "/tmp_pending_strings0";
    pending_strings = get_fstream(pending_strings_filename);

    {   // Initial memory allocation
        external_strings = reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(VPage::SIZE, buffer_size));

        if (external_strings == nullptr) {
            FATAL_ERROR("Could not allocate buffer, try using a smaller buffer size");
        }

        Import::ExternalString::strings      = external_strings;
        external_strings_capacity            = buffer_size;
        external_strings_end                 = StringManager::METADATA_SIZE;
        assert(external_strings_capacity > StringManager::STRING_BLOCK_SIZE);
    }

    {   // Process IRI prefixes

        // We look for prefixes in the following order:
        //   1. prefix file
        //   2. default prefixes
        //   3. base and prefix definitions in first MAX_LINES_TLL_PREFIX of the .ttl file
        // Until exhausting or reaching 256 prefixes.

        std::set<std::string> prefix_set = {""}; // empty prefix

        if (!prefixes_filename.empty()) {
            std::ifstream prefixes_file(prefixes_filename);
            if (prefixes_file.fail()) {
                throw std::runtime_error("Could not open file " + prefixes_filename);
            }

            std::string line;

            while (prefix_set.size() < 256 && std::getline(prefixes_file, line)) {

                auto start_prefix = line.find_first_not_of(" \t");
                if (start_prefix == std::string::npos) {
                    // Whitespace only
                    continue;
                }

                auto end_prefix = line.find_last_not_of(" \t") + 1;

                auto invalid_prefix = false;
                for (size_t i = start_prefix; i < end_prefix; i++) {
                    unsigned char c = line[i];
                    if (c < IRI_PREFIX_CHAR_MIN || c > IRI_PREFIX_CHAR_MAX) {
                        invalid_prefix = true;
                        break;
                    }
                }

                if (invalid_prefix) {
                    std::cout << "Invalid prefix: " << line << "\n";
                    std::cout << "Only chars in range [" << IRI_PREFIX_CHAR_MIN << ", " << IRI_PREFIX_CHAR_MAX << "] are allowed\n";
                    continue;
                }

                auto prefix = line.substr(start_prefix, end_prefix - start_prefix);

                if (!prefix_set.insert(prefix).second) {
                    std::cout << "Duplicated prefix: " << prefix << "\n";
                }
            }
            prefixes_file.close();
        }

        for (auto& prefix : RdfModel::default_compression_prefixes) {
            if (prefix_set.size() < 256) {
                prefix_set.insert(prefix);
            } else {
                break;
            }
        }

        std::string line;
        int lines_checked = 0;
        const std::regex prefix_regex("^\\s*@?(prefix|base)", std::regex::icase);
        std::fstream ttl_stream(input_filename);

        while (prefix_set.size() < 256
            && lines_checked < MAX_LINES_TLL_PREFIX
            && std::getline(ttl_stream, line))
        {
            lines_checked++;

            if (std::regex_search(line, prefix_regex)) {
                auto start = line.find_first_of('<');
                if (start == std::string::npos) {
                    continue;
                }

                auto end = line.find_last_of('>');
                if (end == std::string::npos || start >= end) {
                    continue;
                }

                auto iri = line.substr(start + 1, end - start - 1);
                prefix_set.insert(iri);
            }
        }

        prefixes.init(std::move(prefix_set));
    }

    {   // TTL parsing
        FILE* input_file = fopen(input_filename.c_str(), "r");
        // It receives a pointer to this class for accessing its members in the callback function on_statement
        reader = serd_reader_new(SERD_TURTLE, this, NULL, on_base, on_prefix, on_statement, NULL);
        serd_reader_start_stream(reader, input_file, reinterpret_cast<const uint8_t*>(input_filename.c_str()), true);

        while (true) {
            auto status = serd_reader_read_chunk(reader);
            switch (status) {
            case SERD_SUCCESS:
                continue;
            case SERD_FAILURE:
                goto exit_while;
            case SERD_ERR_UNKNOWN:
            case SERD_ERR_BAD_SYNTAX:
            case SERD_ERR_BAD_ARG:
            case SERD_ERR_NOT_FOUND:
            case SERD_ERR_ID_CLASH:
            case SERD_ERR_BAD_CURIE:
            case SERD_ERR_INTERNAL: {
                auto& cursor = reader->source.cur;
                std::string error = "INTERNAL ERROR on line " + std::to_string(cursor.line) + ":"
                    + std::to_string(cursor.col) + ". " + std::string((char*) serd_strerror(status));
                NON_FATAL_ERROR(error);
            }
            }
        }
        exit_while:

        // Finish reading and Cleanup
        serd_reader_end_stream(reader);
        serd_reader_free(reader);
        serd_env_free(env);
        fclose(input_file);
    }

    print_duration("Parsing", start);

    std::fstream strings_file;
    strings_file.open(db_folder + "/strings.dat", std::ios::out|std::ios::binary);
    strings_file.write(external_strings, external_strings_end);
    // metadata of strings_file (at pos 0) will be updated later
    previous_external_strings_offset += external_strings_end;

    // Big enough buffer to store any string
    char* pending_string_buffer = reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(
        VPage::SIZE,
        StringManager::STRING_BLOCK_SIZE));
    int i = 0; // used for tmp filenames
    pending_triples->finish_appends();

    while (pending_triples->get_total_tuples() > 0) {
        std::cout << "pending triples: " << pending_triples->get_total_tuples() << std::endl;
        auto old_pending_triples = std::move(pending_triples);
        auto old_pending_strings = std::move(pending_strings);

        auto old_pending_strings_filename = db_folder + "/tmp_pending_strings" + std::to_string(i);
        i++;
        pending_strings_filename = db_folder + "/tmp_pending_strings" + std::to_string(i);
        pending_triples = std::make_unique<DiskVector<3>>(db_folder + "/tmp_pending_triples" + std::to_string(i));
        pending_strings = get_fstream(pending_strings_filename);
        external_strings_set.clear();

        // set to align properly the old block of strings with the new block that will be created
        external_strings_align_offset = external_strings_end % StringManager::STRING_BLOCK_SIZE;
        external_strings_end = external_strings_align_offset;

        old_pending_triples->begin_tuple_iter();
        while (old_pending_triples->has_next_tuple()) {
            auto& pending_triple = old_pending_triples->next_tuple();
            const auto original_data_mask = ObjectId::SUB_TYPE_MASK | ObjectId::MASK_LITERAL_TAG;
            if ((pending_triple[0] & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
                uint64_t mask = (pending_triple[0] & original_data_mask);
                old_pending_strings->seekg(pending_triple[0] & ObjectId::MASK_EXTERNAL_ID);
                auto str_len = read_str(*old_pending_strings, pending_string_buffer);
                subject_id.id = get_or_create_external_id(pending_string_buffer, str_len) | mask;
            } else {
                subject_id.id = pending_triple[0];
            }
            if ((pending_triple[1] & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
                uint64_t mask = (pending_triple[1] & original_data_mask);
                old_pending_strings->seekg(pending_triple[1] & ObjectId::MASK_EXTERNAL_ID);
                auto str_len = read_str(*old_pending_strings, pending_string_buffer);
                predicate_id.id = get_or_create_external_id(pending_string_buffer, str_len) | mask;
            } else {
                predicate_id.id = pending_triple[1];
            }
            if ((pending_triple[2] & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
                uint64_t mask = (pending_triple[2] & original_data_mask);
                old_pending_strings->seekg(pending_triple[2] & ObjectId::MASK_EXTERNAL_ID);
                auto str_len = read_str(*old_pending_strings, pending_string_buffer);
                object_id.id = get_or_create_external_id(pending_string_buffer, str_len) | mask;
            } else {
                object_id.id = pending_triple[2];
            }
            save_triple();
        }

        // write new strings, skipping the first `external_strings_align_offset` bytes
        strings_file.write(external_strings + external_strings_align_offset, external_strings_end - external_strings_align_offset);
        previous_external_strings_offset += external_strings_end - external_strings_align_offset;

        // close and delete old_pending_triples file
        pending_triples->finish_appends();
        old_pending_triples->skip_indexing(); // will close and remove file

        // close and delete old_pending_strings
        old_pending_strings->close();
        std::remove(old_pending_strings_filename.c_str());
    }

    // delete pending triples and pending strings
    pending_triples->skip_indexing(); // will close and remove file
    pending_strings->close();
    std::remove(pending_strings_filename.c_str());

    // Finish Strings File
    // Round up to strings file to be a multiple of StringManager::STRING_BLOCK_SIZE
    uint64_t last_str_pos = strings_file.tellp();
    uint64_t last_block_offset = last_str_pos % StringManager::STRING_BLOCK_SIZE;
    uint64_t remaining = StringManager::STRING_BLOCK_SIZE - last_block_offset;

    // can copy anything, content doesn't matter, but setting zeros is better for debug
    memset(external_strings, '\0', remaining);
    strings_file.write(external_strings, remaining);

    // Write strings_file metadata
    strings_file.seekp(0, strings_file.beg);
    strings_file.write(reinterpret_cast<char*>(&last_block_offset), sizeof(uint64_t));

    print_duration("Processing strings", start);

    { // Destroy external_ids_map replacing it with an empty map
        robin_hood::unordered_set<Import::ExternalString> tmp;
        external_strings_set.swap(tmp);
    }

    { // Destroy blank_ids_map replacing it with an empty map
        robin_hood::unordered_map<std::string, uint64_t> tmp;
        blank_ids_map.swap(tmp);
    }

    {   // Create StringsHash
        // we reuse the allocated memory for external strings as a buffer
        StringsHashBulkOnDiskImport strings_hash(db_folder + "/str_hash",
                                                 external_strings,
                                                 external_strings_capacity);
        strings_file.close();
        strings_file.open(db_folder + "/strings.dat", std::ios::in|std::ios::binary);
        strings_file.seekg(StringManager::METADATA_SIZE, strings_file.beg);
        uint64_t current_pos = StringManager::METADATA_SIZE;

        // read all strings one by one and add them to the StringsHash
        while (current_pos < last_str_pos) {
            size_t remaining_in_block = StringManager::STRING_BLOCK_SIZE
                                            - (current_pos % StringManager::STRING_BLOCK_SIZE);

            if (remaining_in_block < StringManager::MIN_PAGE_REMAINING_BYTES) {
                current_pos += remaining_in_block;
                strings_file.read(pending_string_buffer, remaining_in_block);
            }

            auto str_len = read_str(strings_file, pending_string_buffer);
            strings_hash.create_id(pending_string_buffer, current_pos, str_len);
            current_pos = strings_file.tellg();
        }
        strings_file.close();
        MDB_ALIGNED_FREE(pending_string_buffer);
    }

    print_duration("Write strings and strings hashes", start);

    // we reuse the buffer for external strings in the B+trees creation
    char* const buffer = external_strings;
    buffer_size = external_strings_capacity;

    // Save lasts blocks to disk
    triples.finish_appends();
    equal_sp.finish_appends();
    equal_so.finish_appends();
    equal_po.finish_appends();
    equal_spo.finish_appends();

    triples.start_indexing  (buffer, buffer_size, {0,1,2});
    equal_sp.start_indexing (buffer, buffer_size, {0,1});
    equal_so.start_indexing (buffer, buffer_size, {0,1});
    equal_po.start_indexing (buffer, buffer_size, {0,1});
    equal_spo.start_indexing(buffer, buffer_size, {0});


    {   // B+tree creation for triple
        size_t COL_SUBJ = 0, COL_PRED = 1, COL_OBJ = 2;

        Import::PredicateStat pred_stat;
        Import::NoStat<3>     no_stat;

        triples.create_bpt(db_folder + "/spo", { COL_SUBJ, COL_PRED, COL_OBJ }, no_stat);

        triples.create_bpt(db_folder + "/pos", { COL_PRED, COL_OBJ, COL_SUBJ }, pred_stat);
        pred_stat.end();
        catalog.set_triples_count(pred_stat.all_count);
        catalog.set_predicate_stats(std::move(pred_stat.map_predicate_count));

        triples.create_bpt(db_folder + "/osp", { COL_OBJ, COL_SUBJ, COL_PRED }, no_stat);

        if (index_permutations >= 4) {
            triples.create_bpt(db_folder + "/pso", { COL_PRED, COL_SUBJ, COL_OBJ }, no_stat);
        }

        if (index_permutations == 6) {
            triples.create_bpt(db_folder + "/sop", { COL_SUBJ, COL_OBJ, COL_PRED }, no_stat);
            triples.create_bpt(db_folder + "/ops", { COL_OBJ, COL_PRED, COL_SUBJ }, no_stat);
        }
    }

    print_duration("Write triples indexes", start);

    {   // B+tree creation SUBJECT=PREDICATE OBJECT
        size_t COL_SUBJ_PRED = 0, COL_OBJ = 1;

        Import::AllStat<2> all_stat;
        equal_sp.create_bpt(db_folder + "/equal_sp",
                            { COL_SUBJ_PRED, COL_OBJ },
                            all_stat);
        catalog.set_equal_sp_count(all_stat.all);

        Import::NoStat<2> no_stat_inverted;
        equal_sp.create_bpt(db_folder + "/equal_sp_inverted",
                            { COL_OBJ, COL_SUBJ_PRED },
                            no_stat_inverted);
    }

    {   // B+tree creation SUBJECT=OBJECT PREDICATE
        size_t COL_SUBJ_OBJ = 0, COL_PRED = 1;

        Import::AllStat<2> all_stat;
        equal_so.create_bpt(db_folder + "/equal_so",
                            { COL_SUBJ_OBJ, COL_PRED },
                            all_stat);
        catalog.set_equal_so_count(all_stat.all);
        Import::NoStat<2> no_stat_inverted;
        equal_so.create_bpt(db_folder + "/equal_so_inverted",
                            { COL_PRED, COL_SUBJ_OBJ },
                            no_stat_inverted);
    }

    {   // B+tree creation PREDICATE=OBJECT SUBJECT
        size_t COL_PRED_OBJ = 0, COL_SUBJ = 1;

        Import::AllStat<2> all_stat;
        equal_po.create_bpt(db_folder + "/equal_po",
                            { COL_PRED_OBJ, COL_SUBJ },
                            all_stat);
        catalog.set_equal_po_count(all_stat.all);

        Import::NoStat<2> no_stat_inverted;
        equal_po.create_bpt(db_folder + "/equal_po_inverted",
                            { COL_SUBJ, COL_PRED_OBJ },
                            no_stat_inverted);
    }

    {   // B+tree creation SUBJECT=PREDICATE=OBJECT
        size_t COL_SUBJ_PRED_OBJ = 0;

        Import::AllStat<1> all_stat;
        equal_spo.create_bpt(db_folder + "/equal_spo",
                             { COL_SUBJ_PRED_OBJ },
                             all_stat);
        catalog.set_equal_spo_count(all_stat.all);
    }

    // calling finish_indexing() closes and removes the file.
    triples.finish_indexing();
    equal_sp.finish_indexing();
    equal_so.finish_indexing();
    equal_po.finish_indexing();
    equal_spo.finish_indexing();
    MDB_ALIGNED_FREE(external_strings);

    print_duration("Write special cases B+tree index", start);

    // Store IRI aliases, prefixes and literal datatypes/languages into catalog
    catalog.prefixes = std::move(prefixes);
    catalog.datatypes.resize(datatype_ids_map.size());
    for (auto&& [datatype, id] : datatype_ids_map) {
        catalog.datatypes[id] = datatype;
    }
    catalog.languages.resize(language_ids_map.size());
    for (auto&& [language, id] : language_ids_map) {
        catalog.languages[id] = language;
    }

    catalog.print(std::cout);

    print_duration("Write catalog", start);
    print_duration("Total Import", import_start);
}
}} // Namespace Import::Rdf
