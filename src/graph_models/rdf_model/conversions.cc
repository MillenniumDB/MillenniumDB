#include "conversions.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>

#include "graph_models/inliner.h"
#include "graph_models/object_id.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "graph_models/rdf_model/datatypes/decimal.h"
#include "graph_models/rdf_model/datatypes/decimal_inlined.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "storage/unified_storage.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

using namespace SPARQL;

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
        return ObjectId(ObjectId::MASK_BOOL | (value != 0));
    case ObjectId::MASK_STRING_SIMPLE_EXTERN:
        return ObjectId(ObjectId::BOOL_TRUE);
    // Integer
    case ObjectId::MASK_NEGATIVE_INT:
    case ObjectId::MASK_POSITIVE_INT:
        return ObjectId(ObjectId::MASK_BOOL | (value != 0));
    // Float
    case ObjectId::MASK_FLOAT: {
        auto f = unpack_float(oid);
        return ObjectId(ObjectId::MASK_BOOL | (f != 0 && !std::isnan(f)));
    }
    // Double
    case ObjectId::MASK_DOUBLE: {
        auto d = unpack_double(oid);
        return ObjectId(ObjectId::MASK_BOOL | (d != 0 && !std::isnan(d)));
    }
    // Decimal
    // Note: This assumes 0 is never represented as 0.0, 0.00, etc
    case ObjectId::MASK_DECIMAL_INLINED:
        return ObjectId(ObjectId::MASK_BOOL | (value != 0));
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

/**
 *  @brief Unpacks the positive_int inside an ObjectId.
 *  @param oid The ObjectId to unpack.
 *  @return The value inside the ObjectId.
 */
int64_t Conversions::unpack_positive_int(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_POSITIVE_INT);

    return static_cast<int64_t>(oid.get_value());
}


/**
 *  @brief Unpacks the negative_int inside an ObjectId.
 *  @param oid The ObjectId to unpack.
 *  @return The value inside the ObjectId.
 */
int64_t Conversions::unpack_negative_int(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_NEGATIVE_INT);

    return static_cast<int64_t>((~oid.id) & ObjectId::VALUE_MASK) * -1;
}

/**
 *  @brief Unpacks an int (positive or negative) inside an ObjectId.
 *  @param oid The ObjectId to unpack.
 *  @return The value inside the ObjectId.
 */
int64_t Conversions::unpack_int(ObjectId oid) {
    switch (oid.get_type()) {
    case ObjectId::MASK_NEGATIVE_INT:
        return unpack_negative_int(oid);
    case ObjectId::MASK_POSITIVE_INT:
        return unpack_positive_int(oid);
    default:
        throw LogicException("Called unpack_int with incorrect ObjectId type, this should never happen");
    }
}

/**
 *  @brief Unpacks the internal Decimal of an ObjectId.
 *  @param oid The ObjectId to unpack.
 *  @return The value inside the ObjectId.
 */
Decimal Conversions::unpack_decimal_inlined(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_DECIMAL_INLINED);

    auto sign     = (oid.id & DECIMAL_SIGN_MASK) != 0;
    auto number   = (oid.id & DECIMAL_NUMBER_MASK) >> 4;
    auto decimals = oid.id & DECIMAL_SEPARATOR_MASK;

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

    // TODO: inefficient, str is already normalized, but the constructor of Decimal normalizes again.
    bool error;
    Decimal dec(dec_str, &error);
    assert(!error);
    return dec;
}

/**
 *  @brief Unpacks the external Decimal of an ObjectId.
 *  @param oid The ObjectId to unpack.
 *  @return The value referenced by the ObjectId.
 */
Decimal Conversions::unpack_decimal_extern_tmp(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_DECIMAL_EXTERN || oid.get_type() == ObjectId::MASK_DECIMAL_TMP);

    auto ss = std::stringstream();
    UnifiedStorage::print_str(ss, oid);

    return Decimal::from_external(ss.str());
}


/**
 *  @brief Unpacks a Decimal(inlined, extern, or tmp).
 *  @param oid The ObjectId containing the Decimal.
 *  @return The Decimal inside ObjectId.
 */
