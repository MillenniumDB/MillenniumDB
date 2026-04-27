#include "conversions.h"

#include "graph_models/inliner.h"
#include "graph_models/rdf_model/iri_compression.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "query/parser/grammar/sparql/mdb_extensions.h"
#include "system/path_manager.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <sstream>

using namespace SPARQL;

uint64_t Conversions::unpack_blank(ObjectId oid)
{
    return oid.get_value();
}

ObjectId Conversions::string_simple_to_xsd(ObjectId oid)
{
    auto mod = oid.get_mod();
    return ObjectId(oid.get_value() | mod | ObjectId::MASK_STR_INL);
}

ObjectId Conversions::pack_string_xsd_inline(const char* str)
{
    return ObjectId(Inliner::inline_string(str) | ObjectId::MASK_STR_XSD_INL);
}

ObjectId Conversions::pack_iri_inline(const char* str, uint_fast8_t prefix_id)
{
    uint64_t prefix_id_shifted = static_cast<uint64_t>(prefix_id) << 48;
    return ObjectId(Inliner::inline_iri(str) | ObjectId::MASK_IRI_INL | prefix_id_shifted);
}

ObjectId Conversions::pack_string_datatype_inline(uint64_t datatype_id, const char* str)
{
    return ObjectId(
        Inliner::inline_string5(str) | ObjectId::MASK_STR_DATATYPE_INL | (datatype_id << TMP_SHIFT)
    );
}

ObjectId Conversions::pack_string_lang_inline(uint64_t lang_id, const char* str)
{
    return ObjectId(Inliner::inline_string5(str) | ObjectId::MASK_STR_DATATYPE_INL | (lang_id << TMP_SHIFT));
}

/**
 * First looks up the language in the datatype catalog and if found returns
 * the index (shifted).
 * If not found, the tmp_manager is checked, which creates the language if it does
 * not already exist. The id received from the tmp_manager has its most significant
 * bit (bit 15) set and is returned (shifted).
 */
uint64_t Conversions::get_language_id(const std::string& lang)
{
    const auto& languages = rdf_model.catalog.languages;
    auto it = std::find(languages.cbegin(), languages.cend(), lang);

    if (it != languages.end()) {
        return static_cast<uint64_t>(it - languages.cbegin()) << TMP_SHIFT;
    } else {
        return LAST_TMP_ID;
    }
}

/**
 * @brief First looks up the datatype in the datatype catalog and if found returns
 * the id (index). If not found, the tmp_manager is checked,
 * which creates the datatype if it does not already exist. The id received from the
 * tmp_manager has its most significant bit (bit 15) set and is returned.
 *
 * @param datatype the datatype to look up in string_manager and tmp_manager.
 * @return an uint64_t containing the shifted id.
 */
uint64_t Conversions::get_datatype_id(const std::string& datatype)
{
    const auto& datatypes = rdf_model.catalog.datatypes;
    auto it = std::find(datatypes.cbegin(), datatypes.cend(), datatype);

    if (it != datatypes.end()) {
        return static_cast<uint64_t>(it - datatypes.cbegin()) << TMP_SHIFT;
    } else {
        return LAST_TMP_ID;
    }
}

ObjectId Conversions::pack_string_lang(const std::string& lang, const std::string& str)
{
    uint64_t id;
    uint64_t lang_id = get_language_id(lang);

    std::string new_str;
    const std::string* str_ptr;

    if (lang_id == LAST_TMP_ID) {
        new_str = str + "@" + lang;
        str_ptr = &new_str;
    } else {
        str_ptr = &str;
    }

    if (str_ptr->size() <= ObjectId::STR_LANG_INLINE_BYTES) {
        id = Inliner::inline_string5(str_ptr->c_str()) | ObjectId::MASK_STR_LANG_INL;
    } else {
        auto str_id = string_manager.get_str_id(*str_ptr);
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            id = ObjectId::MASK_STR_LANG_EXT | str_id;
        } else {
            id = ObjectId::MASK_STR_LANG_TMP | tmp_manager.get_str_id(*str_ptr);
        }
    }
    return ObjectId(id | lang_id);
}

ObjectId Conversions::try_pack_string_datatype(const std::string& dt, const std::string& str)
{
    namespace MDBType = MDBExtensions::Type;
    if (dt.size() > MDBType::TYPE_PREFIX_IRI.size()
        && memcmp(dt.data(), MDBType::TYPE_PREFIX_IRI.data(), MDBType::TYPE_PREFIX_IRI.size()) == 0)
    {
        return try_pack_mdb_datatype(dt, str);
    }

    constexpr std::string_view XML_SCHEMA = "http://www.w3.org/2001/XMLSchema#";
    if (dt.size() > XML_SCHEMA.size() && memcmp(dt.data(), XML_SCHEMA.data(), XML_SCHEMA.size()) == 0) {
        return try_pack_xsd_datatype(dt, str);
    }

    return pack_string_datatype(dt, str);
}

