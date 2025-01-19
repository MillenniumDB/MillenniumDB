#include "conversions.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <sstream>

#include "graph_models/inliner.h"
#include "graph_models/rdf_model/iri_compression.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "graph_models/rdf_model/rdf_object_id.h"
#include "system/path_manager.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

using namespace SPARQL;

uint64_t Conversions::unpack_blank(ObjectId oid) {
    return oid.get_value();
}

ObjectId Conversions::string_simple_to_xsd(ObjectId oid) {
    auto mod = oid.get_mod();
    return ObjectId(oid.get_value() | mod | ObjectId::MASK_STRING_SIMPLE);
}


ObjectId Conversions::pack_string_simple_inline(const char* str) {
    return ObjectId(
        Inliner::inline_string(str) | ObjectId::MASK_STRING_SIMPLE_INLINED
    );
}


ObjectId Conversions::pack_string_xsd_inline(const char* str) {
    return ObjectId(
        Inliner::inline_string(str) | ObjectId::MASK_STRING_XSD_INLINED
    );
}


ObjectId Conversions::pack_iri_inline(const char* str, uint_fast8_t prefix_id) {
    uint64_t prefix_id_shifted = static_cast<uint64_t>(prefix_id) << 48;
    return ObjectId(
        Inliner::inline_iri(str) | ObjectId::MASK_IRI_INLINED | prefix_id_shifted
    );
}


ObjectId Conversions::pack_string_datatype_inline(uint64_t datatype_id, const char* str) {
    return ObjectId(
        Inliner::inline_string5(str) | ObjectId::MASK_STRING_DATATYPE_INLINED | (datatype_id << TMP_SHIFT)
    );
}


ObjectId Conversions::pack_string_lang_inline(uint64_t lang_id, const char* str) {
    return ObjectId(
        Inliner::inline_string5(str) | ObjectId::MASK_STRING_LANG_INLINED | (lang_id << TMP_SHIFT)
    );
}


/**
 * First looks up the language in the datatype catalog and if found returns
 * the index (shifted).
 * If not found, the tmp_manager is checked, which creates the language if it does
 * not already exist. The id received from the tmp_manager has its most significant
 * bit (bit 15) set and is returned (shifted).
 */
uint64_t Conversions::get_language_id(const std::string& lang) {
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
uint64_t Conversions::get_datatype_id(const std::string& datatype) {
    const auto& datatypes = rdf_model.catalog.datatypes;
    auto it = std::find(datatypes.cbegin(), datatypes.cend(), datatype);

    if (it != datatypes.end()) {
        return static_cast<uint64_t>(it - datatypes.cbegin()) << TMP_SHIFT;
    } else {
        return LAST_TMP_ID;
    }
}


ObjectId Conversions::pack_string_lang(const std::string& lang, const std::string& str) {
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
            id = Inliner::inline_string5(str_ptr->c_str()) | ObjectId::MASK_STRING_LANG_INLINED;
        } else {
            auto str_id = string_manager.get_str_id(*str_ptr);
            if (str_id != ObjectId::MASK_NOT_FOUND) {
                id = ObjectId::MASK_STRING_LANG_EXTERN | str_id;
            } else {
                id = ObjectId::MASK_STRING_LANG_TMP | tmp_manager.get_str_id(*str_ptr);
            }
        }
        return ObjectId(id | lang_id);
}


ObjectId Conversions::try_pack_string_datatype(const std::string& dt, const std::string& str) {
    const char* xml_schema = "http://www.w3.org/2001/XMLSchema#";
    auto const xml_schema_len = std::strlen(xml_schema);

    bool is_xml_schema = false;
    if (dt.size() > xml_schema_len) {
        is_xml_schema = std::memcmp(dt.c_str(), xml_schema, xml_schema_len) == 0;
    }

    if (!is_xml_schema) {
        return pack_string_datatype(dt, str);
    }

    auto xsd_suffix = dt.substr(xml_schema_len);

    if (xsd_suffix == "date") {
        uint64_t object_id = DateTime::from_date(str);
        if (object_id == ObjectId::NULL_ID) {
            return pack_string_datatype(dt, str);
        }
        return pack_date(DateTime(object_id));

    } else if (xsd_suffix == "time") {
        uint64_t object_id = DateTime::from_time(str);
        if (object_id == ObjectId::NULL_ID) {
            return pack_string_datatype(dt, str);
        }
        return pack_date(DateTime(object_id));

    } else if (xsd_suffix == "dateTime") {
        uint64_t object_id = DateTime::from_dateTime(str);
        if (object_id == ObjectId::NULL_ID) {
            return pack_string_datatype(dt, str);
        }
        return pack_date(DateTime(object_id));

    } else if (xsd_suffix == "dateTimeStamp") {
        uint64_t object_id = DateTime::from_dateTimeStamp(str);
        if (object_id == ObjectId::NULL_ID) {
            return pack_string_datatype(dt, str);
        }
        return pack_date(object_id);

    } else if (xsd_suffix == "string") {
        return pack_string_xsd(str);

    } else if (xsd_suffix == "decimal") {
        bool    error;
        Decimal dec(str, &error);
        if (error) {
            return pack_string_datatype(dt, str);
        }
        return pack_decimal(dec);

    } else if (xsd_suffix == "float") {
        try {
            return pack_float(std::stof(str));
        }
        catch (const std::out_of_range& e) {
            return pack_string_datatype(dt, str);
        }
        catch (const std::invalid_argument& e) {
            return pack_string_datatype(dt, str);
        }

    } else if (xsd_suffix == "double") {
        try {
            return pack_double(std::stod(str));
        }
        catch (const std::out_of_range& e) {
            return pack_string_datatype(dt, str);
        }
        catch (const std::invalid_argument& e) {
            return pack_string_datatype(dt, str);
        }
    }
    // Signed integer
    else if (xsd_suffix == "integer" || xsd_suffix == "long" || xsd_suffix == "int" || xsd_suffix == "short"
             || xsd_suffix == "byte")
    {
        return try_pack_integer(dt, str);

    }
    // Negative integer
    else if (xsd_suffix == "nonPositiveInteger" || xsd_suffix == "negativeInteger") {
        return try_pack_integer(dt, str);
    }
    // Positive integer
    else if (xsd_suffix == "positiveInteger" || xsd_suffix == "nonNegativeInteger" || xsd_suffix == "unsignedLong"
               || xsd_suffix == "unsignedInt" || xsd_suffix == "unsignedShort" || xsd_suffix == "unsignedByte")
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


ObjectId Conversions::pack_string_datatype(const std::string& dt, const std::string& str) {
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
        id = Inliner::inline_string5(str_ptr->c_str()) | ObjectId::MASK_STRING_DATATYPE_INLINED;
    } else {
        auto str_id = string_manager.get_str_id(*str_ptr);
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            id = ObjectId::MASK_STRING_DATATYPE_EXTERN | str_id;
        } else {
            id = ObjectId::MASK_STRING_DATATYPE_TMP | tmp_manager.get_str_id(*str_ptr);
        }
    }
    return ObjectId(id | datatype_id);
}