Decimal Conversions::unpack_decimal(ObjectId oid) {
    switch (oid.get_type()) {
    case ObjectId::MASK_DECIMAL_INLINED:
        return unpack_decimal_inlined(oid);
    case ObjectId::MASK_DECIMAL_EXTERN:
    case ObjectId::MASK_DECIMAL_TMP:
        return unpack_decimal_extern_tmp(oid);
    default:
        throw LogicException("Called unpack_decimal with incorrect ObjectId type, this should never happen");
    }
}

/**
 *  @brief Unpacks the float inside an ObjectId.
 *  @param oid The ObjectId to unpack.
 *  @return The value contained in the ObjectId.
 */
float Conversions::unpack_float(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_FLOAT);

    auto  value = oid.id;
    float flt;
    auto  dst = reinterpret_cast<char*>(&flt);

    dst[0] = value & 0xFF;
    dst[1] = (value >> 8) & 0xFF;
    dst[2] = (value >> 16) & 0xFF;
    dst[3] = (value >> 24) & 0xFF;

    return flt;
}

/**
 *  @brief Unpacks the double inside an ObjectId.
 *  @param oid The ObjectId to unpack.
 *  @return The value contained in the ObjectId.
 */
double Conversions::unpack_double(ObjectId oid) {
    assert(oid.get_sub_type() == ObjectId::MASK_DOUBLE);

    std::stringstream ss;
    UnifiedStorage::print_str(ss, oid);
    double dbl;
    auto dst = reinterpret_cast<char*>(&dbl);
    ss.read(dst, 8);

    return dbl;
}

/**
 *  @brief Packs an int64_t into an ObjectId.
 *  @param dec The int64_t value that should be packed.
 *  @return An ObjectId (positive_integer or negative_integer) containing the value or null if it does not fit.
 */
ObjectId Conversions::pack_int(int64_t i) {
    uint64_t oid = ObjectId::MASK_POSITIVE_INT;

    if (i < 0) {
        oid = ObjectId::MASK_NEGATIVE_INT;
        i *= -1;
        if (i > INTEGER_MAX) {
            return ObjectId::get_null();
        }
        i = (~i) & ObjectId::VALUE_MASK;
    } else {
        if (i > INTEGER_MAX) {
            return ObjectId::get_null();
        }
    }

    return ObjectId(oid | i);
}

/**
 *  @brief Packs a normalized Decimal into an ObjectId.
 *  @param dec A normalized Decimal that should be packed.
 *  @return An ObjectId (inlined or extern) containing the value.
 */
ObjectId Conversions::pack_decimal(Decimal dec) {
    auto oid = dec.to_internal();
    if (oid == ObjectId::NULL_ID) {
        auto bytes = dec.to_bytes();
        oid = UnifiedStorage::get_bytes_id(reinterpret_cast<const char*>(bytes.data()),
                                           bytes.size(),
                                           ObjectId::MASK_DECIMAL);
    }
    return ObjectId(oid);
}

/**
 *  @brief Packs a float into an ObjectId.
 *  @param flt The float value that should be packed.
 *  @return An ObjectId containing the value.
 */
ObjectId Conversions::pack_float(float flt) {
    auto src = reinterpret_cast<unsigned char*>(&flt);

    auto oid = ObjectId::MASK_FLOAT;
    oid |= static_cast<uint64_t>(src[0]);
    oid |= static_cast<uint64_t>(src[1]) << 8;
    oid |= static_cast<uint64_t>(src[2]) << 16;
    oid |= static_cast<uint64_t>(src[3]) << 24;

    return ObjectId(oid);
}

/**
 *  @brief Packs a double into an ObjectId.
 *  @param dbl The double value that should be packed.
 *  @return An ObjectId containing the value.
 */