ObjectId Conversions::try_pack_xsd_datatype(const std::string& dt, const std::string& str)
{
    constexpr std::string_view XML_SCHEMA = "http://www.w3.org/2001/XMLSchema#";
    const auto xsd_suffix = dt.substr(XML_SCHEMA.size());

    if (xsd_suffix == "date") {
        const auto object_id = DateTime::from_date(str);
        if (object_id == ObjectId::NULL_ID) {
            return pack_string_datatype(dt, str);
        }
        return pack_date(DateTime(object_id));
    } else if (xsd_suffix == "time") {
        const auto object_id = DateTime::from_time(str);
        if (object_id == ObjectId::NULL_ID) {
            return pack_string_datatype(dt, str);
        }
        return pack_date(DateTime(object_id));
    } else if (xsd_suffix == "dateTime") {
        const auto object_id = DateTime::from_dateTime(str);
        if (object_id == ObjectId::NULL_ID) {
            return pack_string_datatype(dt, str);
        }
        return pack_date(DateTime(object_id));
    } else if (xsd_suffix == "dateTimeStamp") {
        const auto object_id = DateTime::from_dateTimeStamp(str);
        if (object_id == ObjectId::NULL_ID) {
            return pack_string_datatype(dt, str);
        }
        return pack_date(object_id);
    } else if (xsd_suffix == "string") {
        return pack_string_xsd(str);
    } else if (xsd_suffix == "decimal") {
        bool error;
        const Decimal dec(str, &error);
        if (error) {
            return pack_string_datatype(dt, str);
        }
        return pack_decimal(dec);
    } else if (xsd_suffix == "float") {
        float flt;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), flt);

        if (ec == std::errc() && ptr == str.data() + str.size()) {
            return pack_float(flt);
        } else {
            return pack_string_datatype(dt, str);
        }
    } else if (xsd_suffix == "double") {
        double dbl;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), dbl);

        if (ec == std::errc() && ptr == str.data() + str.size()) {
            return pack_double(dbl);
        } else {
            return pack_string_datatype(dt, str);
        }
    } else if (
        xsd_suffix == "integer" || xsd_suffix == "long" || xsd_suffix == "int" || xsd_suffix == "short"
        || xsd_suffix == "byte"
    )
    {
        return try_pack_integer(dt, str);
    } else if (xsd_suffix == "nonPositiveInteger" || xsd_suffix == "negativeInteger") {
        return try_pack_integer(dt, str);
    } else if (
        xsd_suffix == "positiveInteger" || xsd_suffix == "nonNegativeInteger" || xsd_suffix == "unsignedLong"
        || xsd_suffix == "unsignedInt" || xsd_suffix == "unsignedShort" || xsd_suffix == "unsignedByte"
    )
    {
        return try_pack_integer(dt, str);
    } else if (xsd_suffix == "boolean") {
        if (str == "true" || str == "1") {
            return pack_bool(true);
        } else if (str == "false" || str == "0") {
            return pack_bool(false);
        } else {
            return pack_string_datatype(dt, str);
        }
    }
    return pack_string_datatype(dt, str);
}

ObjectId Conversions::try_pack_mdb_datatype(const std::string& dt, const std::string& str)
{
    namespace MDBType = MDBExtensions::Type;
    const auto mdbtype_suffix = dt.substr(MDBType::TYPE_PREFIX_IRI.size());
    if (mdbtype_suffix == MDBType::TENSOR_FLOAT_SUFFIX_IRI) {
        bool error;
        const auto tensor = tensor::Tensor<float>::from_literal(str, &error);
        if (error) {
            return pack_string_datatype(dt, str);
        }
        return pack_tensor<float>(tensor);
    } else if (mdbtype_suffix == MDBType::TENSOR_DOUBLE_SUFFIX_IRI) {
        bool error;
        const auto tensor = tensor::Tensor<double>::from_literal(str, &error);
        if (error) {
            return pack_string_datatype(dt, str);
        }
        return pack_tensor<double>(tensor);
    }

    return pack_string_datatype(dt, str);
}

