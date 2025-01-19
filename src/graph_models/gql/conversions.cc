#include "conversions.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <sstream>

#include "graph_models/gql/gql_model.h"
#include "graph_models/gql/gql_object_id.h"
#include "graph_models/inliner.h"
#include "query/query_context.h"
#include "system/path_manager.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

using namespace GQL;

std::string Conversions::unpack_string(ObjectId oid)
{
    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_SIMPLE_INLINED: {
        return Inliner::get_string_inlined<ObjectId::STR_INLINE_BYTES>(oid.id);
    }
    case ObjectId::MASK_STRING_SIMPLE_EXTERN: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(ss, external_id);
        return ss.str();
    }
    case ObjectId::MASK_STRING_SIMPLE_TMP: {
        std::stringstream ss;
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(ss, external_id);
        return ss.str();
    }
    default:
        throw LogicException("Called unpack_string with incorrect ObjectId type, this should never happen");
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

void Conversions::print_string(ObjectId oid, std::ostream& os)
{
    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_SIMPLE_INLINED: {
        Inliner::print_string_inlined<7>(os, oid.id);
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_EXTERN: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(os, external_id);
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_TMP: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        tmp_manager.print_str(os, external_id);
        break;
    }
    default:
        throw LogicException("Called unpack_string with incorrect ObjectId type, this should never happen");
    }
}

// Converts an ObjectId into its lexical representation.
std::string Conversions::to_lexical_str(ObjectId oid)
{
    switch (GQL_OID::get_type(oid)) {
    case GQL_OID::Type::STRING_SIMPLE_INLINE:
    case GQL_OID::Type::STRING_SIMPLE_EXTERN:
    case GQL_OID::Type::STRING_SIMPLE_TMP: {
        return unpack_string(oid);
    }
    case GQL_OID::Type::INT56_INLINE:
    case GQL_OID::Type::INT64_EXTERN:
    case GQL_OID::Type::INT64_TMP: {
        int64_t i = unpack_int(oid);
        return std::to_string(i);
    }
    case GQL_OID::Type::FLOAT32: {
        float f = unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        return std::string(float_buffer);
    }
    case GQL_OID::Type::DOUBLE64_EXTERN:
    case GQL_OID::Type::DOUBLE64_TMP: {
        double d = unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        return std::string(double_buffer);
    }
    case GQL_OID::Type::BOOL: {
        return (unpack_bool(oid) ? "true" : "false");
    }
    case GQL_OID::Type::PATH: {
        std::stringstream ss;
        ss << '[';
        path_manager.print(ss, Conversions::get_path_id(oid), &print_path_node, &print_path_edge);
        ss << ']';
        return ss.str();
    }
    case GQL_OID::Type::DATE:
    case GQL_OID::Type::DATETIME:
    case GQL_OID::Type::TIME:
    case GQL_OID::Type::DATETIMESTAMP: {
        DateTime datetime = unpack_date(oid);
        return datetime.get_value_string();
    }
    case GQL_OID::Type::DECIMAL_INLINE:
    case GQL_OID::Type::DECIMAL_EXTERN:
    case GQL_OID::Type::DECIMAL_TMP: {
        Decimal decimal = unpack_decimal(oid);
        return decimal.to_string();
    }
    case GQL_OID::Type::NULL_ID:
    case GQL_OID::Type::NODE:
    case GQL_OID::Type::DIRECTED_EDGE:
    case GQL_OID::Type::UNDIRECTED_EDGE:
    case GQL_OID::Type::NODE_LABEL:
    case GQL_OID::Type::EDGE_LABEL:
    case GQL_OID::Type::NODE_KEY:
    case GQL_OID::Type::EDGE_KEY:
    case GQL_OID::Type::LIST: {
        return "";
    }
    }
    return "";
}