ObjectId Conversions::pack_double(double dbl) {
    auto oid = UnifiedStorage::get_bytes_id(reinterpret_cast<const char*>(&dbl),
                                            8,
                                            ObjectId::MASK_DOUBLE);
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

// Converts an ObjectId into it's lexical representation.
std::string Conversions::to_lexical_str(ObjectId oid) {
    switch (oid.get_sub_type()) {
    // case ObjectId::MASK_NULL:
    //     return "null";
    // case ObjectId::MASK_ANON: {
    //     char c = oid.get_mod() == ObjectId::MOD_INLINE ? 'b' : 'c';
    //     return "_:" + std::to_string(c) + std::to_string(oid.get_value());
    // }
    case ObjectId::MASK_IRI:
        return unpack_iri(oid);
    case ObjectId::MASK_STRING_SIMPLE:
        return unpack_string_simple(oid);
    case ObjectId::MASK_STRING_XSD:
        return unpack_string_xsd(oid);
    case ObjectId::MASK_STRING_LANG: {
        auto [lang, str] = unpack_string_lang(oid);
        return str;
    }
    case ObjectId::MASK_STRING_DATATYPE:{
        auto [datatype, str] = unpack_string_datatype(oid);
        return str;
    }
    case ObjectId::MASK_INT:
        return std::to_string(unpack_int(oid));
    case ObjectId::MASK_DECIMAL:
        return unpack_decimal(oid).to_string();
    case ObjectId::MASK_FLOAT:
        return std::to_string(unpack_float(oid));
    case ObjectId::MASK_DOUBLE:
        return std::to_string(unpack_double(oid));
    case ObjectId::MASK_DT_DATE:
    case ObjectId::MASK_DT_DATETIME:
    case ObjectId::MASK_DT_TIME:
    case ObjectId::MASK_DT_DATETIMESTAMP: {
        return DateTime(oid).get_value_string();
    }
    case ObjectId::MASK_BOOL: {
        if (oid.get_value() == 1) {
            return "true";
        } else {
            return "false";
        }
    }
    default:
        throw NotSupportedException("No lexical form defined for ObjectId of sub-type " + std::to_string(oid.get_sub_type()));
    }
}


/**
 * @brief Unpacks the inlined string inside an ObjectId.
 *
 * @param oid ObjectId to unpack.
 * @return std::string string inside the ObjectId.
 */
std::string Conversions::unpack_string_simple_inlined(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_STRING_SIMPLE_INLINED);

    char str[8];
    int  shift_size = 6 * 8;
    for (int i = 0; i < ObjectId::MAX_INLINED_BYTES; i++) {
        uint8_t byte = (oid.id >> shift_size) & 0xFF;
        str[i]       = byte;
        shift_size -= 8;
    }
    str[7] = '\0';

    return std::string(str);
}

/**
 * @brief Unpacks the external string inside an ObjectId.
 *
 * @param oid ObjectId to unpack.
 * @return std::string string inside the ObjectId.
 */
std::string Conversions::unpack_string_simple_extern_tmp(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_STRING_SIMPLE_EXTERN || oid.get_type() == ObjectId::MASK_STRING_SIMPLE_TMP);

    std::stringstream ss;
    UnifiedStorage::print_str(ss, oid);

    return ss.str();
}

/**
 * @brief Unpacks the inlined xsd:string inside an ObjectId.
 *
 * @param oid ObjectId to unpack.
 * @return std::string string inside the ObjectId.
 */
std::string Conversions::unpack_string_xsd_inlined(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_STRING_XSD_INLINED);

    char str[8];
    int  shift_size = 6 * 8;
    for (int i = 0; i < ObjectId::MAX_INLINED_BYTES; i++) {
        uint8_t byte = (oid.id >> shift_size) & 0xFF;
        str[i]       = byte;
        shift_size -= 8;
    }
    str[7] = '\0';

    return std::string(str);
}

/**
 * @brief Unpacks the external xsd:string inside an ObjectId.
 *
 * @param oid ObjectId to unpack.
 * @return std::string string inside the ObjectId.
 */
std::string Conversions::unpack_string_xsd_extern_tmp(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_STRING_XSD_EXTERN || oid.get_type() == ObjectId::MASK_STRING_XSD_TMP);

    std::stringstream ss;
    UnifiedStorage::print_str(ss, oid);

    return ss.str();
}

