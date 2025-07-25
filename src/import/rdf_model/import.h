#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <boost/unordered/unordered_flat_set.hpp>
#include <boost/unordered/unordered_map.hpp>

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/iri_compression.h"
#include "graph_models/rdf_model/iri_prefixes.h"
#include "graph_models/rdf_model/rdf_catalog.h"
#include "import/disk_vector.h"
#include "import/external_helper.h"
#include "misc/istream.h"
#include "query/parser/grammar/sparql/mdb_extensions.h"

#include "third_party/serd/reader.h"
#include "third_party/serd/serd.h"

namespace Import { namespace Rdf {
using namespace SPARQL;

class OnDiskImport {
public:
    static constexpr char PENDING_TRIPLES_FILENAME_PREFIX[] = "tmp_pending_triples";

    static size_t istream_read(void* ptr, size_t size, size_t nmemb, void* stream);
    static int istream_error(void* stream);

    // Maximum number of lines of .ttl to scan for base and prefix definitions
    // to detect IRI compression prefixes.
    static constexpr int MAX_LINES_TLL_PREFIX = 1000;

    size_t index_permutations;

    OnDiskImport(
        const std::string& db_folder,
        uint64_t strings_buffer_size,
        uint64_t tensors_buffer_size,
        size_t index_permutations = 3
    ) :
        index_permutations(index_permutations),
        strings_buffer_size(strings_buffer_size),
        tensors_buffer_size(tensors_buffer_size),
        db_folder(db_folder),
        catalog(RdfCatalog("catalog.dat", index_permutations)),
        triples(db_folder + "/tmp_triples"),
        equal_spo(db_folder + "/tmp_equal_spo"),
        equal_sp(db_folder + "/tmp_equal_sp"),
        equal_so(db_folder + "/tmp_equal_so"),
        equal_po(db_folder + "/tmp_equal_po")
    {
        buffer_iri = new char[StringManager::MAX_STRING_SIZE];
    }

    ~OnDiskImport()
    {
        delete[] buffer_iri;
    }

    SerdReader* reader;

    // Serd environment, used for expanding IRIs using @prefix and @base
    SerdEnv* env = serd_env_new(NULL);

    // True if any element of the current triple has errors
    bool triple_has_errors;

    void start_import(
        MDBIstream& in,
        const std::string& prefixes_filename,
        std::vector<std::string>& input_files
    );

    void handle_subject(const SerdNode* subject);

    void handle_predicate(const SerdNode* predicate);

    void handle_object(const SerdNode* object, const SerdNode* object_datatype, const SerdNode* object_lang);