ObjectId Conversions::try_pack_integer(const std::string& dt, const std::string& str) {
    try {
        size_t  pos;
        int64_t n = std::stoll(str, &pos);
        // Check if the whole string was parsed
        if (pos != str.size())
            return pack_string_datatype(dt, str);
        return pack_int(n);
    }
    catch (std::out_of_range& e) {
        // The integer is too big, we use a Decimal
        bool    error;
        Decimal dec(str, &error);
        if (error) {
            return pack_string_datatype(dt, str);
        }
        return pack_decimal(dec);
    }
    catch (std::invalid_argument& e) {
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
ObjectId Conversions::to_boolean(ObjectId oid) {
    uint64_t value = oid.get_value();

    switch (oid.get_sub_type()) {
    case ObjectId::MASK_BOOL:
        return oid;
    // String
    // Note: Extern strings will never be empty
    case ObjectId::MASK_STRING_SIMPLE_INLINED:
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(value != 0));
    case ObjectId::MASK_STRING_SIMPLE_EXTERN:
        return ObjectId(ObjectId::BOOL_TRUE);
    // Integer
    case ObjectId::MASK_NEGATIVE_INT:
    case ObjectId::MASK_POSITIVE_INT:
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(value != 0));
    // Float
    case ObjectId::MASK_FLOAT: {
        auto f = unpack_float(oid);
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(f != 0 && !std::isnan(f)));
    }
    // Double
    case ObjectId::MASK_DOUBLE: {
        auto d = unpack_double(oid);
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(d != 0 && !std::isnan(d)));
    }
    // Decimal
    // Note: This assumes 0 is never represented as 0.0, 0.00, etc
    case ObjectId::MASK_DECIMAL_INLINED:
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(value != 0));
    // Note: Extern decimals will never be zero
    case ObjectId::MASK_DECIMAL_EXTERN:
        return ObjectId(ObjectId::BOOL_TRUE);
    // Can not be converted to boolean
    default:
        return ObjectId::get_null();
    }
}

/*
Steps to evaluate an expression:
    - Calculate the datatype the operation should use (calculate_optype)
    - unpack operands (unpack_x)
    - convert operands to previously calculated datatype
    - evaluate operation
    - pack result (pack_x)

Type promotion and type substitution order:
    integer -> decimal -> float (-> double)

Conversion:
    int64_t -> Decimal (Decimal constructor)
    int64_t -> float (cast)
    Decimal -> float (Decimal member function)
*/


Decimal Conversions::unpack_decimal_inlined(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_DECIMAL_INLINED);

    auto sign     = (oid.id & Conversions::DECIMAL_SIGN_MASK) != 0;
    auto number   = (oid.id & Conversions::DECIMAL_NUMBER_MASK) >> 4;
    auto decimals =  oid.id & Conversions::DECIMAL_SEPARATOR_MASK;

    std::string dec_str;

    if (sign) {
        dec_str += '-';
    } else {
        dec_str += '+';
    }

    auto num_str      = std::to_string(number);
    auto num_str_size = num_str.size();
    if (decimals > 0) {
        if (decimals >= num_str_size) {
            dec_str += "0.";
            if (decimals > num_str_size) {
                dec_str += std::string(decimals - num_str_size, '0');
            }
            dec_str += num_str;
        } else {
            dec_str += num_str;
            dec_str.insert(dec_str.length() - decimals, ".");
        }
    } else {
        dec_str += num_str;
        dec_str += (".0");
    }

    // inefficient, str is already normalized, but the constructor of Decimal normalizes again.
    bool error;
    Decimal dec(dec_str, &error);
    assert(!error);
    return dec;
}