ObjectId Conversions::pack_string_simple(const std::string& str)
{
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

ObjectId Conversions::pack_node_label(const std::string& label)
{
    if (gql_model.catalog.node_labels2id.count(label)) {
        uint64_t label_id = gql_model.catalog.node_labels2id[label];
        return ObjectId(label_id | ObjectId::MASK_NODE_LABEL);
    }

    return ObjectId::get_null();
}

ObjectId Conversions::pack_edge_label(const std::string& label)
{
    if (gql_model.catalog.edge_labels2id.count(label)) {
        uint64_t label_id = gql_model.catalog.edge_labels2id[label];
        return ObjectId(label_id | ObjectId::MASK_EDGE_LABEL);
    }

    return ObjectId::get_null();
}

ObjectId Conversions::pack_node_property(const std::string& property)
{
    if (gql_model.catalog.node_keys2id.count(property)) {
        uint64_t label_id = gql_model.catalog.node_keys2id[property];
        return ObjectId(label_id | ObjectId::MASK_NODE_KEY);
    }

    return ObjectId::get_null();
}

ObjectId Conversions::pack_edge_property(const std::string& property)
{
    if (gql_model.catalog.edge_keys2id.count(property)) {
        uint64_t label_id = gql_model.catalog.edge_keys2id[property];
        return ObjectId(label_id | ObjectId::MASK_EDGE_KEY);
    }

    return ObjectId::get_null();
}

ObjectId Conversions::pack_list(const std::vector<ObjectId>& list)
{
    TmpLists& tmp_list = get_query_ctx().get_tmp_list();
    uint32_t file_id = tmp_list.get_file_id();
    uint64_t list_offset = tmp_list.insert(list);
    return ObjectId(ObjectId::MASK_LIST | (uint64_t(file_id) << 40) | list_offset);
}

void Conversions::unpack_list(ObjectId list_id, std::vector<ObjectId>& out)
{
    auto& lists = get_query_ctx().get_tmp_list();
    assert((LIST_FILE_ID_MASK & list_id.id) >> 40 == lists.get_file_id());

    lists.get(out, list_id.id & LIST_OFFSET_MASK);
}

std::ostream& Conversions::debug_print(std::ostream& os, ObjectId oid)
{
    const auto unmasked_id = oid.id & ObjectId::VALUE_MASK;

    switch (GQL_OID::get_type(oid)) {
    case GQL_OID::Type::NODE: {
        os << "_n" << unmasked_id;
        break;
    }
    case GQL_OID::Type::DIRECTED_EDGE: {
        os << "_e" << unmasked_id;
        break;
    }
    case GQL_OID::Type::UNDIRECTED_EDGE: {
        os << "_u" << unmasked_id;
        break;
    }
    case GQL_OID::Type::NODE_LABEL: {
        os << gql_model.catalog.node_labels_str[unmasked_id];
        break;
    }
    case GQL_OID::Type::EDGE_LABEL: {
        os << gql_model.catalog.edge_labels_str[unmasked_id];
        break;
    }
    case GQL_OID::Type::NODE_KEY: {
        os << gql_model.catalog.node_keys_str[unmasked_id];
        break;
    }
    case GQL_OID::Type::EDGE_KEY: {
        os << gql_model.catalog.edge_keys_str[unmasked_id];
        break;
    }
    case GQL_OID::Type::LIST: {
        os << "list:" << std::hex << oid.id;
        break;
    }
    case GQL_OID::Type::STRING_SIMPLE_INLINE:
    case GQL_OID::Type::STRING_SIMPLE_EXTERN:
    case GQL_OID::Type::STRING_SIMPLE_TMP: {
        os << '"';
        print_string(oid, os);
        os << '"';
        break;
    }
    case GQL_OID::Type::INT56_INLINE:
    case GQL_OID::Type::INT64_EXTERN:
    case GQL_OID::Type::INT64_TMP: {
        os << Conversions::unpack_int(oid);
        break;
    }
    case GQL_OID::Type::FLOAT32: {
        float f = Conversions::unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        os << float_buffer;
        break;
    }
    case GQL_OID::Type::DOUBLE64_EXTERN:
    case GQL_OID::Type::DOUBLE64_TMP: {
        double d = Conversions::unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        os << double_buffer;
        break;
    }
    case GQL_OID::Type::BOOL: {
        os << (Conversions::unpack_bool(oid) ? "true" : "false");
        break;
    }
    case GQL_OID::Type::PATH: {
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
    case GQL_OID::Type::DATE:
    case GQL_OID::Type::DATETIME:
    case GQL_OID::Type::TIME:
    case GQL_OID::Type::DATETIMESTAMP: {
        DateTime datetime = Conversions::unpack_date(oid);

        os << '"' << datetime.get_value_string();
        os << "\"^^<" << datetime.get_datatype_string() << ">";
        break;
    }
    case GQL_OID::Type::DECIMAL_INLINE:
    case GQL_OID::Type::DECIMAL_EXTERN:
    case GQL_OID::Type::DECIMAL_TMP: {
        auto decimal = Conversions::unpack_decimal(oid);
        os << decimal;
        break;
    }
    case GQL_OID::Type::NULL_ID: {
        os << "NULL";
        break;
    }
    }
    return os;
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

Decimal Conversions::unpack_decimal_inlined(ObjectId oid)
{
    assert(oid.get_type() == ObjectId::MASK_DECIMAL_INLINED);

    auto sign = (oid.id & Conversions::DECIMAL_SIGN_MASK) != 0;
    auto number = (oid.id & Conversions::DECIMAL_NUMBER_MASK) >> 4;
    auto decimals = oid.id & Conversions::DECIMAL_SEPARATOR_MASK;

    std::string dec_str;

    if (sign) {
        dec_str += '-';
    } else {
        dec_str += '+';
    }

    auto num_str = std::to_string(number);
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

Decimal Conversions::unpack_decimal(ObjectId oid)
{
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

ObjectId Conversions::pack_decimal(Decimal dec)
{
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

ObjectId Conversions::pack_double(double dbl)
{
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
uint8_t Conversions::calculate_optype(ObjectId oid1, ObjectId oid2)
{
    auto optype1 = calculate_optype(oid1);
    auto optype2 = calculate_optype(oid2);
    return std::max(optype1, optype2);
}

/**
 *  @brief Calculates the generic datatypes of the operand in an expression.
 *  @param oid ObjectId of the operand involved in an expression.
 *  @return generic numeric datatype of the operand or OPTYPE_INVALID if oid is not numeric
 */
uint8_t Conversions::calculate_optype(ObjectId oid)
{
    switch (oid.get_sub_type()) {
    case ObjectId::MASK_INT:
        return OPTYPE_INTEGER;
    case ObjectId::MASK_DECIMAL:
        return OPTYPE_DECIMAL;
    case ObjectId::MASK_FLOAT:
        return OPTYPE_FLOAT;
    case ObjectId::MASK_DOUBLE:
        return OPTYPE_DOUBLE;
    default:
        return OPTYPE_INVALID;
    }
}

/**
 *  @brief Converts an ObjectId to int64_t if permitted.
 *  @param oid ObjectId to convert.
 *  @return an int64_t representing the ObjectId.
 *  @throws LogicException if the ObjectId has no permitted type.
 */
int64_t Conversions::to_integer(ObjectId oid)
{
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
Decimal Conversions::to_decimal(ObjectId oid)
{
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
float Conversions::to_float(ObjectId oid)
{
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
double Conversions::to_double(ObjectId oid)
{
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