    void save_triple()
    {
        if ((subject_id.id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
            || (predicate_id.id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP
            || (object_id.id & ObjectId::MOD_MASK) == ObjectId::MOD_TMP)
        {
            pending_triples->push_back({ subject_id.id, predicate_id.id, object_id.id });
        } else {
            if (subject_id == predicate_id) {
                equal_sp.push_back({ subject_id.id, object_id.id });
                if (subject_id == object_id) {
                    equal_spo.push_back({ subject_id.id });
                }
            }
            if (subject_id == object_id) {
                equal_so.push_back({ subject_id.id, predicate_id.id });
            }
            if (predicate_id == object_id) {
                equal_po.push_back({ predicate_id.id, subject_id.id });
            }
            triples.push_back({ subject_id.id, predicate_id.id, object_id.id });
        }
    }

private:
    uint64_t strings_buffer_size;
    uint64_t tensors_buffer_size;

    std::string db_folder;
    RdfCatalog catalog;

    ObjectId subject_id;
    ObjectId predicate_id;
    ObjectId object_id;

    DiskVector<3> triples;
    DiskVector<1> equal_spo;
    DiskVector<2> equal_sp;
    DiskVector<2> equal_so;
    DiskVector<2> equal_po;

    // manager writing bytes to disk in a buffered manner
    std::unique_ptr<ExternalHelper> external_helper;

    // Blank nodes
    uint64_t blank_node_count = 0;

    boost::unordered_flat_map<std::string, uint64_t> blank_ids_map;

    // IRI prefixes (from configuration file)
    IriPrefixes prefixes;

    // saves all the datatypes seen and its assigned ids
    boost::unordered_flat_map<std::string, uint64_t> datatype_ids_map;

    // saves all the languages seen and its assigned ids
    boost::unordered_flat_map<std::string, uint64_t> language_ids_map;

    std::unique_ptr<DiskVector<3>> pending_triples;

    // buffer used to store a copy of the iri and compress it
    char* buffer_iri;

    void save_subject_id_iri(const SerdNode* subject)
    {
        auto subject_str = reinterpret_cast<const char*>(subject->buf);
        subject_id = get_iri_id(subject_str, subject->n_bytes);
    }

    void save_subject_id_blank(const SerdNode* subject)
    {
        auto subject_str = reinterpret_cast<const char*>(subject->buf);
        subject_id = get_blank_id(subject_str);
    }

    void save_predicate_id_iri(const SerdNode* predicate)
    {
        auto predicate_str = reinterpret_cast<const char*>(predicate->buf);
        predicate_id = get_iri_id(predicate_str, predicate->n_bytes);
    }

    void save_object_id_iri(const SerdNode* object)
    {
        auto object_str = reinterpret_cast<const char*>(object->buf);
        object_id = get_iri_id(object_str, object->n_bytes);
    }

    void save_object_id_blank(const SerdNode* object)
    {
        auto object_str = reinterpret_cast<const char*>(object->buf);
        object_id = get_blank_id(object_str);
    }

    void save_object_id_literal(const SerdNode* object)
    {
        auto object_str = reinterpret_cast<const char*>(object->buf);
        auto size = object->n_bytes;

        if (size <= RDF_OID::MAX_INLINE_LEN_STRING) {
            object_id = Conversions::pack_string_simple_inline(object_str);
        } else {
            object_id.id = external_helper->get_or_create_external_string_id(object_str, size)
                         | ObjectId::MASK_STRING_SIMPLE;
        }
    }

    void save_object_id_unsupported_datatype(const char* str, uint64_t str_size, const char* dt)
    {
        const uint64_t datatype_id = get_datatype_id(dt);
        if (str_size <= ObjectId::STR_DT_INLINE_BYTES) {
            object_id = Conversions::pack_string_datatype_inline(datatype_id, str);
            return;
        }
        object_id.id = external_helper->get_or_create_external_string_id(str, str_size)
                     | ObjectId::MASK_STRING_DATATYPE | (datatype_id << Conversions::TMP_SHIFT);
    }

    template<typename T>
    void try_save_tensor(const char* str, const char* dt)
    {
        bool error;
        const auto tensor = tensor::Tensor<T>::from_literal(str, &error);
        if (error) {
            object_id = save_ill_typed(reader->source.cur.line, str, dt);
            return;
        }

        if (tensor.empty()) {
            object_id.id = tensor.get_inline_mask();
            return;
        }

        const auto bytes = reinterpret_cast<const char*>(tensor.data());
        const auto num_bytes = sizeof(T) * tensor.size();

        object_id.id = tensor::Tensor<T>::get_subtype()
                     | external_helper->get_or_create_external_tensor_id(bytes, num_bytes);
    }

    void try_save_object_id_mdbtype(const char* str, uint64_t str_size, const char* dt)
    {
        namespace MDBType = MDBExtensions::Type;
        const char* mdbtype_suffix = dt + MDBType::TYPE_PREFIX_IRI.size();

        // Supported mdb datatypes
        // mdbtype:tensorFloat
        if (strcmp(mdbtype_suffix, MDBType::TENSOR_FLOAT_SUFFIX_IRI.data()) == 0) {
            try_save_tensor<float>(str, dt);
        }
        // mdbtype:tensorDouble
        else if (strcmp(mdbtype_suffix, MDBType::TENSOR_DOUBLE_SUFFIX_IRI.data()) == 0)
        {
            try_save_tensor<double>(str, dt);
        }
        // Unsupported datatypes are stored as literals with datatype
        else
        {
            save_object_id_unsupported_datatype(str, str_size, dt);
        }
    }

    void try_save_object_id_xsdtype(const char* str, uint64_t str_size, const char* dt)
    {
        constexpr std::string_view XML_SCHEMA = "http://www.w3.org/2001/XMLSchema#";
        const auto xsd_suffix = dt + XML_SCHEMA.size();

        // Supported xsd datatypes
        // DateTime: xsd:dateTime
        if (strcmp(xsd_suffix, "dateTime") == 0) {
            object_id.id = DateTime::from_dateTime(str);
            if (object_id.is_null()) {
                object_id = save_ill_typed(reader->source.cur.line, str, dt);
            }
        }
        // Date: xsd:date
        else if (strcmp(xsd_suffix, "date") == 0)
        {
            object_id.id = DateTime::from_date(str);
            if (object_id.is_null()) {
                object_id = save_ill_typed(reader->source.cur.line, str, dt);
            }
        }
        // Date: xsd:time
        else if (strcmp(xsd_suffix, "time") == 0)
        {
            object_id.id = DateTime::from_time(str);
            if (object_id.is_null()) {
                object_id = save_ill_typed(reader->source.cur.line, str, dt);
            }
        }
        // Date: xsd:dateTimeStamp
        else if (strcmp(xsd_suffix, "dateTimeStamp") == 0)
        {
            object_id.id = DateTime::from_dateTimeStamp(str);
            if (object_id.is_null()) {
                object_id = save_ill_typed(reader->source.cur.line, str, dt);
            }
        }
        // String: xsd:string
        else if (strcmp(xsd_suffix, "string") == 0)
        {
            if (str_size <= RDF_OID::MAX_INLINE_LEN_STRING) {
                object_id = Conversions::pack_string_xsd_inline(str);
            } else {
                object_id.id = external_helper->get_or_create_external_string_id(str, str_size)
                             | ObjectId::MASK_STRING_XSD;
            }
        }
        // Decimal: xsd:decimal
        else if (strcmp(xsd_suffix, "decimal") == 0)
        {
            bool error;
            Decimal dec(str, &error);

            if (error) {
                object_id = save_ill_typed(reader->source.cur.line, str, dt);
            } else {
                if (dec.can_inline()) {
                    object_id.id = dec.serialize_inlined() | ObjectId::MASK_DECIMAL_INLINED;
                } else {
                    char dec_buffer[Decimal::EXTERN_BUFFER_SIZE];
                    dec.serialize_extern(dec_buffer);
                    object_id.id = external_helper->get_or_create_external_string_id(
                                       dec_buffer,
                                       Decimal::EXTERN_BUFFER_SIZE
                                   )
                                 | ObjectId::MASK_DECIMAL;
                }
            }
        }
        // Float: xsd:float
        else if (strcmp(xsd_suffix, "float") == 0)
        {
            try {
                float f = std::stof(str);
                object_id = Conversions::pack_float(f);
            } catch (const std::out_of_range& e) {
                object_id = save_ill_typed(reader->source.cur.line, str, dt);
            } catch (const std::invalid_argument& e) {
                object_id = save_ill_typed(reader->source.cur.line, str, dt);
            }
        }
        // Double: xsd:double
        else if (strcmp(xsd_suffix, "double") == 0)
        {
            try {
                double d = std::stod(str);
                const char* chars = reinterpret_cast<const char*>(&d);
                object_id.id = external_helper->get_or_create_external_string_id(chars, sizeof(d))
                             | ObjectId::MASK_DOUBLE;
            } catch (const std::out_of_range& e) {
                object_id = save_ill_typed(reader->source.cur.line, str, dt);
            } catch (const std::invalid_argument& e) {
                object_id = save_ill_typed(reader->source.cur.line, str, dt);
            }
        }
        // Signed Integer: xsd:integer, xsd:long, xsd:int, xsd:short and xsd:byte
        else if (strcmp(xsd_suffix, "integer") == 0 || strcmp(xsd_suffix, "long") == 0
                 || strcmp(xsd_suffix, "int") == 0 || strcmp(xsd_suffix, "short") == 0
                 || strcmp(xsd_suffix, "byte") == 0
                 // Negative Integer: xsd:nonPositiveInteger, xsd:negativeInteger
                 || strcmp(xsd_suffix, "nonPositiveInteger") == 0
                 || strcmp(xsd_suffix, "negativeInteger") == 0
                 // Positive Integer:
                 || strcmp(xsd_suffix, "positiveInteger") == 0
                 || strcmp(xsd_suffix, "nonNegativeInteger") == 0 || strcmp(xsd_suffix, "unsignedLong") == 0
                 || strcmp(xsd_suffix, "unsignedInt") == 0 || strcmp(xsd_suffix, "unsignedShort") == 0
                 || strcmp(xsd_suffix, "unsignedByte") == 0)
        {
            bool int_parser_error;
            object_id = handle_integer_string(str, &int_parser_error);
            if (int_parser_error) {
                object_id = save_ill_typed(reader->source.cur.line, str, dt);
            }
        }
        // xsd:boolean
        else if (strcmp(xsd_suffix, "boolean") == 0)
        {
            if (strcmp(str, "true") == 0 || strcmp(str, "1") == 0) {
                object_id = Conversions::pack_bool(true);
            } else if (strcmp(str, "false") == 0 || strcmp(str, "0") == 0) {
                object_id = Conversions::pack_bool(false);
            } else {
                object_id = save_ill_typed(reader->source.cur.line, str, dt);
            }
        }
        // Unsupported datatypes are stored as literals with datatype
        else
        {
            save_object_id_unsupported_datatype(str, str_size, dt);
        }
    }

    void save_object_id_literal_datatype(const SerdNode* object, const SerdNode* datatype)
    {
        namespace MDBType = MDBExtensions::Type;

        const auto str = reinterpret_cast<const char*>(object->buf);
        const auto str_size = object->n_bytes;

        const auto dt = reinterpret_cast<const char*>(datatype->buf);
        const auto dt_size = datatype->n_bytes;

        if (dt_size > MDBType::TYPE_PREFIX_IRI.size()
            && memcmp(dt, MDBType::TYPE_PREFIX_IRI.data(), MDBType::TYPE_PREFIX_IRI.size()) == 0)
        {
            try_save_object_id_mdbtype(str, str_size, dt);
            return;
        }

        constexpr std::string_view XML_SCHEMA = "http://www.w3.org/2001/XMLSchema#";
        if (dt_size > XML_SCHEMA.size() && memcmp(dt, XML_SCHEMA.data(), XML_SCHEMA.size()) == 0) {
            try_save_object_id_xsdtype(str, str_size, dt);
            return;
        }

        // Unsupported datatypes are stored as literals with datatype
        return save_object_id_unsupported_datatype(str, str_size, dt);
    }

    void save_object_id_literal_lang(const SerdNode* object, const SerdNode* lang)
    {
        // Object
        auto object_str = reinterpret_cast<const char*>(object->buf);
        auto object_size = object->n_bytes;

        // Lang
        auto lang_str = reinterpret_cast<const char*>(lang->buf);
        auto lang_id = get_lang_id(lang_str);

        if (object_size <= RDF_OID::MAX_INLINE_LEN_STRING_LANG) {
            object_id = Conversions::pack_string_lang_inline(lang_id, object_str);
        } else {
            object_id.id = external_helper->get_or_create_external_string_id(object_str, object_size)
                         | ObjectId::MASK_STRING_LANG | (lang_id << Conversions::TMP_SHIFT);
        }
    }

    ObjectId get_iri_id(const char* str, size_t str_len)
    {
        // If a prefix matches the IRI, store just the suffix and a pointer to the prefix
        auto [prefix_id, prefix_size] = prefixes.get_prefix_id(str, str_len);
        uint64_t prefix_id_shifted = static_cast<uint64_t>(prefix_id) << 48;

        str += prefix_size;
        str_len -= prefix_size;

        if (UUIDCompression::compress_lower(str, str_len, buffer_iri)) {
            str_len = str_len - 20;
            return ObjectId(
                external_helper->get_or_create_external_string_id(buffer_iri, str_len)
                | (ObjectId::MASK_IRI_UUID_LOWER & (~ObjectId::MOD_MASK)) | prefix_id_shifted
            );

        } else if (UUIDCompression::compress_upper(str, str_len, buffer_iri)) {
            str_len = str_len - 20;
            return ObjectId(
                external_helper->get_or_create_external_string_id(buffer_iri, str_len)
                | (ObjectId::MASK_IRI_UUID_UPPER & (~ObjectId::MOD_MASK)) | prefix_id_shifted
            );
        }

        if (str_len >= HexCompression::MIN_LEN_TO_COMPRESS) {
            auto lower_hex_length = HexCompression::get_lower_hex_length(str, str_len);
            auto upper_hex_length = HexCompression::get_upper_hex_length(str, str_len);

            // Compress lowercase hex characters
            if (lower_hex_length >= upper_hex_length
                && lower_hex_length > HexCompression::MIN_HEX_LEN_TO_COMPRESS)
            {
                str_len = HexCompression::compress(str, str_len, lower_hex_length, buffer_iri);
                return ObjectId(
                    external_helper->get_or_create_external_string_id(buffer_iri, str_len)
                    | (ObjectId::MASK_IRI_HEX_LOWER & (~ObjectId::MOD_MASK)) | prefix_id_shifted
                );

                // Compress uppercase hex characters
            } else if (upper_hex_length > HexCompression::MIN_HEX_LEN_TO_COMPRESS) {
                str_len = HexCompression::compress(str, str_len, upper_hex_length, buffer_iri);
                return ObjectId(
                    external_helper->get_or_create_external_string_id(buffer_iri, str_len)
                    | (ObjectId::MASK_IRI_HEX_UPPER & (~ObjectId::MOD_MASK)) | prefix_id_shifted
                );
            }
        }

        if (str_len <= RDF_OID::MAX_INLINE_LEN_IRI) {
            return SPARQL::Conversions::pack_iri_inline(str, prefix_id);
        } else {
            return ObjectId(
                external_helper->get_or_create_external_string_id(str, str_len) | ObjectId::MASK_IRI
                | prefix_id_shifted
            );
        }
    }

    ObjectId get_blank_id(const char* str)
    {
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

    uint64_t get_datatype_id(const char* datatype)
    {
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

    uint64_t get_lang_id(const char* lang)
    {
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

    ObjectId save_ill_typed(unsigned line, const char* value, const char* datatype);

    /**
     * @brief Handles the conversion of a string to an integer or decimal if
     * it does not fit in 56 bits. If the string is not a valid integer,
     * it will flag the object as having errors.
     *
     * @param str integer represented as a string
     * @return ObjectId of the integer
     */
    ObjectId handle_integer_string(const std::string& str, bool* error)
    {
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
            else if (i > Conversions::INTEGER_MAX || i < -Conversions::INTEGER_MAX)
            {
                Decimal dec(str, error);
                if (*error) {
                    return ObjectId::get_null();
                }

                char dec_buffer[Decimal::EXTERN_BUFFER_SIZE];
                dec.serialize_extern(dec_buffer);
                return ObjectId(
                    external_helper->get_or_create_external_string_id(dec_buffer, Decimal::EXTERN_BUFFER_SIZE)
                    | ObjectId::MASK_DECIMAL
                );
            } else {
                return Conversions::pack_int(i);
            }
        } catch (const std::out_of_range& e) {
            Decimal dec(str, error);
            if (*error) {
                return ObjectId::get_null();
            }

            char dec_buffer[Decimal::EXTERN_BUFFER_SIZE];
            dec.serialize_extern(dec_buffer);
            return ObjectId(
                external_helper->get_or_create_external_string_id(dec_buffer, Decimal::EXTERN_BUFFER_SIZE)
                | ObjectId::MASK_DECIMAL
            );
        } catch (const std::invalid_argument& e) {
            *error = true;
            return ObjectId::get_null();
        }
    }
};
}} // namespace Import::Rdf