Decimal Conversions::unpack_decimal(ObjectId oid) {
    switch (oid.get_type()) {
    case ObjectId::MASK_DECIMAL_INLINED:
        return unpack_decimal_inlined(oid);
    case ObjectId::MASK_DECIMAL_EXTERN: {
        auto ss = std::stringstream();
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(ss, external_id);
        return Decimal::from_external(ss.str());
    }

    case ObjectId::MASK_DECIMAL_TMP: {
        auto ss = std::stringstream();
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(ss, external_id);
        return Decimal::from_external(ss.str());
    }
    default:
        throw LogicException("Called unpack_decimal with incorrect ObjectId type, this should never happen");
    }
}


ObjectId Conversions::pack_decimal(Decimal dec) {
    auto oid = dec.to_internal();

    // dec.to_internal fails if it can't be inlined
    if (oid == ObjectId::NULL_ID) {
        auto bytes_vec = dec.to_bytes();
        auto bytes = reinterpret_cast<const char*>(bytes_vec.data());
        auto bytes_id = string_manager.get_bytes_id(bytes, bytes_vec.size());
        if (bytes_id != ObjectId::MASK_NOT_FOUND) {
            oid = ObjectId::MASK_DECIMAL_EXTERN | bytes_id;
        } else {
            oid = ObjectId::MASK_DECIMAL_TMP | tmp_manager.get_bytes_id(bytes, bytes_vec.size());
        }
    }
    return ObjectId(oid);
}


ObjectId Conversions::pack_double(double dbl) {
    uint64_t oid;
    auto bytes = reinterpret_cast<const char*>(reinterpret_cast<const char*>(&dbl));
    auto bytes_id = string_manager.get_bytes_id(bytes, sizeof(double));
    if (bytes_id != ObjectId::MASK_NOT_FOUND) {
        oid = ObjectId::MASK_DOUBLE_EXTERN | bytes_id;
    } else {
        oid = ObjectId::MASK_DOUBLE_TMP | tmp_manager.get_bytes_id(bytes, sizeof(double));
    }
    return ObjectId(oid);
}


/**
 *  @brief Calculates the datatype that should be used for expression evaluation.
 *  @param oid1 ObjectId of the first operand.
 *  @param oid2 ObjectId of the second operand.
 *  @return datatype that should be used or OPTYPE_INVALID if not both operands are numeric.
 */
uint8_t Conversions::calculate_optype(ObjectId oid1, ObjectId oid2) {
    auto optype1 = calculate_optype(oid1);
    auto optype2 = calculate_optype(oid2);
    return std::max(optype1, optype2);
}

/**
 *  @brief Calculates the generic datatypes of the operand in an expression.
 *  @param oid ObjectId of the operand involved in an expression.
 *  @return generic numeric datatype of the operand or OPTYPE_INVALID if oid is not numeric
 */
uint8_t Conversions::calculate_optype(ObjectId oid) {
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_INT:     return OPTYPE_INTEGER;
    case ObjectId::MASK_DECIMAL: return OPTYPE_DECIMAL;
    case ObjectId::MASK_FLOAT:   return OPTYPE_FLOAT;
    case ObjectId::MASK_DOUBLE:  return OPTYPE_DOUBLE;
    default: return OPTYPE_INVALID;
    }
}

/**
 *  @brief Converts an ObjectId to int64_t if permitted.
 *  @param oid ObjectId to convert.
 *  @return an int64_t representing the ObjectId.
 *  @throws LogicException if the ObjectId has no permitted type.
 */
int64_t Conversions::to_integer(ObjectId oid) {
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_INT:
        return unpack_int(oid);
    default:
        throw LogicException("Called to_integer with incorrect ObjectId type, this should never happen");
    }
}

/**
 *  @brief Converts an ObjectId to Decimal if permitted.
 *  @param oid ObjectId to convert.
 *  @return a Decimal representing the ObjectId.
 *  @throws LogicException if the ObjectId has no permitted type.
 */
Decimal Conversions::to_decimal(ObjectId oid) {
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_INT:
        return Decimal(unpack_int(oid));
    case ObjectId::MASK_DECIMAL:
        return unpack_decimal(oid);
    default:
        throw LogicException("Called to_decimal with incorrect ObjectId type, this should never happen");
    }
}

/**
 *  @brief Converts an ObjectId to float if permitted.
 *  @param oid ObjectId to convert.
 *  @return a float representing the ObjectId.
 *  @throws LogicException if the ObjectId has no permitted type.
 */
float Conversions::to_float(ObjectId oid) {
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_INT:
        return unpack_int(oid);
    case ObjectId::MASK_DECIMAL:
        return unpack_decimal(oid).to_float();
    case ObjectId::MASK_FLOAT:
        return unpack_float(oid);
    case ObjectId::MASK_DOUBLE:
        return unpack_double(oid);
    default:
        throw LogicException("Called to_float with incorrect ObjectId type, this should never happen");
    }
}

/**
 *  @brief Converts an ObjectId to double if permitted.
 *  @param oid ObjectId to convert.
 *  @return a double representing the ObjectId.
 *  @throws LogicException if the ObjectId has no permitted type.
 */
