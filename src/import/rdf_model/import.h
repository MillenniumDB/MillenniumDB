#pragma once

#include <iostream>

#include "base/exceptions.h"
#include "base/graph_object/datetime.h"
#include "base/query/sparql/decimal.h"
#include "base/graph_object/decimal_inlined.h"
#include "import/external_string.h"
#include "import/inliner.h"
#include "import/disk_vector.h"
#include "import/stats_processor.h"
#include "storage/index/hash/strings_hash/strings_hash.h"
#include "query_optimizer/rdf_model/rdf_catalog.h"
#include "third_party/robin_hood/robin_hood.h"
#include "third_party/serd/reader.h"
#include "third_party/serd/serd.h"

// TODO: put inside namespace Import?
namespace ImportRdf {
class OnDiskImport {
public:
    OnDiskImport(const std::string& db_folder, size_t buffer_size_in_GB) :
        buffer_size_in_GB (buffer_size_in_GB),
        db_folder         (db_folder),
        catalog           (RdfCatalog("catalog.dat")),
        triples           (db_folder + "/tmp_triples"),
        equal_spo         (db_folder + "/tmp_equal_spo"),
        equal_sp          (db_folder + "/tmp_equal_sp"),
        equal_so          (db_folder + "/tmp_equal_so"),
        equal_po          (db_folder + "/tmp_equal_po") { }

    // Serd reader
    SerdReader* reader;
    // Serd environment, used for expanding IRIs using @prefix and @base
    SerdEnv* env = serd_env_new(NULL);
    // True if the current object has an error
    bool object_has_errors;

    void start_import(const std::string& input_filename, const std::string& prefixes_filename);

    void handle_subject(const SerdNode* subject) {
        switch (subject->type) {
        case SERD_URI:
            // Handle subject IRI
            {
                SerdNode subject_expanded = serd_env_expand_node(env, subject);
                save_subject_id_iri(const_cast<SerdNode*>(&subject_expanded));
                serd_node_free(&subject_expanded);
            }
            break;
        case SERD_CURIE:
            // Handle subject CURIE (prefixed IRI)
            {
                SerdNode subject_expanded = serd_env_expand_node(env, subject);
                save_subject_id_iri(const_cast<SerdNode*>(&subject_expanded));
                serd_node_free(&subject_expanded);
            }
            break;
        case SERD_BLANK:
            // Handle subject blank node
            save_subject_id_blank(subject);
            break;
        default:
            auto cuint = const_cast<uint8_t*>(subject->buf);
            auto cchar = reinterpret_cast<char*>(cuint);
            throw ImportException("Unexpected subject: \"" + std::string(cchar) + "\"");
            break;
        }
    }

    void handle_predicate(const SerdNode* predicate) {
        switch (predicate->type) {
        case SERD_URI:
            // Handle predicate IRI
            {
                SerdNode predicate_expanded = serd_env_expand_node(env, predicate);
                save_predicate_id_iri(const_cast<SerdNode*>(&predicate_expanded));
                serd_node_free(&predicate_expanded);
            }
            break;
        case SERD_CURIE:
            // Handle predicate CURIE (prefixed IRI)
            {
                SerdNode predicate_expanded = serd_env_expand_node(env, predicate);
                save_predicate_id_iri(const_cast<SerdNode*>(&predicate_expanded));
                serd_node_free(&predicate_expanded);
            }
            break;
        default:
            auto cuint = const_cast<uint8_t*>(predicate->buf);
            auto cchar = reinterpret_cast<char*>(cuint);
            throw ImportException("Unexpected predicate: \"" + std::string(cchar) + "\"");
            break;
        }
    }

