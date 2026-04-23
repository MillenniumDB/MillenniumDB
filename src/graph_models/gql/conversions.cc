#include "conversions.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <sstream>

#include "graph_models/gql/gql_model.h"
#include "graph_models/inliner.h"
#include "system/path_manager.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"
#include "third_party/dragonbox/dragonbox_to_chars.h"

using namespace GQL;

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
    switch (oid.type()) {
    case ObjectType::StringInl: {
        Inliner::print_string_inlined<7>(os, oid.id);
        break;
    }
    case ObjectType::StringExt: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        string_manager.print(os, external_id);
        break;
    }
    case ObjectType::StringTmp: {
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
    case ObjectType::StringInl: {
        return Inliner::print_string_inlined<7>(out, oid.id);
    }
    case ObjectType::StringExt: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        return string_manager.print_to_buffer(out, external_id);
    }
    case ObjectType::StringTmp: {
        uint64_t external_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        return tmp_manager.print_to_buffer(out, external_id);
    }
    default:
        throw LogicException("Called print_string with incorrect ObjectId type, this should never happen");
    }
}

// Converts an ObjectId into its lexical representation.
std::string Conversions::to_lexical_str(ObjectId oid)
{
    switch (oid.subtype()) {
    case ObjectSubType::String: {
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
            [&](ObjectId oid) { print_path_node(ss, oid); },
            [&](ObjectId oid, bool reverse) { print_path_edge(ss, oid, reverse); }
        );
        ss << ']';
        return ss.str();
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
    case ObjectSubType::Dictionary: // TODO: should print something?
    case ObjectSubType::List:  // TODO: should print something?
    case ObjectSubType::TensorFloat: // TODO: should print something?
    case ObjectSubType::TensorDouble:  // TODO: should print something?
    case ObjectSubType::Anon: // TODO: should print something?
    case ObjectSubType::Edge: // TODO: should print something?
    case ObjectSubType::NamedNode:
    case ObjectSubType::StringXsd:
    case ObjectSubType::StringLang:
    case ObjectSubType::StringDatatype:
    case ObjectSubType::Iri:
    case ObjectSubType::NotFound:
        break;
    }
    return "";
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

ObjectId Conversions::pack_path(const std::vector<ObjectId>& oid_list)
{
    ObjectId path_oid = pack_list(oid_list);
    return ObjectId((path_oid.id & ObjectId::VALUE_MASK) | ObjectId::MASK_PATH);
}

void Conversions::unpack_path(ObjectId oid, std::vector<ObjectId>& out)
{
    oid = ObjectId((oid.id & ObjectId::VALUE_MASK) | ObjectId::MASK_LIST_TMP);
    unpack_list(oid, out);
}

std::ostream& Conversions::debug_print(std::ostream& os, ObjectId oid)
{
    const auto unmasked_id = oid.id & ObjectId::VALUE_MASK;

    switch (oid.type()) {
    case ObjectType::AnonInl: {
        os << "_n" << unmasked_id;
        break;
    }
    case ObjectType::DirectedEdge: {
        os << "_e" << unmasked_id;
        break;
    }
    case ObjectType::UndirectedEdge: {
        os << "_u" << unmasked_id;
        break;
    }
    case ObjectType::NodeLabel: {
        os << gql_model.catalog.node_labels_str[unmasked_id];
        break;
    }
    case ObjectType::EdgeLabel: {
        os << gql_model.catalog.edge_labels_str[unmasked_id];
        break;
    }
    case ObjectType::NodeKey: {
        os << gql_model.catalog.node_keys_str[unmasked_id];
        break;
    }
    case ObjectType::EdgeKey: {
        os << gql_model.catalog.edge_keys_str[unmasked_id];
        break;
    }
    case ObjectType::ListExt:
    case ObjectType::ListTmp: {
        std::vector<ObjectId> out = Conversions::unpack_list(oid);
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
    case ObjectType::DictionaryExt:
    case ObjectType::DictionaryTmp: {
        std::unique_ptr<Dictionary> dict;
        Common::Conversions::unpack_dictionary(oid, dict);
        dict->to_string(os);
        break;
    }
    case ObjectType::StringInl:
    case ObjectType::StringExt:
    case ObjectType::StringTmp: {
        os << '"';
        print_string(oid, os);
        os << '"';
        break;
    }
    case ObjectType::PositiveInt56:
    case ObjectType::NegativeInt56: {
        os << Conversions::unpack_int(oid);
        break;
    }
    case ObjectType::Float: {
        float f = Conversions::unpack_float(oid);

        char float_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary32>];
        jkj::dragonbox::to_chars(f, float_buffer);

        os << float_buffer;
        break;
    }
    case ObjectType::DoubleExt:
    case ObjectType::DoubleTmp: {
        double d = Conversions::unpack_double(oid);

        char double_buffer[1 + jkj::dragonbox::max_output_string_length<jkj::dragonbox::ieee754_binary64>];
        jkj::dragonbox::to_chars(d, double_buffer);

        os << double_buffer;
        break;
    }
    case ObjectType::Bool: {
        os << (Conversions::unpack_bool(oid) ? "true" : "false");
        break;
    }
    case ObjectType::Path: {
        print_path(os, oid);
        break;
    }
    case ObjectType::Date: {
        DateTime datetime = Conversions::unpack_date(oid);
        os << "Date(" << datetime.get_value_string() << ")";
        break;
    }
    case ObjectType::Datetime: {
        DateTime datetime = Conversions::unpack_date(oid);
        os << "Datetime(" << datetime.get_value_string() << ")";
        break;
    }
    case ObjectType::Time: {
        DateTime datetime = Conversions::unpack_date(oid);
        os << "Time(" << datetime.get_value_string() << ")";
        break;
    }
    case ObjectType::Datetimestamp: {
        DateTime datetime = Conversions::unpack_date(oid);
        os << "DatetimeStamp(" << datetime.get_value_string() << ")";
        break;
    }
    case ObjectType::DecimalInl:
    case ObjectType::DecimalExt:
    case ObjectType::DecimalTmp: {
        auto decimal = Conversions::unpack_decimal(oid);
        os << decimal;
        break;
    }
    case ObjectType::Null: {
        os << "NULL";
        break;
    }
    case ObjectType::AnonTmp:
    case ObjectType::StringXsdInl:
    case ObjectType::StringXsdExt:
    case ObjectType::StringXsdTmp:
    case ObjectType::StringLangInl:
    case ObjectType::StringLangExt:
    case ObjectType::StringLangTmp:
    case ObjectType::StringDatatypeInl:
    case ObjectType::StringDatatypeExt:
    case ObjectType::StringDatatypeTmp:
    case ObjectType::IriInl:
    case ObjectType::IriExt:
    case ObjectType::IriTmp:
    case ObjectType::IriUuidLowerTmp:
    case ObjectType::IriUuidLowerExt:
    case ObjectType::IriUuidUpperTmp:
    case ObjectType::IriUuidUpperExt:
    case ObjectType::IriHexLowerTmp:
    case ObjectType::IriHexLowerExt:
    case ObjectType::IriHexUpperTmp:
    case ObjectType::IriHexUpperExt:
    case ObjectType::NamedNodeInl:
    case ObjectType::NamedNodeExt:
    case ObjectType::NamedNodeTmp:
    case ObjectType::TensorFloatExt:
    case ObjectType::TensorFloatTmp:
    case ObjectType::TensorDoubleExt:
    case ObjectType::TensorDoubleTmp:
    case ObjectType::NotFound:
        break;
    }
    return os;
}

ObjectId Conversions::to_boolean(ObjectId oid)
{
    uint64_t value = oid.get_value();

    switch (oid.subtype()) {
    case ObjectSubType::Bool:
        return oid;
    // String
    // Note: Extern strings will never be empty
    case ObjectSubType::String:
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(value != 0));
    // Integer
    case ObjectSubType::Int:
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(value != 0));
    // Float
    case ObjectSubType::Float: {
        auto f = unpack_float(oid);
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(f != 0 && !std::isnan(f)));
    }
    // Double
    case ObjectSubType::Double: {
        auto d = unpack_double(oid);
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(d != 0 && !std::isnan(d)));
    }
    // Decimal
    // Note: This assumes 0 is never represented as 0.0, 0.00, etc
    // Note: Extern decimals will never be zero
    case ObjectSubType::Decimal:
        return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(value != 0));
    // Can not be converted to boolean
    default:
        return ObjectId::get_null();
    }
}
