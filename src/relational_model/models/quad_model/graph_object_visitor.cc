#include "graph_object_visitor.h"

GraphObjectVisitor::GraphObjectVisitor(const QuadModel& model, bool create_if_not_exists) :
    model                (model),
    create_if_not_exists (create_if_not_exists) { }


ObjectId GraphObjectVisitor::operator()(const IdentifiableInlined& identifiable_inlined) const {
    std::string str(identifiable_inlined.id);
    uint64_t res = 0;
    int shift_size = 8*6;
    // MUST convert to uint8_t and then to uint64_t.
    // Shift with shift_size >=32 is undefined behaviour.
    for (uint8_t byte : str) {
        uint64_t byte64 = static_cast<uint64_t>(byte);
        res |= byte64 << shift_size;
        shift_size -= 8;
    }
    return ObjectId(res | GraphModel::IDENTIFIABLE_INLINED_MASK);
}


ObjectId GraphObjectVisitor::operator()(const IdentifiableExternal& identifiable_external) const {
    const std::string str(identifiable_external.id);
    if (create_if_not_exists) {
        bool created;
        auto external_id = model.strings_hash().get_or_create_id(str, &created);
        return ObjectId(external_id | GraphModel::IDENTIFIABLE_EXTERNAL_MASK);
    } else {
        auto external_id = model.strings_hash().get_id(str);
        if (external_id == ObjectId::OBJECT_ID_NOT_FOUND) {
            return ObjectId::get_not_found();
        } else {
            return ObjectId(external_id | GraphModel::IDENTIFIABLE_EXTERNAL_MASK);
        }
    }
}


ObjectId GraphObjectVisitor::operator()(const Edge& edge) const {
    return ObjectId(GraphModel::CONNECTION_MASK | edge.id);
}


ObjectId GraphObjectVisitor::operator()(const AnonymousNode& anonymous_node) const {
    return ObjectId(GraphModel::ANONYMOUS_NODE_MASK | anonymous_node.id);
}


ObjectId GraphObjectVisitor::operator()(const StringInlined& string_inlined) const {
    std::string str(string_inlined.id);

    uint64_t res = 0;
    int shift_size = 8*6;
    // MUST convert to uint8_t and then to uint64_t.
    // Shift with shift_size >=32 is undefined behaviour.
    for (uint8_t byte : str) {
        uint64_t byte64 = static_cast<uint64_t>(byte);
        res |= byte64 << shift_size;
        shift_size -= 8;
    }
    return ObjectId(res | GraphModel::VALUE_INLINE_STR_MASK);
}


ObjectId GraphObjectVisitor::operator()(const StringExternal& string_external) const {
    const std::string str(string_external.id);
    if (create_if_not_exists) {
        bool created;
        auto external_id = model.strings_hash().get_or_create_id(str, &created);
        return ObjectId(external_id | GraphModel::VALUE_EXTERNAL_STR_MASK);
    } else {
        auto external_id = model.strings_hash().get_id(str);
        if (external_id == ObjectId::OBJECT_ID_NOT_FOUND) {
            return ObjectId::get_not_found();
        } else {
            return ObjectId(external_id | GraphModel::VALUE_EXTERNAL_STR_MASK);
        }
    }
}


ObjectId GraphObjectVisitor::operator()(const NullGraphObject&) const {
    return ObjectId::get_null();
}


ObjectId GraphObjectVisitor::operator()(const NotFoundObject&) const {
    return ObjectId::get_not_found();
}


ObjectId GraphObjectVisitor::operator()(const int64_t n) const {
    int64_t int_value = n;

    if (int_value < 0) {
        int_value *= -1;

        // check if it needs more than 7 bytes
        if ( (int_value & 0xFF00'0000'0000'0000UL) == 0) {
            int_value = (~int_value) & 0x00FF'FFFF'FFFF'FFFFUL;
            return ObjectId(GraphModel::VALUE_NEGATIVE_INT_MASK | int_value);
        } else {
            // VALUE_EXTERNAL_INT_MASK
            throw std::logic_error("BIG INTEGERS NOT SUPPORTED YET");
        }
    }
    else {
        // check if it needs more than 7 bytes
        if ( (int_value & 0xFF00'0000'0000'0000UL) == 0) {
            return ObjectId(GraphModel::VALUE_POSITIVE_INT_MASK | int_value);
        } else {
            // VALUE_EXTERNAL_INT_MASK
            throw std::logic_error("BIG INTEGERS NOT SUPPORTED YET");
        }
    }
}


ObjectId GraphObjectVisitor::operator()(const bool value_bool) const {
    if (value_bool) {
        return ObjectId(GraphModel::VALUE_BOOL_MASK | 0x01);
    } else {
        return ObjectId(GraphModel::VALUE_BOOL_MASK | 0x00);
    }
}


ObjectId GraphObjectVisitor::operator()(const float value_float) const {
    static_assert(sizeof(value_float) == 4);
    unsigned char bytes[sizeof(value_float)];
    memcpy(bytes, &value_float, sizeof(value_float));

    uint64_t res = 0;
    int shift_size = 0;
    for (std::size_t i = 0; i < sizeof(bytes); ++i) {
        uint64_t byte = bytes[i];
        res |= byte << shift_size;
        shift_size += 8;
    }
    return ObjectId(GraphModel::VALUE_FLOAT_MASK | res);
}


ObjectId GraphObjectVisitor::operator()(const Path& path) const {
    return ObjectId(GraphModel::VALUE_PATH_MASK | path.path_id);
}