    void handle_object(const SerdNode* object, const SerdNode* object_datatype, const SerdNode* object_lang) {
        object_has_errors = false;
        switch (object->type) {
        case SERD_URI:
            // Handle object IRI
            {
                SerdNode object_expanded = serd_env_expand_node(env, object);
                save_object_id_iri(const_cast<SerdNode*>(&object_expanded));
                serd_node_free(&object_expanded);
            }
            break;
        case SERD_CURIE:
            // Handle object CURIE (prefixed IRI)
            {
                SerdNode object_expanded = serd_env_expand_node(env, object);
                save_object_id_iri(const_cast<SerdNode*>(&object_expanded));
                serd_node_free(&object_expanded);
            }
            break;
        case SERD_BLANK:
            // Handle object blank node
            save_object_id_blank(object);
            break;
        case SERD_LITERAL:
            if (object_datatype) {
                // Handle object literal with datatype
                // Notice that an object's datatype is either an URI or a CURIE, so it could be necessary to expand it
                // Also there are some special cases that are handled differently (e.g. xsd:integer, xsd:dateTime, etc.)
                SerdNode object_datatype_expanded = serd_env_expand_node(env, object_datatype);
                save_object_id_literal_datatype(object, const_cast<SerdNode*>(&object_datatype_expanded));
                serd_node_free(&object_datatype_expanded);
            } else if (object_lang) {
                // Handle object literal with language tag
                save_object_id_literal_lang(object, object_lang);
            } else {
                // Handle object literal without datatype or language tag
                save_object_id_literal(object);
            }
            break;
        default:
            auto cuint = const_cast<uint8_t*>(object->buf);
            auto cchar = reinterpret_cast<char*>(cuint);
            throw ImportException("Unexpected object: \"" + std::string(cchar) + "\"");
            break;
        }
    }

    void save_triple() {
        if (subject_id == predicate_id) {
            equal_sp.push_back({subject_id, object_id});
            if (subject_id == object_id) {
                equal_spo.push_back({subject_id});
            }
        }
        if (subject_id == object_id) {
            equal_so.push_back({subject_id, predicate_id});
        }
        if (predicate_id == object_id) {
            equal_po.push_back({predicate_id, subject_id});
        }
        triples.push_back({ subject_id, predicate_id, object_id });
    }

private:
    size_t buffer_size_in_GB;

    std::string db_folder;
    RdfCatalog catalog;

    uint64_t subject_id;
    uint64_t predicate_id;
    uint64_t object_id;

    Import::DiskVector<3> triples;
    Import::DiskVector<1> equal_spo;
    Import::DiskVector<2> equal_sp;
    Import::DiskVector<2> equal_so;
    Import::DiskVector<2> equal_po;

    robin_hood::unordered_set<Import::ExternalString> external_strings_set;

    // External strings
    char*    external_strings;
    uint64_t external_strings_capacity;
    uint64_t external_strings_end;

    // Blank nodes
    uint64_t blank_node_count = 0;

    robin_hood::unordered_map<std::string, uint64_t> blank_ids_map;

    // IRI prefixes (configuration file)
    std::vector<std::string> prefixes;

    // Literal attributes
    uint64_t language_count = 0;
    uint64_t datatype_count = 0;

    robin_hood::unordered_map<std::string, uint64_t> datatype_ids_map;
    robin_hood::unordered_map<std::string, uint64_t> language_ids_map;

    // Generation of IDs
    // Save Subjects
    void save_subject_id_iri(const SerdNode* subject) {
        auto cuint = const_cast<uint8_t*>(subject->buf);
        auto cchar = reinterpret_cast<char*>(cuint);
        auto size  = subject->n_bytes;

        subject_id = get_iri_id(cchar, size);
    }

    void save_subject_id_blank(const SerdNode* subject) {
        auto cuint = const_cast<uint8_t*>(subject->buf);
        auto cchar = reinterpret_cast<char*>(cuint);

        subject_id = get_blank_id(cchar);
    }

    // Save Predicates
    void save_predicate_id_iri(const SerdNode* predicate) {
        auto cuint = const_cast<uint8_t*>(predicate->buf);
        auto cchar = reinterpret_cast<char*>(cuint);
        auto size  = predicate->n_bytes;

        predicate_id = get_iri_id(cchar, size);
    }

    // Save Objects
    void save_object_id_iri(const SerdNode* object) {
        auto cuint = const_cast<uint8_t*>(object->buf);
        auto cchar = reinterpret_cast<char*>(cuint);
        auto size  = object->n_bytes;

        object_id = get_iri_id(cchar, size);
    }

    void save_object_id_blank(const SerdNode* object) {
        auto cuint = const_cast<uint8_t*>(object->buf);
        auto cchar = reinterpret_cast<char*>(cuint);

        object_id = get_blank_id(cchar);
    }

    void save_object_id_literal(const SerdNode* object) {
        auto cuint = const_cast<uint8_t*>(object->buf);
        auto cchar = reinterpret_cast<char*>(cuint);
        auto size  = object->n_bytes;

        if (size < 8) {
            object_id = Inliner::inline_string(cchar) | ObjectId::MASK_STRING_INLINED;
        } else {
            object_id = get_or_create_external_string_id(cchar, size) | ObjectId::MASK_STRING_EXTERN;
        }
    }

