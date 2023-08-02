#include "import.h"

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <locale>
#include <map>

#include "import/disk_vector.h"
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


// returns the encoded string length, and writes the string in the buffer
// assumes the string fits in the buffer and file read position is correct
size_t read_pending_str(std::fstream& file, char* buffer) {
    size_t decoded_len = 0;
    // size_t bytes_for_len = 0;
    size_t shift_size = 0;

    while (true) {
        char c;
        file.read(&c, 1);
        auto decode_ptr = reinterpret_cast<unsigned char*>(&c);
        uint64_t b = *decode_ptr;
        // bytes_for_len++;

        if (b <= 127) {
            decoded_len |= b << shift_size;
            break;
        } else {
            decoded_len |= (b & 0x7FUL) << shift_size;
        }
        shift_size += 7;
    }
    file.read(buffer, decoded_len);
    return decoded_len;
}


inline std::unique_ptr<std::fstream> get_fstream(const std::string& filename) {
    auto res = std::make_unique<std::fstream>();
    res->open(filename, std::ios::out|std::ios::app); // only to create new file
    res->close();
    res->open(filename, std::ios::in|std::ios::out|std::ios::binary);
    return res;
}


// prints the duration and updates the start time point
 void print_duration(const std::string& msg,
                     std::chrono::system_clock::time_point& start)
{
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<float> duration = end - start;
    auto seconds_duration = duration.count();
    if (seconds_duration < 1) {
         std::cout << msg << " duration: " << seconds_duration*1000 << " milliseconds" << std::endl;
    } else if (seconds_duration <= 60) {
        std::cout << msg << " duration: " << seconds_duration << " seconds" << std::endl;
    } else if (seconds_duration <= 60*60) {
        std::cout << msg << " duration: " << (seconds_duration / 60) << " minutes" << std::endl;
    } else {
        std::cout << msg << " duration: " << (seconds_duration / (60*60)) << " hours" << std::endl;
    }
    start = end;
}