ObjectId Conversions::pack_string_datatype(const std::string& dt, const std::string& str)
{
    uint64_t id;
    uint64_t datatype_id = get_datatype_id(dt);

    std::string new_str;
    const std::string* str_ptr;

    if (datatype_id == LAST_TMP_ID) {
        new_str = str + "^" + dt;
        str_ptr = &new_str;
    } else {
        str_ptr = &str;
    }

    if (str_ptr->size() <= ObjectId::STR_DT_INLINE_BYTES) {
        id = Inliner::inline_string5(str_ptr->c_str()) | ObjectId::MASK_STR_DATATYPE_INL;
    } else {
        auto str_id = string_manager.get_str_id(*str_ptr);
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            id = ObjectId::MASK_STR_DATATYPE_EXT | str_id;
        } else {
            id = ObjectId::MASK_STR_DATATYPE_TMP | tmp_manager.get_str_id(*str_ptr);
        }
    }
    return ObjectId(id | datatype_id);
}

ObjectId Conversions::try_pack_integer(const std::string& dt, const std::string& str)
{
    int64_t n;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), n);

    if (ec == std::errc() && ptr == str.data() + str.size()) {
        return pack_int(n);
    } else if (ec == std::errc::result_out_of_range) {
        // The integer is too big, we use a Decimal
        bool error;
        Decimal dec(str, &error);
        if (error) {
            return pack_string_datatype(dt, str);
        }
        return pack_decimal(dec);
    } else {
        // The string is not a valid integer
        return pack_string_datatype(dt, str);
    }
}

/*
Implemented according to the Effective Boolean Value rules:
https://www.w3.org/TR/2017/REC-xquery-31-20170321/#dt-ebv

If the ObjectId can not be converted to boolean, it returns a Null ObjectId
This Null ObjectId represents the Error Type according to the following:
https://www.w3.org/TR/sparql11-query/#evaluation
*/
ObjectId Conversions::to_boolean(ObjectId oid)
{
    uint64_t value = oid.get_value();

    switch (oid.subtype()) {
    case ObjectSubType::Bool:
        return oid;
    // Note: Extern strings will never be empty
    // Note: This assumes Decimal(0) is never represented as 0.0, 0.00, etc
    // Note: This assumes empty tensors will never be extern/tmp and they are the only inlined value
    case ObjectSubType::TensorFloat:
    case ObjectSubType::TensorDouble:
    case ObjectSubType::Decimal:
    case ObjectSubType::String:
    case ObjectSubType::Int:
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(value != 0));
    case ObjectSubType::Float: {
        auto f = unpack_float(oid);
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(f != 0 && !std::isnan(f)));
    }
    case ObjectSubType::Double: {
        auto d = unpack_double(oid);
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(d != 0 && !std::isnan(d)));
    }
    // Can not be converted to boolean
    default:
        return ObjectId::get_null();
    }
}

void Conversions::print_path_node(std::ostream& os, ObjectId node_id)
{
    Conversions::debug_print(os, node_id);
}

void Conversions::print_path_edge(std::ostream& os, ObjectId edge_id, bool inverse)
{
    os << ' ';
    if (inverse) {
        os << '^';
    }
    Conversions::debug_print(os, edge_id); // No need to escape os, as only IRIs are possible edges
    os << ' ';
}

// Converts an ObjectId into its lexical representation.
std::string Conversions::to_lexical_str(ObjectId oid)
{
    switch (oid.subtype()) {
    case ObjectSubType::Anon: {
        if (oid.type() == ObjectType::AnonInl) {
            return "_:b" + std::to_string(unpack_blank(oid));
        } else {
            return "_:c" + std::to_string(unpack_blank(oid));
        }
    }
    case ObjectSubType::String:
    case ObjectSubType::StringXsd: {
        return unpack_string(oid);
    }
    case ObjectSubType::Int: {
        int64_t i = unpack_int(oid);
        return std::to_string(i);
    }
    case ObjectSubType::Float: {
        float f = unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        return std::string(float_buffer);
    }
    case ObjectSubType::Double: {
        double d = unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        return std::string(double_buffer);
    }
    case ObjectSubType::Bool: {
        return (unpack_bool(oid) ? "true" : "false");
    }
    case ObjectSubType::Path: {
        std::stringstream ss;
        ss << '[';
        path_manager.for_each(
            Conversions::get_path_id(oid),
            [&](ObjectId oid) {
                print_path_node(ss, oid);
            },
            [&](ObjectId oid, bool reverse) {
                print_path_edge(ss, oid, reverse);
            }
        );
        ss << ']';
        return ss.str();
    }
    case ObjectSubType::Iri: {
        return unpack_iri(oid);
    }
    case ObjectSubType::StringDatatype: {
        auto&& [datatype, str] = unpack_string_datatype(oid);
        return str;
    }
    case ObjectSubType::StringLang: {
        auto&& [lang, str] = unpack_string_lang(oid);
        return str;
    }
    case ObjectSubType::TemporalLiteral: {
        DateTime datetime = unpack_date(oid);
        return datetime.get_value_string();
    }
    case ObjectSubType::Decimal: {
        Decimal decimal = unpack_decimal(oid);
        return decimal.to_string();
    }
    case ObjectSubType::Null: {
        return "";
    }
    case ObjectSubType::TensorFloat: {
        const auto tensor = unpack_tensor<float>(oid);
        return tensor.to_string();
    }
    case ObjectSubType::TensorDouble: {
        const auto tensor = unpack_tensor<double>(oid);
        return tensor.to_string();
    }
    case ObjectSubType::NamedNode:
    case ObjectSubType::Dictionary:
    case ObjectSubType::List:
    case ObjectSubType::Edge:
    case ObjectSubType::PGMetaData:
    case ObjectSubType::NotFound: {
        // Impossible for RDF Model
        break;
    }
    }
    return "";
}

