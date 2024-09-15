#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/iri_prefixes.h"
#include "graph_models/rdf_model/rdf_catalog.h"
#include "import/disk_vector.h"
#include "import/exceptions.h"
#include "import/external_string.h"
#include "import/stats_processor.h"
#include "storage/index/hash/strings_hash/strings_hash.h"
#include "third_party/robin_hood/robin_hood.h"
#include "third_party/serd/reader.h"
#include "third_party/serd/serd.h"

namespace Import { namespace Rdf {
using namespace SPARQL;

class OnDiskImport {
public:
    // Maximum number of lines of .ttl to scan for base and prefix definitions
    // to detect IRI compression prefixes.
    static constexpr int MAX_LINES_TLL_PREFIX = 1000;

    size_t index_permutations;

    OnDiskImport(const std::string& db_folder, uint64_t buffer_size, size_t index_permutations = 3) :
        index_permutations (index_permutations),
        buffer_size (buffer_size),
        db_folder   (db_folder),
        catalog     (RdfCatalog("catalog.dat", index_permutations)),
        triples     (db_folder + "/tmp_triples"),
        equal_spo   (db_folder + "/tmp_equal_spo"),
        equal_sp    (db_folder + "/tmp_equal_sp"),
        equal_so    (db_folder + "/tmp_equal_so"),
        equal_po    (db_folder + "/tmp_equal_po") {}

    SerdReader* reader;

    // Serd environment, used for expanding IRIs using @prefix and @base
    SerdEnv* env = serd_env_new(NULL);

    // True if any element of the current triple has errors
    bool triple_has_errors;

    void start_import(const std::string& input_filename, const std::string& prefixes_filename);

    void handle_subject(const SerdNode* subject) {
        switch (subject->type) {
        case SERD_URI: { // complete IRI
            SerdNode subject_expanded = serd_env_expand_node(env, subject);
            save_subject_id_iri(&subject_expanded);
            serd_node_free(&subject_expanded);
            break;
        }
        case SERD_CURIE: { // prefixed IRI
            SerdNode subject_expanded = serd_env_expand_node(env, subject);
            save_subject_id_iri(&subject_expanded);
            serd_node_free(&subject_expanded);
            break;
        }
        case SERD_BLANK: { // blank node
            save_subject_id_blank(subject);
            break;
        }
        default:
            auto subject_str = reinterpret_cast<const char*>(subject->buf);
            throw ImportException("Unexpected subject: \"" + std::string(subject_str) + "\"");
            break;
        }
    }

    void handle_predicate(const SerdNode* predicate) {
        switch (predicate->type) {
        case SERD_URI: { // complete IRI
            SerdNode predicate_expanded = serd_env_expand_node(env, predicate);
            save_predicate_id_iri(const_cast<SerdNode*>(&predicate_expanded));
            serd_node_free(&predicate_expanded);
            break;
        }
        case SERD_CURIE: { // prefixed IRI
            SerdNode predicate_expanded = serd_env_expand_node(env, predicate);
            if (predicate_expanded.buf == NULL) {
                std::cout << "Using undefined prefix: " << reinterpret_cast<const char*>(predicate->buf) << std::endl;
                triple_has_errors = true;
                return;
            }
            save_predicate_id_iri(const_cast<SerdNode*>(&predicate_expanded));
            serd_node_free(&predicate_expanded);
            break;
        }
        default:
            auto predicate_str = reinterpret_cast<const char*>(predicate->buf);
            throw ImportException("Unexpected predicate: \"" + std::string(predicate_str) + "\"");
            break;
        }
    }