void OnDiskImport::start_import(const std::string& input_filename, const std::string& prefixes_filename) {
    auto start = std::chrono::system_clock::now();
    auto import_start = start;

    pending_triples = std::make_unique<DiskVector<3>>(db_folder + "/tmp_pending_triples0");
    std::string pending_strings_filename = db_folder + "/tmp_pending_strings0";
    pending_strings = get_fstream(pending_strings_filename);

    {   // Initial memory allocation
        size_t external_strings_initial_size = 1024ULL * 1024ULL * 1024ULL * buffer_size_in_GB;
        external_strings                     = reinterpret_cast<char*>(std::aligned_alloc(Page::MDB_PAGE_SIZE, external_strings_initial_size));
        Import::ExternalString::strings      = external_strings;
        external_strings_capacity            = external_strings_initial_size;
        external_strings_end                 = StringManager::METADATA_SIZE;
        assert(external_strings_capacity > StringManager::STRING_BLOCK_SIZE);
    }

    {   // Process IRI prefixes
        // The first prefix/alias needs to be the empty string
        prefixes = { "" };
        aliases  = { "" };
        if (!prefixes_filename.empty()) {
            std::ifstream prefixes_file(prefixes_filename);
            if (prefixes_file.fail()) {
                throw std::runtime_error("Could not open file " + prefixes_filename);
            }
            std::map<std::string, std::string> prefix_map;
            std::string line;
            while (std::getline(prefixes_file, line)) {
                const char* whitespaces = " \t\f\v\n\r";
                auto start_alias = line.find_first_not_of(whitespaces);
                if (start_alias == std::string::npos) {
                    continue;
                }
                auto colon_pos = line.find_first_of(':');
                if (colon_pos == std::string::npos) {
                    std::cout << "Bad prefix line: " << line << "\n";
                    continue;
                }
                auto end_alias = line.find_first_of(whitespaces, start_alias);
                if (colon_pos < end_alias) {
                    end_alias = colon_pos;
                }

                auto start_prefix = line.find_first_not_of(whitespaces, colon_pos+1);
                auto end_prefix1 = line.find_first_of(whitespaces, start_prefix+1);
                if (end_prefix1 == std::string::npos) {
                    end_prefix1 = line.size();
                }
                auto end_prefix2 = line.find_last_not_of(whitespaces) + 1;

                // end_prefix1 != end_prefix2 means there is a whitespace inside the prefix
                if (end_prefix1 != end_prefix2) {
                    std::cout << "Bad prefix line: " << line << "\n";
                    continue;
                }
                auto alias = line.substr(start_alias, end_alias-start_alias);
                auto prefix = line.substr(start_prefix, end_prefix1-start_prefix);

                auto insertion = prefix_map.insert({prefix, alias});
                if (!insertion.second) {
                    std::cout << "duplicated prefix: " << prefix << "\n";
                }
            }
            // insert in inverse order so a longer prefix comes first if there is a substring
            // e.g. "http://www." should be considered before "http://"
            for (auto iter = prefix_map.crbegin(); iter != prefix_map.crend(); ++iter) {
                prefixes.push_back(iter->first);
                aliases.push_back(iter->second);
            }
            prefixes_file.close();
        }
    }

    {   // TTL parsing
        FILE* input_file = fopen(input_filename.c_str(), "r");
        // It receives a pointer to this class for accessing its members in the callback function on_statement
        reader = serd_reader_new(SERD_TURTLE, this, NULL, on_base, on_prefix, on_statement, NULL);
        serd_reader_start_stream(reader, input_file, NULL, true);

        while (serd_reader_read_chunk(reader) != SERD_FAILURE) { // Read until EOF
            continue;
        }

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
    char* pending_string_buffer = reinterpret_cast<char*>(std::aligned_alloc(
                                                            Page::MDB_PAGE_SIZE,
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
                old_pending_strings->seekg(pending_triple[0] & ObjectId::MASK_LITERAL);
                auto str_len = read_pending_str(*old_pending_strings, pending_string_buffer);
                subject_id = get_or_create_external_id(pending_string_buffer, str_len) | mask;
            } else {
                subject_id = pending_triple[0];
            }
            if ((pending_triple[1] & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
                uint64_t mask = (pending_triple[1] & original_data_mask);
                old_pending_strings->seekg(pending_triple[1] & ObjectId::MASK_LITERAL);
                auto str_len = read_pending_str(*old_pending_strings, pending_string_buffer);
                predicate_id = get_or_create_external_id(pending_string_buffer, str_len) | mask;
            } else {
                predicate_id = pending_triple[1];
            }
            if ((pending_triple[2] & ObjectId::MOD_MASK) == ObjectId::MOD_TMP) {
                uint64_t mask = (pending_triple[2] & original_data_mask);
                old_pending_strings->seekg(pending_triple[2] & ObjectId::MASK_LITERAL);
                auto str_len = read_pending_str(*old_pending_strings, pending_string_buffer);
                object_id = get_or_create_external_id(pending_string_buffer, str_len) | mask;
            } else {
                object_id = pending_triple[2];
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
    strings_file.write(external_strings, remaining); // copies anything, content doesn't matter

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

            auto str_len = read_pending_str(strings_file, pending_string_buffer);
            strings_hash.create_id(pending_string_buffer, current_pos, str_len);
            current_pos = strings_file.tellg();
        }
        strings_file.close();
        free(pending_string_buffer);
    }

    print_duration("Write strings hash strings", start);

    // we reuse the buffer for external strings in the B+trees creation
    char* const buffer = external_strings;
    const auto buffer_size = external_strings_capacity;

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

        Import::DistinctStat<3> subject_stat;
        Import::PredicateStat   predicate_stat;
        Import::DistinctStat<3> object_stat;
        Import::NoStat<3>       no_stat;

        triples.create_bpt(db_folder + "/spo", { COL_SUBJ, COL_PRED, COL_OBJ }, subject_stat);
        catalog.triples_count = subject_stat.all;
        catalog.distinct_subjects = subject_stat.distinct;

        triples.create_bpt(db_folder + "/pos", { COL_PRED, COL_OBJ, COL_SUBJ }, predicate_stat);
        predicate_stat.end();
        catalog.distinct_predicates = predicate_stat.distinct_values;
        catalog.predicate2total_count = std::move(predicate_stat.map_predicate_count);

        triples.create_bpt(db_folder + "/osp", { COL_OBJ, COL_SUBJ, COL_PRED }, object_stat);
        catalog.distinct_objects = object_stat.distinct;

        triples.create_bpt(db_folder + "/pso", { COL_PRED, COL_SUBJ, COL_OBJ }, no_stat);
    }

    print_duration("Write triples indexes", start);

    {   // B+tree creation SUBJECT=PREDICATE OBJECT
        size_t COL_SUBJ_PRED = 0, COL_OBJ = 1;

        Import::AllStat<2> all_stat;
        equal_sp.create_bpt(db_folder + "/equal_sp",
                            { COL_SUBJ_PRED, COL_OBJ },
                            all_stat);
        catalog.equal_sp_count = all_stat.all;

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
        catalog.equal_so_count = all_stat.all;
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
        catalog.equal_po_count = all_stat.all;

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
        catalog.equal_spo_count = all_stat.all;
    }

    // calling finish_indexing() closes and removes the file.
    triples.finish_indexing();
    equal_sp.finish_indexing();
    equal_so.finish_indexing();
    equal_po.finish_indexing();
    equal_spo.finish_indexing();
    free(external_strings);

    print_duration("Write special cases B+tree index", start);

    // Store IRI aliases, prefixes and literal datatypes/languages into catalog
    catalog.aliases  = std::move(aliases);
    catalog.prefixes = std::move(prefixes);
    catalog.datatypes.resize(datatype_ids_map.size());
    for (auto&& [datatype, id] : datatype_ids_map) {
        catalog.datatypes[id] = datatype;
    }
    catalog.languages.resize(language_ids_map.size());
    for (auto&& [language, id] : language_ids_map) {
        catalog.languages[id] = language;
    }

    catalog.save_changes();
    catalog.print();

    print_duration("Write catalog", start);
    print_duration("Total Import", import_start);
}
}} // Namespace Import::Rdf