std::ostream& Conversions::debug_print(std::ostream& os, ObjectId oid)
{
    switch (oid.subtype()) {
    case ObjectSubType::Anon: {
        if (oid.type() == ObjectType::AnonInl) {
            os << "_:b";
        } else {
            os << "_:c";
        }
        os << unpack_blank(oid);
        break;
    }
    case ObjectSubType::String: {
        os << '"';
        print_string(oid, os);
        os << '"';
        break;
    }
    case ObjectSubType::StringXsd: {
        os << '"';
        print_string(oid, os);
        os << "\"^^<http://www.w3.org/2001/XMLSchema#string>";
        break;
    }
    case ObjectSubType::Int: {
        os << Conversions::unpack_int(oid);
        break;
    }
    case ObjectSubType::Float: {
        float f = Conversions::unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        os << float_buffer;
        break;
    }
    case ObjectSubType::Double: {
        double d = Conversions::unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        os << double_buffer;
        break;
    }
    case ObjectSubType::Bool: {
        os << (Conversions::unpack_bool(oid) ? "true" : "false");
        break;
    }
    case ObjectSubType::Path: {
        using namespace std::placeholders;
        os << '[';
        path_manager.for_each(
            Conversions::get_path_id(oid),
            [&](ObjectId oid) {
                print_path_node(os, oid);
            },
            [&](ObjectId oid, bool reverse) {
                print_path_edge(os, oid, reverse);
            }
        );
        os << ']';
        break;
    }
    case ObjectSubType::Iri: {
        os << '<';
        Conversions::print_iri(oid, os);
        os << '>';
        break;
    }
    case ObjectSubType::StringDatatype: {
        auto&& [datatype, str] = Conversions::unpack_string_datatype(oid);
        os << '"';
        os << str;
        os << "\"^^<";
        os << datatype;
        os << ">";
        break;
    }
    case ObjectSubType::StringLang: {
        auto&& [lang, str] = Conversions::unpack_string_lang(oid);
        os << '"';
        os << str;
        os << "\"@";
        os << lang;
        break;
    }
    case ObjectSubType::TemporalLiteral: {
        DateTime datetime = Conversions::unpack_date(oid);

        os << '"' << datetime.get_value_string();
        os << "\"^^<" << datetime.get_datatype_string() << ">";
        break;
    }
    case ObjectSubType::Decimal: {
        auto decimal = Conversions::unpack_decimal(oid);
        os << decimal;
        break;
    }
    case ObjectSubType::Null: {
        os << "NULL";
        break;
    }
    case ObjectSubType::TensorFloat: {
        const auto tensor = unpack_tensor<float>(oid);
        os << tensor.to_string();
        break;
    }
    case ObjectSubType::TensorDouble: {
        const auto tensor = unpack_tensor<double>(oid);
        os << tensor.to_string();
        break;
    }
    case ObjectSubType::NamedNode:
    case ObjectSubType::Dictionary:
    case ObjectSubType::List:
    case ObjectSubType::Edge:
    case ObjectSubType::PGMetaData:
    case ObjectSubType::NotFound: {
        // Impossible in RDF MODEL
        break;
    }
    }
    return os;
}

ObjectId Conversions::pack_string_xsd(const std::string& str)
{
    uint64_t oid;
    if (str.size() == 0) {
        return ObjectId(ObjectId::MASK_STR_XSD_INL);
    } else if (str.size() <= ObjectId::STR_INLINE_BYTES) {
        oid = Inliner::inline_string(str.c_str()) | ObjectId::MASK_STR_XSD_INL;
    } else {
        auto str_id = string_manager.get_str_id(str);
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            oid = ObjectId::MASK_STR_XSD_INL | str_id;
        } else {
            oid = ObjectId::MASK_STR_XSD_TMP | tmp_manager.get_str_id(str);
        }
    }
    return ObjectId(oid);
}