/**
 * @brief Unpacks the inlined string with language inside an ObjectId.
 *
 * @param oid ObjectId to unpack.
 * @return a std::pair of the language id and the string contained in the ObjectId.
 */
std::pair<uint16_t, std::string> Conversions::unpack_string_lang_inlined(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_STRING_LANG_INLINED);
    const uint8_t* data = reinterpret_cast<uint8_t*>(&oid.id);

    char str[6] = {}; // 5 + 1 for null byte, zero initialized

    uint16_t lang_id = data[5];
    lang_id |= static_cast<uint16_t>(data[6]) << 8;

    for (size_t i = 0; i < 5; i++) {
        str[i] = data[4 - i];
    }

    return { lang_id, std::string(str) };
}

/**
 * @brief Unpacks the external string with language from an ObjectId.
 *
 * @param oid ObjectId to unpack.
 * @return a std::pair of the language id and the string contained in the ObjectId.
 */
std::pair<uint16_t, std::string> Conversions::unpack_string_lang_extern_tmp(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_STRING_LANG_EXTERN || oid.get_type() == ObjectId::MASK_STRING_LANG_TMP);

    auto language_id = oid.get_value() >> (ObjectId::STR_LANG_INLINE_BYTES * 8);

    std::stringstream ss;
    UnifiedStorage::print_str(ss, oid);

    return { language_id, ss.str() };
}

/**
 * @brief Unpacks the inlined string with datatype inside an ObjectId.
 *
 * @param oid ObjectId to unpack.
 * @return a std::pair of the datatype id and the string contained in the ObjectId.
 */
std::pair<uint16_t, std::string> Conversions::unpack_string_datatype_inlined(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_STRING_DATATYPE_INLINED);
    const uint8_t* data = reinterpret_cast<uint8_t*>(&oid.id);

    char str[6] = {}; // 5 + 1 for null byte, zero initialized

    uint16_t datatype_id = data[5];
    datatype_id |= static_cast<uint16_t>(data[6]) << 8;

    for (size_t i = 0; i < 5; i++) {
        str[i] = data[4 - i];
    }

    return { datatype_id, std::string(str) };
}

/**
 * @brief Unpacks the external string with datatype from an ObjectId.
 *
 * @param oid ObjectId to unpack.
 * @return a std::pair of the datatype id and the string contained in the ObjectId.
 */
std::pair<uint16_t, std::string> Conversions::unpack_string_datatype_extern_tmp(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_STRING_DATATYPE_EXTERN || oid.get_type() == ObjectId::MASK_STRING_DATATYPE_TMP);

    auto datatype_id = oid.get_value() >> (ObjectId::STR_DT_INLINE_BYTES * 8);

    std::stringstream ss;
    UnifiedStorage::print_str(ss, oid);

    return { datatype_id, ss.str() };
}

/**
 * @brief Packs a string into an ObjectId.
 *
 * @param str string to pack.
 * @return ObjectId containing the string.
 */
ObjectId Conversions::pack_string_simple(const std::string& str) {
    uint64_t oid;
    if (str.size() == 0) {
        return ObjectId(ObjectId::STRING_SIMPLE_EMPTY);
    } else if (str.size() <= ObjectId::STR_INLINE_BYTES) {
        oid = Inliner::inline_string(str.c_str()) | ObjectId::MASK_STRING_SIMPLE_INLINED;
    } else {
        oid = UnifiedStorage::get_str_id(str, ObjectId::MASK_STRING_SIMPLE);
    }
    return ObjectId(oid);
}

/**
 * @brief Packs a xsd:string into an ObjectId.
 *
 * @param str string to pack.
 * @return ObjectId containing the string.
 */
ObjectId Conversions::pack_string_xsd(const std::string& str) {
    uint64_t oid;
    if (str.size() == 0) {
        return ObjectId(ObjectId::STRING_XSD_EMPTY);
    } else if (str.size() <= ObjectId::STR_INLINE_BYTES) {
        oid = Inliner::inline_string(str.c_str()) | ObjectId::MASK_STRING_XSD_INLINED;
    } else {
        oid = UnifiedStorage::get_str_id(str, ObjectId::MASK_STRING_XSD);
    }
    return ObjectId(oid);
}