double Conversions::to_double(ObjectId oid) {
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_INT:
        return unpack_int(oid);
    case ObjectId::MASK_DECIMAL:
        return unpack_decimal(oid).to_double();
    case ObjectId::MASK_FLOAT:
        return unpack_float(oid);
    case ObjectId::MASK_DOUBLE:
        return unpack_double(oid);
    default:
        throw LogicException("Called to_double with incorrect ObjectId type, this should never happen");
    }
}


void Conversions::print_path_node(std::ostream& os, ObjectId node_id) {
    Conversions::debug_print(os, node_id);
}


void Conversions::print_path_edge(std::ostream& os, ObjectId edge_id, bool inverse) {
    os << ' ';
    if (inverse) {
        os << '^';
    }
    Conversions::debug_print(os, edge_id); // No need to escape os, as only IRIs are possible edges
    os << ' ';
}


// Converts an ObjectId into its lexical representation.
std::string Conversions::to_lexical_str(ObjectId oid) {
    switch (RDF_OID::get_type(oid)) {
    case RDF_OID::Type::BLANK_INLINED: {
        return "_:b" + std::to_string(unpack_blank(oid));
    }
    case RDF_OID::Type::BLANK_TMP: {
        return "_:c" + std::to_string(unpack_blank(oid));
    }
    case RDF_OID::Type::STRING_SIMPLE_INLINE:
    case RDF_OID::Type::STRING_SIMPLE_EXTERN:
    case RDF_OID::Type::STRING_SIMPLE_TMP:
    case RDF_OID::Type::STRING_XSD_INLINE:
    case RDF_OID::Type::STRING_XSD_EXTERN:
    case RDF_OID::Type::STRING_XSD_TMP:{
        return unpack_string(oid);
    }
    case RDF_OID::Type::INT56_INLINE:
    case RDF_OID::Type::INT64_EXTERN:
    case RDF_OID::Type::INT64_TMP: {
        int64_t i = unpack_int(oid);
        return std::to_string(i);
    }
    case RDF_OID::Type::FLOAT32: {
        float f = unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        return std::string(float_buffer);
    }
    case RDF_OID::Type::DOUBLE64_EXTERN:
    case RDF_OID::Type::DOUBLE64_TMP: {
        double d = unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        return std::string(double_buffer);
    }
    case RDF_OID::Type::BOOL: {
        return (unpack_bool(oid) ? "true" : "false");
    }
    case RDF_OID::Type::PATH: {
        std::stringstream ss;
        ss << '[';
        path_manager.print(
            ss,
            Conversions::get_path_id(oid),
            &print_path_node,
            &print_path_edge
        );
        ss << ']';
        return ss.str();
    }
    case RDF_OID::Type::IRI_INLINE:
    case RDF_OID::Type::IRI_INLINE_INT_SUFFIX:
    case RDF_OID::Type::IRI_EXTERN:
    case RDF_OID::Type::IRI_TMP:
    case RDF_OID::Type::IRI_UUID_LOWER:
    case RDF_OID::Type::IRI_UUID_UPPER:
    case RDF_OID::Type::IRI_UUID_LOWER_TMP:
    case RDF_OID::Type::IRI_UUID_UPPER_TMP:
    case RDF_OID::Type::IRI_HEX_LOWER:
    case RDF_OID::Type::IRI_HEX_LOWER_TMP:
    case RDF_OID::Type::IRI_HEX_UPPER:
    case RDF_OID::Type::IRI_HEX_UPPER_TMP: {
        return unpack_iri(oid);
    }
    case RDF_OID::Type::STRING_DATATYPE_INLINE:
    case RDF_OID::Type::STRING_DATATYPE_EXTERN:
    case RDF_OID::Type::STRING_DATATYPE_TMP: {
        auto&& [datatype, str] = unpack_string_datatype(oid);
        return str;
    }
    case RDF_OID::Type::STRING_LANG_INLINE:
    case RDF_OID::Type::STRING_LANG_EXTERN:
    case RDF_OID::Type::STRING_LANG_TMP: {
        auto&& [lang, str] = unpack_string_lang(oid);
        return str;
    }
    case RDF_OID::Type::DATE:
    case RDF_OID::Type::DATETIME:
    case RDF_OID::Type::TIME:
    case RDF_OID::Type::DATETIMESTAMP: {
        DateTime datetime = unpack_date(oid);
        return datetime.get_value_string();
    }
    case RDF_OID::Type::DECIMAL_INLINE:
    case RDF_OID::Type::DECIMAL_EXTERN:
    case RDF_OID::Type::DECIMAL_TMP: {
        Decimal decimal = unpack_decimal(oid);
        return decimal.to_string();
    }
    case RDF_OID::Type::NULL_ID: {
        return "";
    }
    }
    return "";
}