    void handle_object(const SerdNode* object, const SerdNode* object_datatype, const SerdNode* object_lang) {
        switch (object->type) {
        case SERD_URI: { // complete IRI
            SerdNode object_expanded = serd_env_expand_node(env, object);
            save_object_id_iri(&object_expanded);
            serd_node_free(&object_expanded);
            break;
        }
        case SERD_CURIE: { // prefixed IRI
            SerdNode object_expanded = serd_env_expand_node(env, object);
            if (object_expanded.buf == NULL) {
                std::cout << "Using undefined prefix: " << reinterpret_cast<const char*>(object->buf) << std::endl;
                triple_has_errors = true;
                return;
            }
            save_object_id_iri(&object_expanded);
            serd_node_free(&object_expanded);
            break;
        }
        case SERD_BLANK: { // blank node
            save_object_id_blank(object);
            break;
        }
        case SERD_LITERAL: {
            if (object_datatype) {
                // Handle object literal with datatype
                // Notice that an object's datatype is either an URI or a CURIE, so it could be necessary to expand it
                // Also there are some special cases that are handled differently (e.g. xsd:integer, xsd:dateTime, etc.)
                SerdNode object_datatype_expanded = serd_env_expand_node(env, object_datatype);
                if (object_datatype_expanded.buf == NULL) {
                    std::cout << "Using undefined prefix: " << reinterpret_cast<const char*>(object_datatype->buf) << std::endl;
                    triple_has_errors = true;
                    return;
                }
                save_object_id_literal_datatype(object, &object_datatype_expanded);
                serd_node_free(&object_datatype_expanded);
            } else if (object_lang) {
                // Handle object literal with language tag
                save_object_id_literal_lang(object, object_lang);
            } else {
                // Handle object literal without datatype or language tag
                save_object_id_literal(object);
            }
            break;
        }
        default:
            auto object_str = reinterpret_cast<const char*>(object->buf);
            throw ImportException("Unexpected object: \"" + std::string(object_str) + "\"");
            break;
        }
    }

   void save_triple() {
        if (triple_pending) {
            pending_triples->push_back({ subject_id.id, predicate_id.id, object_id.id });
            triple_pending = false;
        }
        else {
            if (subject_id == predicate_id) {
                equal_sp.push_back({subject_id.id, object_id.id});
                if (subject_id == object_id) {
                    equal_spo.push_back({subject_id.id});
                }
            }
            if (subject_id == object_id) {
                equal_so.push_back({subject_id.id, predicate_id.id});
            }
            if (predicate_id == object_id) {
                equal_po.push_back({predicate_id.id, subject_id.id});
            }
            triples.push_back({ subject_id.id, predicate_id.id, object_id.id });
        }
    }

private:
    uint64_t buffer_size;

    std::string db_folder;
    RdfCatalog catalog;

    ObjectId subject_id;
    ObjectId predicate_id;
    ObjectId object_id;

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

    // IRI prefixes (from configuration file)
    IriPrefixes prefixes;

    // saves all the datatypes seen and its assigned ids
    robin_hood::unordered_map<std::string, uint64_t> datatype_ids_map;

    // saves all the languages seen and its assigned ids
    robin_hood::unordered_map<std::string, uint64_t> language_ids_map;

    // when external strings is full, the id of new strings will be assigned later
    // triple_pending indicates if the current triple has an element that needs to
    // be revisited
    bool triple_pending = false;

    std::unique_ptr<std::fstream> pending_strings;

    std::unique_ptr<Import::DiskVector<3>> pending_triples;

    uint64_t previous_external_strings_offset = 0;

    uint64_t external_strings_align_offset = 0;

    void save_subject_id_iri(const SerdNode* subject) {
        auto subject_str = reinterpret_cast<const char*>(subject->buf);
        subject_id = get_iri_id(subject_str, subject->n_bytes);
    }

    void save_subject_id_blank(const SerdNode* subject) {
        auto subject_str = reinterpret_cast<const char*>(subject->buf);
        subject_id = get_blank_id(subject_str);
    }

    void save_predicate_id_iri(const SerdNode* predicate) {
        auto predicate_str = reinterpret_cast<const char*>(predicate->buf);
        predicate_id = get_iri_id(predicate_str, predicate->n_bytes);
    }

    void save_object_id_iri(const SerdNode* object) {
        auto object_str = reinterpret_cast<const char*>(object->buf);
        object_id = get_iri_id(object_str, object->n_bytes);
    }

    void save_object_id_blank(const SerdNode* object) {
        auto object_str = reinterpret_cast<const char*>(object->buf);
        object_id = get_blank_id(object_str);
    }

    void save_object_id_literal(const SerdNode* object) {
        auto object_str = reinterpret_cast<const char*>(object->buf);
        auto size = object->n_bytes;

        if (size <= RDF_OID::MAX_INLINE_LEN_STRING) {
            object_id = Conversions::pack_string_simple_inline(object_str);
        } else {
            object_id.id = get_or_create_external_id(object_str, size) | ObjectId::MASK_STRING_SIMPLE;
        }
    }