    void save_object_id_literal_datatype(const SerdNode* object, const SerdNode* datatype) {
        // Object
        auto cuint = const_cast<uint8_t*>(object->buf);
        auto cchar = reinterpret_cast<char*>(cuint);
        auto size  = object->n_bytes;

        // Datatype
        auto cuint_datatype = const_cast<uint8_t*>(datatype->buf);
        auto cchar_datatype = reinterpret_cast<char*>(cuint_datatype);

        // Supported datatypes
        // xsd:dateTime
        if (strcmp(cchar_datatype, "http://www.w3.org/2001/XMLSchema#dateTime") == 0) {
            uint64_t datetime_id = DateTime::get_datetime_id(cchar);
            if (datetime_id == DateTime::INVALID_ID) {
                std::cout << "Warning [line " << reader->source.cur.line  << "] invalid datetime: " << cchar << '\n';
                object_has_errors = true;
            } else {
                object_id = datetime_id | ObjectId::MASK_DATETIME;
            }
        }
        // xsd:decimal
        else if (strcmp(cchar_datatype, "http://www.w3.org/2001/XMLSchema#decimal") == 0) {
            uint64_t decimal_id = DecimalInlined::get_decimal_id(cchar);
            if (decimal_id == DecimalInlined::INVALID_ID) {
                std::string str(cchar);
                std::string normalized = Decimal::normalize(str);
                object_id = get_or_create_external_string_id(normalized.c_str(), normalized.size()) | ObjectId::MASK_DECIMAL_EXTERN;
            } else {
                object_id = decimal_id | ObjectId::MASK_DECIMAL_INLINED;
            }
        }
        // xsd:boolean
        else if (strcmp(cchar_datatype, "http://www.w3.org/2001/XMLSchema#boolean") == 0) {
            if (strcmp(cchar, "true") == 0 || strcmp(cchar, "1") == 0) {
                object_id = ObjectId::MASK_BOOL | 0x01;
            }
            else if (strcmp(cchar, "false") == 0 || strcmp(cchar, "0") == 0) {
                object_id = ObjectId::MASK_BOOL | 0x00;
            }
            else {
                std::cout << "Warning [line " << reader->source.cur.line  << "] invalid boolean: " << cchar << '\n';
                object_has_errors = true;
            }
        }
        // Unsupported datatypes are stored as literals with datatype
        else {
            uint64_t datatype_id = get_datatype_id(cchar_datatype) << 40;
            if (size < 6) {
                object_id = Inliner::inline_string5(cchar) | ObjectId::MASK_STRING_DATATYPE_INLINED | datatype_id;
            } else {
                object_id = get_or_create_external_string_id(cchar, size) | ObjectId::MASK_STRING_DATATYPE_EXTERN | datatype_id;
            }
        }
    }

    void save_object_id_literal_lang(const SerdNode* object, const SerdNode* lang) {
        // Object
        auto cuint = const_cast<uint8_t*>(object->buf);
        auto cchar = reinterpret_cast<char*>(cuint);
        auto size  = object->n_bytes;

        // Lang
        auto cuint_lang = const_cast<uint8_t*>(lang->buf);
        auto cchar_lang = reinterpret_cast<char*>(cuint_lang);
        uint64_t lang_id = get_lang_id(cchar_lang) << 40;

        if (size < 6) {
            object_id = Inliner::inline_string5(cchar) | ObjectId::MASK_STRING_LANG_INLINED | lang_id;
        } else {
            object_id = get_or_create_external_string_id(cchar, size) | ObjectId::MASK_STRING_LANG_EXTERN | lang_id;
        }
    }

    uint64_t get_iri_id(const char* str, size_t str_len) {
        // If a prefix matches the IRI, store just the suffix and a pointer to the prefix
        uint64_t prefix_id = 0;
        for (size_t i = 0; i < prefixes.size(); ++i) {
            if (strncmp(str, prefixes[i].c_str(), prefixes[i].size()) == 0) {
                str += prefixes[i].size();
                str_len -= prefixes[i].size();
                // Shift prefix_id for preventing collision on the id
                prefix_id = i << 48;
                break;
            }
        }

        if (str_len < 7) {
            return Inliner::inline_iri(str) | ObjectId::MASK_IRI_INLINED | prefix_id;
        } else {
            return get_or_create_external_string_id(str, str_len) | ObjectId::MASK_IRI_EXTERN | prefix_id;
        }
    }