std::ostream& Conversions::debug_print(std::ostream& os, ObjectId oid) {
    switch (RDF_OID::get_type(oid)) {
    case RDF_OID::Type::BLANK_INLINED: {
        os << "_:b";
        os << unpack_blank(oid);
        break;
    }
    case RDF_OID::Type::BLANK_TMP: {
        os << "_:c";
        os << unpack_blank(oid);
        break;
    }
    case RDF_OID::Type::STRING_SIMPLE_INLINE:
    case RDF_OID::Type::STRING_SIMPLE_EXTERN:
    case RDF_OID::Type::STRING_SIMPLE_TMP: {
        os << '"';
        print_string(oid, os);
        os << '"';
        break;
    }
    case RDF_OID::Type::STRING_XSD_INLINE:
    case RDF_OID::Type::STRING_XSD_EXTERN:
    case RDF_OID::Type::STRING_XSD_TMP:{
        os << '"';
        print_string(oid, os);
        os << "\"^^<http://www.w3.org/2001/XMLSchema#string>";
        break;
    }
    case RDF_OID::Type::INT56_INLINE:
    case RDF_OID::Type::INT64_EXTERN:
    case RDF_OID::Type::INT64_TMP: {
        os << Conversions::unpack_int(oid);
        break;
    }
    case RDF_OID::Type::FLOAT32: {
        float f = Conversions::unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        os << float_buffer;
        break;
    }
    case RDF_OID::Type::DOUBLE64_EXTERN:
    case RDF_OID::Type::DOUBLE64_TMP: {
        double d = Conversions::unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        os << double_buffer;
        break;
    }
    case RDF_OID::Type::BOOL: {
        os << (Conversions::unpack_bool(oid) ? "true" : "false")
           << "^^<http://www.w3.org/2001/XMLSchema#boolean>";
        break;
    }
    case RDF_OID::Type::PATH: {
        using namespace std::placeholders;
        os << '[';
        path_manager.print(
            os,
            Conversions::get_path_id(oid),
            std::bind(&print_path_node, _1, _2),
            std::bind(&print_path_edge, _1, _2, _3)
        );
        os << ']';
        break;
    }
    case RDF_OID::Type::IRI_INLINE:
    case RDF_OID::Type::IRI_INLINE_INT_SUFFIX:
    case RDF_OID::Type::IRI_EXTERN:
    case RDF_OID::Type::IRI_TMP:
    case RDF_OID::Type::IRI_UUID_LOWER:
    case RDF_OID::Type::IRI_UUID_LOWER_TMP:
    case RDF_OID::Type::IRI_UUID_UPPER:
    case RDF_OID::Type::IRI_UUID_UPPER_TMP:
    case RDF_OID::Type::IRI_HEX_LOWER:
    case RDF_OID::Type::IRI_HEX_LOWER_TMP:
    case RDF_OID::Type::IRI_HEX_UPPER:
    case RDF_OID::Type::IRI_HEX_UPPER_TMP: {
        os << '<';
        Conversions::print_iri(oid, os);
        os << '>';
        break;
    }
    case RDF_OID::Type::STRING_DATATYPE_INLINE:
    case RDF_OID::Type::STRING_DATATYPE_EXTERN:
    case RDF_OID::Type::STRING_DATATYPE_TMP: {
        auto&& [datatype, str] = Conversions::unpack_string_datatype(oid);
        os << '"';
        os << str;
        os << "\"^^<";
        os << datatype;
        os << ">";
        break;
    }
    case RDF_OID::Type::STRING_LANG_INLINE:
    case RDF_OID::Type::STRING_LANG_EXTERN:
    case RDF_OID::Type::STRING_LANG_TMP: {
        auto&& [lang, str] = Conversions::unpack_string_lang(oid);
        os << '"';
        os << str;
        os << "\"@";
        os << lang;
        break;
    }
    case RDF_OID::Type::DATE:
    case RDF_OID::Type::DATETIME:
    case RDF_OID::Type::TIME:
    case RDF_OID::Type::DATETIMESTAMP: {
        DateTime datetime = Conversions::unpack_date(oid);

        os << '"' << datetime.get_value_string();
        os << "\"^^<" << datetime.get_datatype_string() << ">";
        break;
    }
    case RDF_OID::Type::DECIMAL_INLINE:
    case RDF_OID::Type::DECIMAL_EXTERN:
    case RDF_OID::Type::DECIMAL_TMP: {
        auto decimal = Conversions::unpack_decimal(oid);
        os << decimal;
        break;
    }
    case RDF_OID::Type::NULL_ID: {
        os << "NULL";
        break;
    }
    }
    return os;
}


ObjectId Conversions::pack_string_simple(const std::string& str) {
    uint64_t oid;
    if (str.size() == 0) {
        return ObjectId(ObjectId::MASK_STRING_SIMPLE_INLINED);
    } else if (str.size() <= ObjectId::STR_INLINE_BYTES) {
        oid = Inliner::inline_string(str.c_str()) | ObjectId::MASK_STRING_SIMPLE_INLINED;
    } else {
        auto str_id = string_manager.get_str_id(str);
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            oid = ObjectId::MASK_STRING_SIMPLE_EXTERN | str_id;
        } else {
            oid = ObjectId::MASK_STRING_SIMPLE_TMP | tmp_manager.get_str_id(str);
        }
    }
    return ObjectId(oid);
}


ObjectId Conversions::pack_string_xsd(const std::string& str) {
    uint64_t oid;
    if (str.size() == 0) {
        return ObjectId(ObjectId::MASK_STRING_XSD_INLINED);
    } else if (str.size() <= ObjectId::STR_INLINE_BYTES) {
        oid = Inliner::inline_string(str.c_str()) | ObjectId::MASK_STRING_XSD_INLINED;
    } else {
        auto str_id = string_manager.get_str_id(str);
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            oid = ObjectId::MASK_STRING_XSD_EXTERN | str_id;
        } else {
            oid = ObjectId::MASK_STRING_XSD_TMP | tmp_manager.get_str_id(str);
        }
    }
    return ObjectId(oid);
}