ObjectId Conversions::pack_iri(const std::string& str)
{
    // If a prefix matches the IRI, store just the suffix and a pointer to the prefix
    auto [prefix_id, prefix_size] = rdf_model.catalog.prefixes.get_prefix_id(str);

    auto suffix = str.substr(prefix_size);
    // Pointer to the iri, it is updated if the iri is compressed
    const char* suffix_ptr = suffix.c_str();
    size_t suffix_len = suffix.size();

    auto prefix_id_shifted = static_cast<uint64_t>(prefix_id) << ObjectId::IRI_INLINE_BYTES * 8;

    uint64_t suffix_id;

    char* buffer_iri = get_query_ctx().get_buffer1();

    uint64_t iri_type_mask = ObjectId::MASK_IRI_INL;

    if (UUIDCompression::compress_lower(suffix_ptr, suffix_len, buffer_iri)) {
        suffix_ptr = buffer_iri;
        suffix_len = suffix_len - 20;
        iri_type_mask = ObjectId::MASK_IRI_UUID_LOWER_EXT;
    } else if (UUIDCompression::compress_upper(suffix_ptr, suffix_len, buffer_iri)) {
        suffix_ptr = buffer_iri;
        suffix_len = suffix_len - 20;
        iri_type_mask = ObjectId::MASK_IRI_UUID_UPPER_EXT;
    } else if (suffix_len >= HexCompression::MIN_LEN_TO_COMPRESS) {
        auto lower_hex_length = HexCompression::get_lower_hex_length(suffix_ptr, suffix_len);
        auto upper_hex_length = HexCompression::get_upper_hex_length(suffix_ptr, suffix_len);

        // Compress lowercase hex characters
        if (lower_hex_length >= upper_hex_length
            && lower_hex_length > HexCompression::MIN_HEX_LEN_TO_COMPRESS)
        {
            suffix_len = HexCompression::compress(suffix_ptr, suffix_len, lower_hex_length, buffer_iri);
            suffix_ptr = buffer_iri;
            iri_type_mask = ObjectId::MASK_IRI_HEX_LOWER_EXT;

            // Compress uppercase hex characters
        } else if (upper_hex_length > HexCompression::MIN_HEX_LEN_TO_COMPRESS) {
            suffix_len = HexCompression::compress(suffix_ptr, suffix_len, upper_hex_length, buffer_iri);
            suffix_ptr = buffer_iri;
            iri_type_mask = ObjectId::MASK_IRI_HEX_UPPER_EXT;
        }
    }

    if (suffix_len <= ObjectId::IRI_INLINE_BYTES) {
        suffix_id = Inliner::inline_iri(suffix_ptr) | ObjectId::MASK_IRI_INL;
    } else {
        auto str_id = string_manager.get_str_id(std::string(suffix_ptr, suffix_len));
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            suffix_id = iri_type_mask | str_id;
        } else {
            iri_type_mask &= ~ObjectId::MOD_MASK; // delete ext mask
            suffix_id = iri_type_mask | ObjectId::MOD_TMP
                      | tmp_manager.get_str_id(std::string(suffix_ptr, suffix_len));
        }
    }

    return ObjectId(suffix_id | prefix_id_shifted);
}

void Conversions::print_iri(ObjectId oid, std::ostream& os)
{
    auto prefix_id = oid.get_value() >> (ObjectId::IRI_INLINE_BYTES * 8);
    auto& prefix = rdf_model.catalog.prefixes.get_prefix(prefix_id);
    os << prefix;

    char* buffer = get_query_ctx().get_buffer1();

    switch (oid.type()) {
    case ObjectType::IriInl: {
        Inliner::print_string_inlined<6>(os, oid.id);
        break;
    }
    case ObjectType::IriExt: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(os, external_id);
        break;
    }
    case ObjectType::IriTmp: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(os, external_id);
        break;
    }
    case ObjectType::IriUuidLowerTmp: {
        auto size = print_tmp_iri_uuid_lower(oid, buffer);
        os.write(buffer, size);
        break;
    }
    case ObjectType::IriUuidLowerExt: {
        auto size = print_iri_uuid_lower(oid, buffer);
        os.write(buffer, size);
        break;
    }
    case ObjectType::IriUuidUpperTmp: {
        auto size = print_tmp_iri_uuid_upper(oid, buffer);
        os.write(buffer, size);
        break;
    }
    case ObjectType::IriUuidUpperExt: {
        auto size = print_iri_uuid_upper(oid, buffer);
        os.write(buffer, size);
        break;
    }
    case ObjectType::IriHexLowerTmp: {
        auto size = print_tmp_iri_hex_lower(oid, buffer);
        os.write(buffer, size);
        break;
    }
    case ObjectType::IriHexLowerExt: {
        auto size = print_iri_hex_lower(oid, buffer);
        os.write(buffer, size);
        break;
    }
    case ObjectType::IriHexUpperTmp: {
        auto size = print_tmp_iri_hex_upper(oid, buffer);
        os.write(buffer, size);
        break;
    }
    case ObjectType::IriHexUpperExt: {
        auto size = print_iri_hex_upper(oid, buffer);
        os.write(buffer, size);
        break;
    }
    default:
        throw LogicException("Called unpack_iri with incorrect ObjectId type, this should never happen");
    }
}

