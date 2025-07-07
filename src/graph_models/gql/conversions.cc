#include "conversions.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <sstream>

#include "graph_models/gql/gql_model.h"
#include "graph_models/gql/gql_object_id.h"
#include "graph_models/inliner.h"
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

void Conversions::print_path(std::ostream& os, ObjectId oid)
{
    std::vector<ObjectId> oid_list;
    unpack_path(oid, oid_list);

    bool is_node = true;
    for (auto it = oid_list.begin(); it != oid_list.end();) {
        ObjectId current_oid = *it;

        if (is_node) {
            os << "(";
            debug_print(os, current_oid);
            os << ")";
            ++it;
        } else {
            ++it;
            ObjectId direction = *it;

            switch (direction.id) {
            case ObjectId::DIRECTION_LEFT: {
                os << "<-[";
                debug_print(os, current_oid);
                os << "]-";
                break;
            }
            case ObjectId::DIRECTION_RIGHT: {
                os << "-[";
                debug_print(os, current_oid);
                os << "]->";
                break;
            }
            case ObjectId::DIRECTION_UNDIRECTED: {
                os << "~[";
                debug_print(os, current_oid);
                os << "]~";
                break;
            }
            }
            ++it;
        }
        is_node = !is_node;
    }
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

size_t Conversions::print_string(ObjectId oid, char* out)
{
    switch (oid.get_type()) {
    case ObjectId::MASK_STRING_SIMPLE_INLINED: {
        return Inliner::print_string_inlined<7>(out, oid.id);
    }
    case ObjectId::MASK_STRING_SIMPLE_EXTERN: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        return string_manager.print_to_buffer(out, external_id);
    }
    case ObjectId::MASK_STRING_SIMPLE_TMP: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        return tmp_manager.print_to_buffer(out, external_id);
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
    TmpLists& tmp_list = tmp_manager.get_tmp_list();
    uint32_t file_id = tmp_list.get_file_id();
    uint64_t list_offset = tmp_list.insert(list);
    return ObjectId(ObjectId::MASK_LIST | (uint64_t(file_id) << 40) | list_offset);
}

void Conversions::unpack_list(ObjectId list_id, std::vector<ObjectId>& out)
{
    auto& lists = tmp_manager.get_tmp_list();
    assert((LIST_FILE_ID_MASK & list_id.id) >> 40 == lists.get_file_id());

    lists.get(out, list_id.id & LIST_OFFSET_MASK);
}

ObjectId Conversions::pack_path(const std::vector<ObjectId>& oid_list)
{
    ObjectId path_oid = pack_list(oid_list);
    return ObjectId((path_oid.id & ~ObjectId::TYPE_MASK) | ObjectId::MASK_GQL_PATH);
}

void Conversions::unpack_path(ObjectId oid, std::vector<ObjectId>& out)
{
    unpack_list(oid, out);
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
        std::vector<ObjectId> out;
        Conversions::unpack_list(oid, out);
        os << "[";
        for (auto it = out.begin(); it != out.end(); ++it) {
            if (it != out.begin()) {
                os << ",";
            }
            debug_print(os, *it);
        }
        os << "]";
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
        print_path(os, oid);
        break;
    }
    case GQL_OID::Type::DATE: {
        DateTime datetime = Conversions::unpack_date(oid);
        os << "Date(" << datetime.get_value_string() << ")";
        break;
    }
    case GQL_OID::Type::DATETIME: {
        DateTime datetime = Conversions::unpack_date(oid);
        os << "Datetime(" << datetime.get_value_string() << ")";
        break;
    }
    case GQL_OID::Type::TIME: {
        DateTime datetime = Conversions::unpack_date(oid);
        os << "Time(" << datetime.get_value_string() << ")";
        break;
    }
    case GQL_OID::Type::DATETIMESTAMP: {
        DateTime datetime = Conversions::unpack_date(oid);
        os << "DatetimeStamp(" << datetime.get_value_string() << ")";
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