ObjectId Conversions::pack_iri(const std::string& str) {
    // If a prefix matches the IRI, store just the suffix and a pointer to the prefix
    auto [prefix_id, prefix_size] = rdf_model.catalog.prefixes.get_prefix_id(str);

    auto suffix = str.substr(prefix_size);
    // Pointer to the iri, it is updated if the iri is compressed
    const char* suffix_ptr = suffix.c_str();
    size_t suffix_len = suffix.size();

    auto prefix_id_shifted = static_cast<uint64_t>(prefix_id) << ObjectId::IRI_INLINE_BYTES*8;

    uint64_t suffix_id;

    char* buffer_iri = get_query_ctx().get_buffer1();

    uint64_t iri_type_mask = ObjectId::MASK_IRI;

    if (UUIDCompression::compress_lower(suffix_ptr, suffix_len, buffer_iri)) {
        suffix_ptr = buffer_iri;
        suffix_len = suffix_len - 20;
        iri_type_mask = ObjectId::MASK_IRI_UUID_LOWER;

    } else if (UUIDCompression::compress_upper(suffix_ptr, suffix_len, buffer_iri)) {
        suffix_ptr = buffer_iri;
        suffix_len = suffix_len - 20;
        iri_type_mask = ObjectId::MASK_IRI_UUID_UPPER;
    }

    else if (suffix_len >= HexCompression::MIN_LEN_TO_COMPRESS) {
        auto lower_hex_length = HexCompression::get_lower_hex_length(suffix_ptr, suffix_len);
        auto upper_hex_length = HexCompression::get_upper_hex_length(suffix_ptr, suffix_len);

        // Compress lowercase hex characters
        if (lower_hex_length >= upper_hex_length && lower_hex_length > HexCompression::MIN_HEX_LEN_TO_COMPRESS) {
            suffix_len = HexCompression::compress(suffix_ptr, suffix_len, lower_hex_length, buffer_iri);
            suffix_ptr = buffer_iri;
            iri_type_mask = ObjectId::MASK_IRI_HEX_LOWER;

        // Compress uppercase hex characters
        } else if (upper_hex_length > HexCompression::MIN_HEX_LEN_TO_COMPRESS) {
            suffix_len = HexCompression::compress(suffix_ptr, suffix_len, upper_hex_length, buffer_iri);
            suffix_ptr = buffer_iri;
            iri_type_mask = ObjectId::MASK_IRI_HEX_UPPER;
        }
    }

    if (suffix_len <= ObjectId::IRI_INLINE_BYTES) {
        suffix_id = Inliner::inline_iri(suffix_ptr) | ObjectId::MASK_IRI_INLINED;
    } else {
        auto str_id = string_manager.get_str_id(std::string(suffix_ptr, suffix_len));
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            suffix_id = iri_type_mask | ObjectId::MOD_EXTERNAL | str_id;
        } else {
            iri_type_mask &= ~ObjectId::MOD_MASK;
            suffix_id = iri_type_mask | ObjectId::MOD_TMP | tmp_manager.get_str_id(std::string(suffix_ptr, suffix_len));
        }
    }

    return ObjectId(suffix_id | prefix_id_shifted);
}


void Conversions::print_iri(ObjectId oid, std::ostream& os) {
    auto prefix_id = oid.get_value() >> (ObjectId::IRI_INLINE_BYTES * 8);
    auto& prefix = rdf_model.catalog.prefixes.get_prefix(prefix_id);
    os << prefix;

    switch (oid.get_type()) {
    case ObjectId::MASK_IRI_INLINED: {
        Inliner::print_string_inlined<6>(os, oid.id);
        break;
    }
    case ObjectId::MASK_IRI_EXTERN: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(os, external_id);
        break;
    }
    case ObjectId::MASK_IRI_TMP: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(os, external_id);
        break;
    }
    case ObjectId::MASK_IRI_UUID_LOWER_TMP:{
        print_tmp_iri_uuid_lower(oid, os);
        break;
    }
    case ObjectId::MASK_IRI_UUID_LOWER: {
        print_iri_uuid_lower(oid, os);
        break;
    }
    case ObjectId::MASK_IRI_UUID_UPPER_TMP: {
        print_tmp_iri_uuid_upper(oid, os);
        break;
    }
    case ObjectId::MASK_IRI_UUID_UPPER: {
        print_iri_uuid_upper(oid, os);
        break;
    }
    case ObjectId::MASK_IRI_HEX_LOWER_TMP: {
        print_tmp_iri_hex_lower(oid, os);
        break;
    }
    case ObjectId::MASK_IRI_HEX_LOWER: {
        print_iri_hex_lower(oid, os);
        break;
    }
    case ObjectId::MASK_IRI_HEX_UPPER_TMP: {
        print_tmp_iri_hex_upper(oid, os);
        break;
    }
    case ObjectId::MASK_IRI_HEX_UPPER: {
        print_iri_hex_upper(oid, os);
        break;
    }
    default:
        throw LogicException("Called unpack_iri with incorrect ObjectId type, this should never happen");
    }
}