size_t Conversions::print_iri(ObjectId oid, char* out)
{
    auto prefix_id = oid.get_value() >> (ObjectId::IRI_INLINE_BYTES * 8);
    auto& prefix = rdf_model.catalog.prefixes.get_prefix(prefix_id);
    std::memcpy(out, prefix.data(), prefix.size());

    return prefix.size() + print_iri_suffix(oid, out + prefix.size());
}

size_t Conversions::print_iri_suffix(ObjectId oid, char* out)
{
    switch (oid.type()) {
    case ObjectType::IriInl: {
        return Inliner::print_string_inlined<6>(out, oid.id);
    }
    case ObjectType::IriExt: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        return string_manager.print_to_buffer(out, external_id);
    }
    case ObjectType::IriTmp: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        return tmp_manager.print_to_buffer(out, external_id);
    }
    case ObjectType::IriUuidLowerTmp: {
        return print_tmp_iri_uuid_lower(oid, out);
    }
    case ObjectType::IriUuidLowerExt: {
        return print_iri_uuid_lower(oid, out);
    }
    case ObjectType::IriUuidUpperTmp: {
        return print_tmp_iri_uuid_upper(oid, out);
    }
    case ObjectType::IriUuidUpperExt: {
        return print_iri_uuid_upper(oid, out);
    }
    case ObjectType::IriHexLowerTmp: {
        return print_tmp_iri_hex_lower(oid, out);
    }
    case ObjectType::IriHexLowerExt: {
        return print_iri_hex_lower(oid, out);
    }
    case ObjectType::IriHexUpperTmp: {
        return print_tmp_iri_hex_upper(oid, out);
    }
    case ObjectType::IriHexUpperExt: {
        return print_iri_hex_upper(oid, out);
    }
    default:
        throw LogicException("Called unpack_iri with incorrect ObjectId type, this should never happen");
    }
}

size_t Conversions::print_iri_uuid_lower(ObjectId oid, char* buffer)
{
    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    uint64_t size = string_manager.print_to_buffer(buffer, external_id);
    return UUIDCompression::decompress_lower(buffer, size);
}

size_t Conversions::print_iri_uuid_upper(ObjectId oid, char* buffer)
{
    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    uint64_t size = string_manager.print_to_buffer(buffer, external_id);
    return UUIDCompression::decompress_upper(buffer, size);
}

size_t Conversions::print_tmp_iri_uuid_lower(ObjectId oid, char* buffer)
{
    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    uint64_t size = tmp_manager.print_to_buffer(buffer, external_id);
    return UUIDCompression::decompress_lower(buffer, size);
}

size_t Conversions::print_tmp_iri_uuid_upper(ObjectId oid, char* buffer)
{
    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    uint64_t size = tmp_manager.print_to_buffer(buffer, external_id);
    return UUIDCompression::decompress_upper(buffer, size);
}

size_t Conversions::print_iri_hex_lower(ObjectId oid, char* buffer)
{
    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    auto size = string_manager.print_to_buffer(buffer, external_id);
    return HexCompression::decompress_lower(buffer, size);
}

size_t Conversions::print_tmp_iri_hex_lower(ObjectId oid, char* buffer)
{
    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    auto size = tmp_manager.print_to_buffer(buffer, external_id);
    return HexCompression::decompress_lower(buffer, size);
}

size_t Conversions::print_iri_hex_upper(ObjectId oid, char* buffer)
{
    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    auto size = string_manager.print_to_buffer(buffer, external_id);
    return HexCompression::decompress_upper(buffer, size);
}