    void save_object_id_literal_datatype(const SerdNode* object, const SerdNode* datatype) {
        auto object_str = reinterpret_cast<const char*>(object->buf);
        auto object_size = object->n_bytes;

        auto datatype_str = reinterpret_cast<const char*>(datatype->buf);

        const char* xml_schema = "http://www.w3.org/2001/XMLSchema#";
        auto const xml_schema_len = std::strlen(xml_schema);

        bool is_xml_schema = false;
        if (std::strlen(datatype_str) > xml_schema_len) {
            is_xml_schema = std::memcmp(datatype_str, xml_schema, xml_schema_len) == 0;
        }

        if (!is_xml_schema) {
            uint64_t datatype_id = get_datatype_id(datatype_str);
            if (object_size <= ObjectId::STR_DT_INLINE_BYTES) {
                object_id = Conversions::pack_string_datatype_inline(datatype_id, object_str);
            } else {
                object_id.id = get_or_create_external_id(object_str, object_size)
                                                        | ObjectId::MASK_STRING_DATATYPE
                                                        | (datatype_id << Conversions::TMP_SHIFT);
            }
            return;
        }

        auto xsd_suffix = datatype_str + xml_schema_len;

        // Supported datatypes
        // DateTime: xsd:dateTime
        if (strcmp(xsd_suffix, "dateTime") == 0) {
            object_id.id = DateTime::from_dateTime(object_str);
            if (object_id.is_null()) {
                object_id = save_ill_typed(reader->source.cur.line, object_str, datatype_str);
            }
        }
        // Date: xsd:date
        else if (strcmp(xsd_suffix, "date") == 0) {
            object_id.id = DateTime::from_date(object_str);
            if (object_id.is_null()) {
                object_id = save_ill_typed(reader->source.cur.line, object_str, datatype_str);
            }
        }
        // Date: xsd:time
        else if (strcmp(xsd_suffix, "time") == 0) {
            object_id.id = DateTime::from_time(object_str);
            if (object_id.is_null()) {
                object_id = save_ill_typed(reader->source.cur.line, object_str, datatype_str);
            }
        }
        // Date: xsd:dateTimeStamp
        else if (strcmp(xsd_suffix, "dateTimeStamp") == 0) {
            object_id.id = DateTime::from_dateTimeStamp(object_str);
            if (object_id.is_null()) {
                object_id = save_ill_typed(reader->source.cur.line, object_str, datatype_str);
            }
        }
        // String: xsd:string
        else if (strcmp(xsd_suffix, "string") == 0) {
            if (object_size <= RDF_OID::MAX_INLINE_LEN_STRING) {
                object_id = Conversions::pack_string_xsd_inline(object_str);
            } else {
                object_id.id = get_or_create_external_id(object_str, object_size) | ObjectId::MASK_STRING_XSD;
            }
        }
        // Decimal: xsd:decimal
        else if (strcmp(xsd_suffix, "decimal") == 0) {
            bool error;
            Decimal dec(object_str, &error);

            if (error) {
                object_id = save_ill_typed(reader->source.cur.line, object_str, datatype_str);
            } else {
                object_id.id = dec.to_internal();
                if (object_id.is_null()) {
                    std::string external = dec.to_external();
                    object_id.id = get_or_create_external_id(external.c_str(), external.size()) | ObjectId::MASK_DECIMAL;
                }
            }
        }
        // Float: xsd:float
        else if (strcmp(xsd_suffix, "float") == 0) {
            try {
                float f = std::stof(object_str);
                object_id = Conversions::pack_float(f);
            } catch (const std::out_of_range& e) {
                object_id = save_ill_typed(reader->source.cur.line, object_str, datatype_str);
            } catch (const std::invalid_argument& e) {
                object_id = save_ill_typed(reader->source.cur.line, object_str, datatype_str);
            }
        }
        // Double: xsd:double
        else if (strcmp(xsd_suffix, "double") == 0) {
            try {
                double d = std::stod(object_str);
                const char* chars = reinterpret_cast<const char*>(&d);
                object_id.id = get_or_create_external_id(chars, sizeof(d)) | ObjectId::MASK_DOUBLE;
            } catch (const std::out_of_range& e) {
                object_id = save_ill_typed(reader->source.cur.line, object_str, datatype_str);
            } catch (const std::invalid_argument& e) {
                object_id = save_ill_typed(reader->source.cur.line, object_str, datatype_str);
            }
        }
        // Signed Integer: xsd:integer, xsd:long, xsd:int, xsd:short and xsd:byte
        else if (strcmp(xsd_suffix, "integer") == 0
              || strcmp(xsd_suffix, "long") == 0
              || strcmp(xsd_suffix, "int") == 0
              || strcmp(xsd_suffix, "short") == 0
              || strcmp(xsd_suffix, "byte") == 0
              // Negative Integer: xsd:nonPositiveInteger, xsd:negativeInteger
              || strcmp(xsd_suffix, "nonPositiveInteger") == 0
              || strcmp(xsd_suffix, "negativeInteger") == 0
              // Positive Integer:
              || strcmp(xsd_suffix, "positiveInteger") == 0
              || strcmp(xsd_suffix, "nonNegativeInteger") == 0
              || strcmp(xsd_suffix, "unsignedLong") == 0
              || strcmp(xsd_suffix, "unsignedInt") == 0
              || strcmp(xsd_suffix, "unsignedShort") == 0
              || strcmp(xsd_suffix, "unsignedByte") == 0)
        {
            bool int_parser_error;
            object_id = handle_integer_string(object_str, &int_parser_error);
            if (int_parser_error) {
                object_id = save_ill_typed(reader->source.cur.line, object_str, datatype_str);
            }
        }
        // xsd:boolean
        else if (strcmp(xsd_suffix, "boolean") == 0) {
            if (strcmp(object_str, "true") == 0 || strcmp(object_str, "1") == 0) {
                object_id = Conversions::pack_bool(true);
            } else if (strcmp(object_str, "false") == 0 || strcmp(object_str, "0") == 0) {
                object_id = Conversions::pack_bool(false);
            } else {
                object_id = save_ill_typed(reader->source.cur.line, object_str, datatype_str);
            }
        }
        // Unsupported datatypes are stored as literals with datatype
        else {
            uint64_t datatype_id = get_datatype_id(datatype_str);
            if (object_size <= ObjectId::STR_DT_INLINE_BYTES) {
                object_id = Conversions::pack_string_datatype_inline(datatype_id, object_str);
            } else {
                object_id.id = get_or_create_external_id(object_str, object_size)
                                                        | ObjectId::MASK_STRING_DATATYPE
                                                        | (datatype_id << Conversions::TMP_SHIFT);
            }
        }
    }