void Conversions::print_iri_uuid_lower(ObjectId oid, std::ostream& os) {
    char* iri_buffer    = get_query_ctx().get_buffer1();
    char* result_buffer = get_query_ctx().get_buffer2();

    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    uint64_t size        = string_manager.print_to_buffer(iri_buffer, external_id);
    UUIDCompression::decompress_lower(iri_buffer, size, result_buffer);

    os.write(result_buffer, size + 20);
}


void Conversions::print_iri_uuid_upper(ObjectId oid, std::ostream& os) {
    char* iri_buffer    = get_query_ctx().get_buffer1();
    char* result_buffer = get_query_ctx().get_buffer2();

    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    uint64_t size        = string_manager.print_to_buffer(iri_buffer, external_id);
    UUIDCompression::decompress_upper(iri_buffer, size, result_buffer);

    os.write(result_buffer, size + 20);
}


void Conversions::print_tmp_iri_uuid_lower(ObjectId oid, std::ostream& os) {
    char* iri_buffer    = get_query_ctx().get_buffer1();
    char* result_buffer = get_query_ctx().get_buffer2();

    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    uint64_t size        = tmp_manager.print_to_buffer(iri_buffer, external_id);
    UUIDCompression::decompress_lower(iri_buffer, size, result_buffer);

    os.write(result_buffer, size + 20);
}


void Conversions::print_tmp_iri_uuid_upper(ObjectId oid, std::ostream& os) {
    char* iri_buffer    = get_query_ctx().get_buffer1();
    char* result_buffer = get_query_ctx().get_buffer2();

    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    uint64_t size        = tmp_manager.print_to_buffer(iri_buffer, external_id);
    UUIDCompression::decompress_upper(iri_buffer, size, result_buffer);

    os.write(result_buffer, size + 20);
}


void Conversions::print_iri_hex_lower(ObjectId oid, std::ostream& os) {
    char* buffer        = get_query_ctx().get_buffer1();
    char* result_buffer = get_query_ctx().get_buffer2();

    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    auto size = string_manager.print_to_buffer(buffer, external_id);
    auto decompressed_size = HexCompression::decompress_lower(buffer, size, result_buffer);

    os.write(result_buffer, decompressed_size);
}


void Conversions::print_tmp_iri_hex_lower(ObjectId oid, std::ostream& os) {
    char* buffer        = get_query_ctx().get_buffer1();
    char* result_buffer = get_query_ctx().get_buffer2();

    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    auto size = tmp_manager.print_to_buffer(buffer, external_id);
    auto decompressed_size = HexCompression::decompress_lower(buffer, size, result_buffer);

    os.write(result_buffer, decompressed_size);
}


void Conversions::print_iri_hex_upper(ObjectId oid, std::ostream& os) {
    char* buffer        = get_query_ctx().get_buffer1();
    char* result_buffer = get_query_ctx().get_buffer2();

    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    auto size = string_manager.print_to_buffer(buffer, external_id);
    auto decompressed_size = HexCompression::decompress_upper(buffer, size, result_buffer);

    os.write(result_buffer, decompressed_size);
}


void Conversions::print_tmp_iri_hex_upper(ObjectId oid, std::ostream& os) {
    char* buffer        = get_query_ctx().get_buffer1();
    char* result_buffer = get_query_ctx().get_buffer2();

    uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
    auto size = tmp_manager.print_to_buffer(buffer, external_id);
    auto decompressed_size = HexCompression::decompress_upper(buffer, size, result_buffer);

    os.write(result_buffer, decompressed_size);
}


std::string Conversions::unpack_iri(ObjectId oid) {
    auto prefix_id = oid.get_value() >> (ObjectId::IRI_INLINE_BYTES * 8);
    auto prefix = rdf_model.catalog.prefixes.get_prefix(prefix_id);

    switch (oid.get_type()) {
    case ObjectId::MASK_IRI_INLINED: {
        return prefix + Inliner::get_string_inlined<6>(oid.id);
    }
    case ObjectId::MASK_IRI_EXTERN: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(ss, external_id);

        prefix.append(ss.str());
        return prefix;
    }
    case ObjectId::MASK_IRI_UUID_LOWER: {
        std::stringstream ss;
        print_iri_uuid_lower(oid, ss);
        prefix.append(ss.str());
        return prefix;
    }
    case ObjectId::MASK_IRI_UUID_UPPER: {
        std::stringstream ss;
        print_iri_uuid_upper(oid, ss);
        prefix.append(ss.str());
        return prefix;
    }
    case ObjectId::MASK_IRI_UUID_LOWER_TMP: {
        std::stringstream ss;
        print_tmp_iri_uuid_lower(oid, ss);
        prefix.append(ss.str());
        return prefix;
    }
    case ObjectId::MASK_IRI_UUID_UPPER_TMP: {
        std::stringstream ss;
        print_tmp_iri_uuid_upper(oid, ss);
        prefix.append(ss.str());
        return prefix;
    }
    case ObjectId::MASK_IRI_HEX_LOWER_TMP:{
        std::stringstream ss;
        print_tmp_iri_hex_lower(oid, ss);
        prefix.append(ss.str());
        return prefix;
    }
    case ObjectId::MASK_IRI_HEX_UPPER_TMP: {
        std::stringstream ss;
        print_tmp_iri_hex_upper(oid, ss);
        prefix.append(ss.str());
        return prefix;
    }
    case ObjectId::MASK_IRI_TMP: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(ss, external_id);

        prefix.append(ss.str());
        return prefix;
    }
    case ObjectId::MASK_IRI_HEX_LOWER: {
        std::stringstream ss;
        print_iri_hex_lower(oid, ss);
        prefix.append(ss.str());
        return prefix;
    }
    case ObjectId::MASK_IRI_HEX_UPPER: {
        std::stringstream ss;
        print_iri_hex_upper(oid, ss);
        prefix.append(ss.str());
        return prefix;
    }
    default:
        throw LogicException("Called unpack_iri with incorrect ObjectId type, this should never happen");
    }
}