/**
 * @brief Packs a string with language into an ObjectId.
 *
 * @param lang language of the string to pack.
 * @param str string to pack.
 * @return ObjectId containing the string with language.
 */
ObjectId Conversions::pack_string_lang(uint16_t lang, const std::string& str) {
    uint64_t id;
    uint64_t lang64 = static_cast<uint64_t>(lang) << (ObjectId::STR_LANG_INLINE_BYTES * 8);
    if (str.size() == 0) {
        return ObjectId(ObjectId::MASK_STRING_LANG_INLINED | lang64);
    } else if (str.size() <= ObjectId::STR_LANG_INLINE_BYTES) {
        id = Inliner::inline_string5(str.c_str()) | ObjectId::MASK_STRING_LANG_INLINED | lang64;
    } else {
        id = UnifiedStorage::get_str_id(str, ObjectId::MASK_STRING_LANG) | lang64;
    }
    return ObjectId(id);
}

/**
 * @brief Packs a string with datatype into an ObjectId.
 *
 * @param dt datatype of the string to pack.
 * @param str string to pack.
 * @return ObjectId containing the string with datatype.
 */
ObjectId Conversions::pack_string_datatype(uint16_t dt, const std::string& str) {
    uint64_t id;
    uint64_t dt64 = static_cast<uint64_t>(dt) << (ObjectId::STR_DT_INLINE_BYTES * 8);
    if (str.size() == 0) {
        return ObjectId(ObjectId::MASK_STRING_DATATYPE_INLINED | dt64);
    } else if (str.size() <= ObjectId::STR_DT_INLINE_BYTES) {
        id = Inliner::inline_string5(str.c_str()) | ObjectId::MASK_STRING_DATATYPE_INLINED | dt64;
    } else {
        id = UnifiedStorage::get_str_id(str, ObjectId::MASK_STRING_DATATYPE) | dt64;
    }
    return ObjectId(id);
}

/**
 * @brief Pack an IRI string into an ObjectId.
 *
 * @param str IRI string to pack.
 * @return ObjectId containing the IRI.
 */
ObjectId Conversions::pack_iri(const std::string& str) {
    // If a prefix matches the IRI, store just the suffix and a pointer to the prefix
    uint64_t prefix_id = 0;
    std::string suffix = str;
    for (uint64_t i = 1; i < rdf_model.catalog().prefixes.size(); ++i) {
        std::string& prefix = rdf_model.catalog().prefixes[i];
        if (strncmp(str.c_str(), prefix.c_str(), prefix.size()) == 0) {
            suffix.erase(0, prefix.size());
            prefix_id = i << ObjectId::IRI_INLINE_BYTES*8;
            break;
        }
    }

    uint64_t suffix_id;
    if (str.size() <= ObjectId::IRI_INLINE_BYTES) {
        suffix_id = Inliner::inline_iri(suffix.c_str()) | ObjectId::MASK_IRI_INLINED;
    } else {
        suffix_id = UnifiedStorage::get_str_id(suffix, ObjectId::MASK_IRI);
    }
    return ObjectId(suffix_id | prefix_id);
}

/**
 * @brief Unpack an IRI string from an ObjectId.
 *
 * @param oid ObjectId containing the IRI to unpack.
 * @return std::string with the IRI.
 */
std::string Conversions::unpack_iri_inlined(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_IRI_INLINED);

    auto prefix_id = (oid.id >> 48) & 0xFF;
    auto prefix = rdf_model.catalog().prefixes[prefix_id];

    for (int i = 5; i >= 0; i--) {
        uint8_t c = (oid.id >> (i*8)) & 0xFF;
        if (c == 0) {
            break;
        }
        prefix += static_cast<char>(c);
    }

    return prefix;
}