    void save_object_id_literal_lang(const SerdNode* object, const SerdNode* lang) {
        // Object
        auto object_str = reinterpret_cast<const char*>(object->buf);
        auto object_size  = object->n_bytes;

        // Lang
        auto lang_str = reinterpret_cast<const char*>(lang->buf);
        auto lang_id = get_lang_id(lang_str);

        if (object_size <= RDF_OID::MAX_INLINE_LEN_STRING_LANG) {
            object_id = Conversions::pack_string_lang_inline(lang_id, object_str);
        } else {
            object_id.id = get_or_create_external_id(object_str, object_size)
                                                    | ObjectId::MASK_STRING_LANG
                                                    | (lang_id << Conversions::TMP_SHIFT);
        }
    }

    ObjectId get_iri_id(const char* str, size_t str_len) {
        // If a prefix matches the IRI, store just the suffix and a pointer to the prefix
        auto [prefix_id, prefix_size] = prefixes.get_prefix_id(str, str_len);

        str += prefix_size;
        str_len -= prefix_size;

        if (str_len <= RDF_OID::MAX_INLINE_LEN_IRI) {
            return Conversions::pack_iri_inline(str, prefix_id);
        } else {
            uint64_t prefix_id_shifted = static_cast<uint64_t>(prefix_id) << 48;
            return ObjectId(get_or_create_external_id(str, str_len) | ObjectId::MASK_IRI | prefix_id_shifted);
        }
    }

    ObjectId get_blank_id(const char* str) {
        auto it = blank_ids_map.find(str);
        if (it == blank_ids_map.end()) {
            // Blank node not seen before
            auto new_blank_id = blank_node_count;
            blank_ids_map.insert({ str, new_blank_id });
            blank_node_count++;
            catalog.set_blank_node_count(blank_node_count);
            return Conversions::pack_blank_inline(new_blank_id);
        } else {
            // Blank node found
            return Conversions::pack_blank_inline(it->second);
        }
    }

    uint64_t get_datatype_id(const char* datatype) {
        auto it = datatype_ids_map.find(datatype);
        if (it == datatype_ids_map.end()) {
            // Datatype not seen before
            uint64_t new_id = datatype_ids_map.size();
            datatype_ids_map.insert({ datatype, new_id });
            return new_id;
        } else {
            // Datatype found
            return it->second;
        }
    }