void Conversions::print_string(ObjectId oid, std::ostream& os) {
    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_SIMPLE_INLINED:
    case ObjectId::MASK_STRING_XSD_INLINED: {
        Inliner::print_string_inlined<7>(os, oid.id);
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_EXTERN:
    case ObjectId::MASK_STRING_XSD_EXTERN: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(os, external_id);
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_TMP:
    case ObjectId::MASK_STRING_XSD_TMP: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(os, external_id);
        break;
    }
    default:
        throw LogicException("Called unpack_string with incorrect ObjectId type, this should never happen");
    }
}


std::string Conversions::unpack_string(ObjectId oid) {
    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_SIMPLE_INLINED:
    case ObjectId::MASK_STRING_XSD_INLINED: {
        return Inliner::get_string_inlined<ObjectId::STR_INLINE_BYTES>(oid.id);
    }
    case ObjectId::MASK_STRING_SIMPLE_EXTERN:
    case ObjectId::MASK_STRING_XSD_EXTERN: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(ss, external_id);
        return ss.str();
    }
    case ObjectId::MASK_STRING_SIMPLE_TMP:
    case ObjectId::MASK_STRING_XSD_TMP: {
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
std::pair<std::string, std::string> Conversions::unpack_string_lang(ObjectId oid) {
    std::string str;
    std::string lang;

    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_LANG_INLINED: {
        str = Inliner::get_string_inlined<5>(oid.id);
        break;
    }
    case ObjectId::MASK_STRING_LANG_EXTERN: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(ss, external_id);

        str =  ss.str();
        break;
    }
    case ObjectId::MASK_STRING_LANG_TMP: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(ss, external_id);

        str =  ss.str();
        break;
    }
    default:
        throw LogicException("Called unpack_string_lang with incorrect ObjectId type, this should never happen");
    }

    auto lang_id = oid.get_value() >> TMP_SHIFT;
    if (lang_id == (LAST_TMP_ID >> TMP_SHIFT)) {
        auto found = str.find_last_of('@');
        if (found == std::string::npos) {
            throw LogicException("string with lang:LAST_TMP_ID `" + str + "` must have @ as separator");
        }
        lang = str.substr(found+1);
        str = str.substr(0,found);
    } else {
        lang = rdf_model.catalog.languages[lang_id];
    }
    return std::make_pair(std::move(lang), std::move(str));
}


// Doesn't print the language
void Conversions::print_string_lang(ObjectId oid, std::ostream& os) {
    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_LANG_INLINED: {
        Inliner::print_string_inlined<5>(os, oid.id);
        break;
    }
    case ObjectId::MASK_STRING_LANG_EXTERN: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(os, external_id);
        break;
    }
    case ObjectId::MASK_STRING_LANG_TMP: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(os, external_id);
        break;
    }
    default:
        throw LogicException("Called unpack_string_lang with incorrect ObjectId type, this should never happen");
    }
}


// returns <datatype, str>
std::pair<std::string, std::string> Conversions::unpack_string_datatype(ObjectId oid) {
    std::string str;
    std::string datatype;

    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_DATATYPE_INLINED: {
        str = Inliner::get_string_inlined<5>(oid.id);
        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_EXTERN: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(ss, external_id);

        str =  ss.str();
        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_TMP: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(ss, external_id);

        str =  ss.str();
        break;
    }
    default:
        throw LogicException("Called unpack_string_data with incorrect ObjectId type, this should never happen");
    }

    auto datatype_id = oid.get_value() >> TMP_SHIFT;
    if (datatype_id == (LAST_TMP_ID >> TMP_SHIFT)) {
        auto found = str.find_last_of('^');
        if (found == std::string::npos) {
            throw LogicException("string with datatype:LAST_TMP_ID `" + str + "` must have ^ as separator");
        }
        datatype = str.substr(found+1);
        str = str.substr(0,found);
    } else {
        datatype = rdf_model.catalog.datatypes[datatype_id];
    }
    return std::make_pair(std::move(datatype), std::move(str));
}


// Doesn't print the datatype
void Conversions::print_string_datatype(ObjectId oid, std::ostream& os) {
    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_DATATYPE_INLINED: {
        Inliner::print_string_inlined<5>(os, oid.id);
        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_EXTERN: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(os, external_id);
        break;
    }
    case ObjectId::MASK_STRING_DATATYPE_TMP: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(os, external_id);
        break;
    }
    default:
        throw LogicException("Called unpack_string_data with incorrect ObjectId type, this should never happen");
    }
}