std::string Conversions::unpack_iri_extern_tmp(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_IRI_EXTERN || oid.get_type() == ObjectId::MASK_IRI_TMP);

    auto prefix_id = oid.get_value() >>(ObjectId::IRI_INLINE_BYTES * 8);
    auto prefix = rdf_model.catalog().prefixes[prefix_id];

    std::stringstream ss;
    UnifiedStorage::print_str(ss, oid);

    prefix.append(ss.str());
    return prefix;
}

std::string Conversions::unpack_iri(ObjectId oid) {
    switch (oid.get_type()) {
    case ObjectId::MASK_IRI_INLINED:
        return unpack_iri_inlined(oid);
    case ObjectId::MASK_IRI_EXTERN:
    case ObjectId::MASK_IRI_TMP:
        return unpack_iri_extern_tmp(oid);
    default:
        throw LogicException("Called unpack_iri with incorrect ObjectId type, this should never happen");
    }
}

std::string Conversions::unpack_string_simple(ObjectId oid) {
    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_SIMPLE_INLINED:
        return unpack_string_simple_inlined(oid);
    case ObjectId::MASK_STRING_SIMPLE_EXTERN:
    case ObjectId::MASK_STRING_SIMPLE_TMP:
        return unpack_string_simple_extern_tmp(oid);
    default:
        throw LogicException("Called unpack_string with incorrect ObjectId type, this should never happen");
    }
}

std::string Conversions::unpack_string_xsd(ObjectId oid) {
    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_XSD_INLINED:
        return unpack_string_xsd_inlined(oid);
    case ObjectId::MASK_STRING_XSD_EXTERN:
    case ObjectId::MASK_STRING_XSD_TMP:
        return unpack_string_xsd_extern_tmp(oid);
    default:
        throw LogicException("Called unpack_string with incorrect ObjectId type, this should never happen");
    }
}

std::pair<uint16_t, std::string> Conversions::unpack_string_lang(ObjectId oid) {
    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_LANG_INLINED:
        return unpack_string_lang_inlined(oid);
    case ObjectId::MASK_STRING_LANG_EXTERN:
    case ObjectId::MASK_STRING_LANG_TMP:
        return unpack_string_lang_extern_tmp(oid);
    default:
        throw LogicException("Called unpack_string_lang with incorrect ObjectId type, this should never happen");
    }
}

std::pair<uint16_t, std::string> Conversions::unpack_string_datatype(ObjectId oid) {
    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_DATATYPE_INLINED:
        return unpack_string_datatype_inlined(oid);
    case ObjectId::MASK_STRING_DATATYPE_EXTERN:
    case ObjectId::MASK_STRING_DATATYPE_TMP:
        return unpack_string_datatype_extern_tmp(oid);
    default:
        throw LogicException("Called unpack_string_data with incorrect ObjectId type, this should never happen");
    }
}

std::string Conversions::unpack_named_node(ObjectId oid) {
    switch (oid.get_type()) {
    case ObjectId::MASK_NAMED_NODE_INLINED:
        return unpack_named_node_inlined(oid);
    case ObjectId::MASK_NAMED_NODE_EXTERN:
    case ObjectId::MASK_NAMED_NODE_TMP:
        return unpack_named_node_extern_tmp(oid);
    default:
        throw LogicException("Called unpack_named_node with incorrect ObjectId type, this should never happen");
    }
}

std::string Conversions::unpack_named_node_inlined(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_NAMED_NODE_INLINED);

    char str[8];
    int  shift_size = 6 * 8;
    for (int i = 0; i < ObjectId::MAX_INLINED_BYTES; i++) {
        uint8_t byte = (oid.id >> shift_size) & 0xFF;
        str[i]       = byte;
        shift_size -= 8;
    }
    str[7] = '\0';

    return std::string(str);
}

std::string Conversions::unpack_named_node_extern_tmp(ObjectId oid) {
    assert(oid.get_type() == ObjectId::MASK_NAMED_NODE_EXTERN
        || oid.get_type() == ObjectId::MASK_NAMED_NODE_TMP);

    std::stringstream ss;
    UnifiedStorage::print_str(ss, oid);

    return ss.str();
}