size_t Conversions::print_tmp_iri_hex_upper(ObjectId oid, char* buffer)
{
    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    auto size = tmp_manager.print_to_buffer(buffer, external_id);
    return HexCompression::decompress_upper(buffer, size);
}

std::string Conversions::unpack_iri(ObjectId oid)
{
    auto prefix_id = oid.get_value() >> (ObjectId::IRI_INLINE_BYTES * 8);
    auto prefix = rdf_model.catalog.prefixes.get_prefix(prefix_id);
    char* buffer = get_query_ctx().get_buffer1();

    switch (oid.type()) {
    case ObjectType::IriInl: {
        return prefix + Inliner::get_string_inlined<6>(oid.id);
    }
    case ObjectType::IriExt: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        auto size = string_manager.print_to_buffer(buffer, external_id);
        prefix.append(buffer, size);
        return prefix;
    }
    case ObjectType::IriTmp: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        auto size = tmp_manager.print_to_buffer(buffer, external_id);
        prefix.append(buffer, size);
        return prefix;
    }
    case ObjectType::IriUuidLowerExt: {
        auto size = print_iri_uuid_lower(oid, buffer);
        prefix.append(buffer, size);
        return prefix;
    }
    case ObjectType::IriUuidUpperExt: {
        auto size = print_iri_uuid_upper(oid, buffer);
        prefix.append(buffer, size);
        return prefix;
    }
    case ObjectType::IriUuidLowerTmp: {
        auto size = print_tmp_iri_uuid_lower(oid, buffer);
        prefix.append(buffer, size);
        return prefix;
    }
    case ObjectType::IriUuidUpperTmp: {
        auto size = print_tmp_iri_uuid_upper(oid, buffer);
        prefix.append(buffer, size);
        return prefix;
    }
    case ObjectType::IriHexLowerTmp: {
        auto size = print_tmp_iri_hex_lower(oid, buffer);
        prefix.append(buffer, size);
        return prefix;
    }
    case ObjectType::IriHexUpperTmp: {
        auto size = print_tmp_iri_hex_upper(oid, buffer);
        prefix.append(buffer, size);
        return prefix;
    }
    case ObjectType::IriHexLowerExt: {
        auto size = print_iri_hex_lower(oid, buffer);
        prefix.append(buffer, size);
        return prefix;
    }
    case ObjectType::IriHexUpperExt: {
        auto size = print_iri_hex_upper(oid, buffer);
        prefix.append(buffer, size);
        return prefix;
    }
    default:
        throw LogicException("Called unpack_iri with incorrect ObjectId type, this should never happen");
    }
}

void Conversions::print_string(ObjectId oid, std::ostream& os)
{
    switch (oid.type()) {
    case ObjectType::StringInl:
    case ObjectType::StringXsdInl: {
        Inliner::print_string_inlined<7>(os, oid.id);
        break;
    }
    case ObjectType::StringExt:
    case ObjectType::StringXsdExt: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(os, external_id);
        break;
    }
    case ObjectType::StringTmp:
    case ObjectType::StringXsdTmp: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(os, external_id);
        break;
    }
    default:
        throw LogicException("Called print_string with incorrect ObjectId type, this should never happen");
    }
}

size_t Conversions::print_string(ObjectId oid, char* out)
{
    switch (oid.type()) {
    case ObjectType::StringInl:
    case ObjectType::StringXsdInl: {
        return Inliner::print_string_inlined<7>(out, oid.id);
    }
    case ObjectType::StringExt:
    case ObjectType::StringXsdExt: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        return string_manager.print_to_buffer(out, external_id);
    }
    case ObjectType::StringTmp:
    case ObjectType::StringXsdTmp: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        return tmp_manager.print_to_buffer(out, external_id);
    }
    default:
        throw LogicException("Called print_string with incorrect ObjectId type, this should never happen");
    }
}

std::string Conversions::unpack_string(ObjectId oid)
{
    switch (oid.type()) {
    case ObjectType::StringInl:
    case ObjectType::StringXsdInl: {
        return Inliner::get_string_inlined<ObjectId::STR_INLINE_BYTES>(oid.id);
    }
    case ObjectType::StringExt:
    case ObjectType::StringXsdExt: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(ss, external_id);
        return ss.str();
    }
    case ObjectType::StringTmp:
    case ObjectType::StringXsdTmp: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(ss, external_id);
        return ss.str();
    }
    default:
        throw LogicException("Called unpack_string with incorrect ObjectId type, this should never happen");
    }
}