    uint64_t get_literal_id(const char* str, size_t str_len) {
        if (str_len < 8) {
            return Inliner::inline_string(str);
        } else {
            return get_or_create_external_string_id(str, str_len);
        }
    }

    uint64_t get_blank_id(const char* str) {
        auto it = blank_ids_map.find(str);
        if (it == blank_ids_map.end()) {
            // Blank node not seen before
            blank_node_count++;
            blank_ids_map.insert({ str, blank_node_count });
            catalog.blank_nodes_count = blank_node_count;
            return blank_node_count | ObjectId::MASK_ANON;
        } else {
            // Blank node found
            return it->second | ObjectId::MASK_ANON;
        }
    }

    // Object attributes methods
    uint64_t get_datatype_id(const char* datatype) {
        auto it = datatype_ids_map.find(datatype);
        if (it == datatype_ids_map.end()) {
            // Datatype not seen before
            datatype_ids_map.insert({ datatype, datatype_count });
            return datatype_count++;
        } else {
            // Datatype found
            return it->second;
        }
    }

    uint64_t get_lang_id(const char* lang) {
        auto it = language_ids_map.find(lang);
        if (it == language_ids_map.end()) {
            // Lang not seen before
            language_ids_map.insert({ lang, language_count });
            return language_count++;
        } else {
            // Lang found
            return it->second;
        }
    }

    // External string methods
    // void check_external_string_size(size_t str_len) {
    //     while (external_strings_end + str_len + 1 >= external_strings_capacity) {
    //         // duplicate buffer
    //         char* new_external_strings = new char[external_strings_capacity * 2];
    //         std::memcpy(new_external_strings, external_strings, external_strings_capacity);

    //         external_strings_capacity *= 2;

    //         delete[] external_strings;
    //         external_strings                = new_external_strings;
    //         Import::ExternalString::strings = external_strings;
    //     }
    // }

    // uint64_t get_or_create_external_string_id(const char* str, size_t str_len) {
    //     check_external_string_size(str_len);

    //     std::memcpy(&external_strings[external_strings_end], str, str_len);
    //     external_strings[external_strings_end + str_len] = '\0';
    //     Import::ExternalString s(external_strings_end);

    //     auto search = external_ids_map.find(s);
    //     if (search == external_ids_map.end()) {
    //         external_strings_end += str_len + 1;
    //         external_ids_map.insert(s);
    //         return s.offset;
    //     } else {
    //         return search->offset;
    //     }
    // }

    uint64_t get_or_create_external_string_id(const char* str, size_t str_len) {
        // reserve more space if needed
        while (external_strings_end
               + str_len
               + Import::ExternalString::MIN_PAGE_REMAINING_BYTES
               + Import::ExternalString::MAX_LEN_BYTES
               + 1 >= external_strings_capacity)
        {
            // duplicate buffer
            char* new_external_strings = new char[external_strings_capacity*2];
            std::memcpy(new_external_strings,
                        external_strings,
                        external_strings_capacity);

            external_strings_capacity *= 2;

            delete[] external_strings;
            external_strings = new_external_strings;
            Import::ExternalString::strings = external_strings;
        }


        // encode length
        size_t bytes_for_len = 0;

        auto* ptr = &external_strings[external_strings_end];
        size_t remaining_len = str_len;

        // TODO: move to string manager?
        while (remaining_len != 0) {
            if (remaining_len <= 127) {
                *ptr = static_cast<char>(remaining_len);
            } else {
                *ptr = static_cast<char>(remaining_len & 0x7FUL) | 0x80;
            }
            remaining_len = remaining_len >> 7;
            bytes_for_len++;
            ptr++;
        }

        // copy string
        std::memcpy(ptr, str, str_len);

        Import::ExternalString s(external_strings_end);
        auto found = external_strings_set.find(s);
        if (found == external_strings_set.end()) {
            external_strings_set.insert(s);
            external_strings_end += str_len + bytes_for_len;

            size_t remaining_in_block = StringManager::STRING_BLOCK_SIZE
                                        - (external_strings_end % StringManager::STRING_BLOCK_SIZE);
            if (remaining_in_block < Import::ExternalString::MIN_PAGE_REMAINING_BYTES) {
                external_strings_end += remaining_in_block;
            }

            return s.offset;
        } else {
            return found->offset;
        }
    }
};
} // namespace ImportRdf