    uint64_t get_lang_id(const char* lang) {
        auto it = language_ids_map.find(lang);
        if (it == language_ids_map.end()) {
            // Lang not seen before
            uint64_t new_id = language_ids_map.size();
            language_ids_map.insert({ lang, new_id });
            return new_id;
        } else {
            // Lang found
            return it->second;
        }
    }

    ObjectId save_ill_typed(unsigned line, const char* value, const char* datatype) {
        auto size = strlen(value);

        std::cout << "Warning [line " << line  << "] invalid " << datatype << ": " << value << '\n';

        uint64_t datatype_id = get_datatype_id(datatype);
        if (size <= ObjectId::STR_DT_INLINE_BYTES) {
            return Conversions::pack_string_datatype_inline(datatype_id, value);
        } else {
            return ObjectId(get_or_create_external_id(value, size)
                            | ObjectId::MASK_STRING_DATATYPE
                            | (datatype_id << Conversions::TMP_SHIFT));
        }
    }

    /**
     * @brief Handles the conversion of a string to an integer or decimal if
     * it does not fit in 56 bits. If the string is not a valid integer,
     * it will flag the object as having errors.
     *
     * @param str integer represented as a string
     * @return ObjectId of the integer
     */
    ObjectId handle_integer_string(const std::string& str, bool* error) {
        *error = false;
        try {
            size_t pos;
            int64_t i = std::stoll(str, &pos);
            // Check if the whole string was parsed
            if (pos != str.size()) {
                *error = true;
                return ObjectId::get_null();
            }
            // If the integer uses more than 56 bits, it must be converted into Decimal Extern (overflow)
            else if (i > Conversions::INTEGER_MAX || i < -Conversions::INTEGER_MAX) {
                std::string normalized = Decimal(str, error).to_external();
                if (*error) {
                    return ObjectId::get_null();
                }
                return ObjectId(get_or_create_external_id(normalized.c_str(), normalized.size()) | ObjectId::MASK_DECIMAL);
            } else {
                return Conversions::pack_int(i);
            }
        } catch (const std::out_of_range& e) {
            std::string normalized = Decimal(str, error).to_external();
            if (*error) {
                return ObjectId::get_null();
            }
            return ObjectId(get_or_create_external_id(normalized.c_str(), normalized.size()) | ObjectId::MASK_DECIMAL);
        } catch (const std::invalid_argument& e) {
            *error = true;
            return ObjectId::get_null();
        }
    }

    // Don't apply MOD_EXTERNAL mask (it might be a tmp to indicate that is a pending string)
    uint64_t get_or_create_external_id(const char* str, size_t str_len) {
        // reserving extra space for writing the string to search
        bool strings_full = external_strings_end + StringManager::STRING_BLOCK_SIZE > external_strings_capacity;

        // encode str_len inside external_strings
        auto bytes_for_len = StringManager::write_encoded_strlen(
            &external_strings[external_strings_end],
            str_len);

        // copy string
        std::memcpy(
            &external_strings[external_strings_end] + bytes_for_len,
            str,
            str_len);

        Import::ExternalString s(external_strings_end);
        auto found = external_strings_set.find(s);
        if (found == external_strings_set.end()) {
            if (!strings_full) {
                external_strings_set.insert(s);
                external_strings_end += str_len + bytes_for_len;

                // if there is no enough space left in the block for the next string
                // we align external_strings_end to StringManager::STRING_BLOCK_SIZE
                size_t remaining_in_block = StringManager::STRING_BLOCK_SIZE
                                            - (external_strings_end % StringManager::STRING_BLOCK_SIZE);
                if (remaining_in_block < StringManager::MIN_PAGE_REMAINING_BYTES) {
                    external_strings_end += remaining_in_block;
                }
                return ObjectId::MOD_EXTERNAL | (s.offset + previous_external_strings_offset - external_strings_align_offset);
            }
            else {
                triple_pending = true;
                uint64_t index = pending_strings->tellg();
                pending_strings->write(&external_strings[external_strings_end], bytes_for_len + str_len);
                return ObjectId::MOD_TMP | index;
            }
        } else {
            return ObjectId::MOD_EXTERNAL | (found->offset + previous_external_strings_offset - external_strings_align_offset);
        }
    }
};
}} // namespace Import::Rdf