// returns <lang, str>
std::pair<std::string, std::string> Conversions::unpack_string_lang(ObjectId oid)
{
    std::string str;
    std::string lang;

    switch (oid.type()) {
    case ObjectType::StringLangInl: {
        str = Inliner::get_string_inlined<5>(oid.id);
        break;
    }
    case ObjectType::StringLangExt: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(ss, external_id);

        str = ss.str();
        break;
    }
    case ObjectType::StringLangTmp: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(ss, external_id);

        str = ss.str();
        break;
    }
    default:
        throw LogicException(
            "Called unpack_string_lang with incorrect ObjectId type, this should never happen"
        );
    }

    auto lang_id = oid.get_value() >> TMP_SHIFT;
    if (lang_id == (LAST_TMP_ID >> TMP_SHIFT)) {
        auto found = str.find_last_of('@');
        if (found == std::string::npos) {
            throw LogicException("string with lang:LAST_TMP_ID `" + str + "` must have @ as separator");
        }
        lang = str.substr(found + 1);
        str = str.substr(0, found);
    } else {
        lang = rdf_model.catalog.languages[lang_id];
    }
    return std::make_pair(std::move(lang), std::move(str));
}

// Doesn't print the language
void Conversions::print_string_lang(ObjectId oid, std::ostream& os)
{
    switch (oid.type()) {
    case ObjectType::StringLangInl: {
        Inliner::print_string_inlined<5>(os, oid.id);
        break;
    }
    case ObjectType::StringLangExt: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(os, external_id);
        break;
    }
    case ObjectType::StringLangTmp: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(os, external_id);
        break;
    }
    default:
        throw LogicException(
            "Called unpack_string_lang with incorrect ObjectId type, this should never happen"
        );
    }
}

// Doesn't print the language
size_t Conversions::print_string_lang(ObjectId oid, char* out)
{
    switch (oid.type()) {
    case ObjectType::StringLangInl: {
        return Inliner::print_string_inlined<5>(out, oid.id);
    }
    case ObjectType::StringLangExt: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        return string_manager.print_to_buffer(out, external_id);
    }
    case ObjectType::StringLangTmp: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        return tmp_manager.print_to_buffer(out, external_id);
    }
    default:
        throw LogicException(
            "Called unpack_string_lang with incorrect ObjectId type, this should never happen"
        );
    }
}

// returns <datatype, str>
std::pair<std::string, std::string> Conversions::unpack_string_datatype(ObjectId oid)
{
    std::string str;
    std::string datatype;

    switch (oid.type()) {
    case ObjectType::StringDatatypeInl: {
        str = Inliner::get_string_inlined<5>(oid.id);
        break;
    }
    case ObjectType::StringDatatypeExt: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(ss, external_id);

        str = ss.str();
        break;
    }
    case ObjectType::StringDatatypeTmp: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(ss, external_id);

        str = ss.str();
        break;
    }
    default:
        throw LogicException(
            "Called unpack_string_data with incorrect ObjectId type, this should never happen"
        );
    }

    auto datatype_id = oid.get_value() >> TMP_SHIFT;
    if (datatype_id == (LAST_TMP_ID >> TMP_SHIFT)) {
        auto found = str.find_last_of('^');
        if (found == std::string::npos) {
            throw LogicException("string with datatype:LAST_TMP_ID `" + str + "` must have ^ as separator");
        }
        datatype = str.substr(found + 1);
        str = str.substr(0, found);
    } else {
        datatype = rdf_model.catalog.datatypes[datatype_id];
    }
    return std::make_pair(std::move(datatype), std::move(str));
}

// Doesn't print the datatype
void Conversions::print_string_datatype(ObjectId oid, std::ostream& os)
{
    switch (oid.type()) {
    case ObjectType::StringDatatypeInl: {
        Inliner::print_string_inlined<5>(os, oid.id);
        break;
    }
    case ObjectType::StringDatatypeExt: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(os, external_id);
        break;
    }
    case ObjectType::StringDatatypeTmp: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(os, external_id);
        break;
    }
    default:
        throw LogicException(
            "Called unpack_string_data with incorrect ObjectId type, this should never happen"
        );
    }
}

// Doesn't print the datatype
size_t Conversions::print_string_datatype(ObjectId oid, char* out)
{
    switch (oid.type()) {
    case ObjectType::StringDatatypeInl: {
        return Inliner::print_string_inlined<5>(out, oid.id);
    }
    case ObjectType::StringDatatypeExt: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        return string_manager.print_to_buffer(out, external_id);
    }
    case ObjectType::StringDatatypeTmp: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        return tmp_manager.print_to_buffer(out, external_id);
    }
    default:
        throw LogicException(
            "Called unpack_string_data with incorrect ObjectId type, this should never happen"
        );
    }
}
