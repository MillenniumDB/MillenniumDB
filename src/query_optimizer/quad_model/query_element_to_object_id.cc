#include "query_element_to_object_id.h"

#include "base/exceptions.h"
#include "query_optimizer/quad_model/quad_model.h"
#include "storage/string_manager.h"


ObjectId QueryElementToObjectId::operator()(const QueryElement& obj) {
    return std::visit(*this, obj.value);
}


ObjectId QueryElementToObjectId::operator()(const Var&) {
    throw LogicException("Var cannot be converted into ObjectId");
}


ObjectId QueryElementToObjectId::operator()(const NamedNode& named_node) {
    return get_string_id(named_node.name, ObjectId::MASK_NAMED_NODE_INLINED, ObjectId::MASK_NAMED_NODE_EXTERN);
}


ObjectId QueryElementToObjectId::operator()(const std::string& str) {
    return get_string_id(str, ObjectId::MASK_STRING_INLINED, ObjectId::MASK_STRING_EXTERN);
}


ObjectId QueryElementToObjectId::get_string_id(const std::string& str, uint64_t mask_inlined, uint64_t mask_external) {
    if (str.size() < 8) {
        uint64_t res = 0;
        int shift_size = 8*6;
        // MUST convert to uint8_t and then to uint64_t.
        // Shift with shift_size >=32 is undefined behaviour.
        for (uint8_t byte : str) {
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return ObjectId(res | mask_inlined);
    } else {
        return ObjectId(string_manager.get_str_id(str, create_if_not_exists) | mask_external);
    }
}


ObjectId QueryElementToObjectId::operator()(const AnonymousNode& anon) {
    return ObjectId(ObjectId::MASK_ANON | anon.id);
}


ObjectId QueryElementToObjectId::operator()(const Edge& edge) {
    return ObjectId(ObjectId::MASK_EDGE | edge.id);
}


ObjectId QueryElementToObjectId::operator()(const bool b) {
    return ObjectId(ObjectId::MASK_BOOL | (b ? 0x01 : 0x00));
}


ObjectId QueryElementToObjectId::operator()(int64_t int_value) {
    if (int_value < 0) {
        int_value *= -1;

        // check if it needs more than 7 bytes
        if ( (int_value & 0xFF00'0000'0000'0000UL) == 0) {
            int_value = (~int_value) & 0x00FF'FFFF'FFFF'FFFFUL;
            return ObjectId(ObjectId::MASK_NEGATIVE_INT | int_value);
        } else {
            throw NotSupportedException("saving big integers (>= 2^56)");
        }
    }
    else {
        // check if it needs more than 7 bytes
        if ( (int_value & 0xFF00'0000'0000'0000UL) == 0) {
            return ObjectId(ObjectId::MASK_POSITIVE_INT | int_value);
        } else {
            throw NotSupportedException("saving big integers (>= 2^56)");
        }
    }
}


ObjectId QueryElementToObjectId::operator()(const float f) {
    static_assert(sizeof(f) == 4);
    unsigned char bytes[sizeof(f)];
    memcpy(bytes, &f, sizeof(f));

    uint64_t res = 0;
    int shift_size = 0;
    for (std::size_t i = 0; i < sizeof(bytes); ++i) {
        uint64_t byte = bytes[i];
        res |= byte << shift_size;
        shift_size += 8;
    }
    return ObjectId(ObjectId::MASK_FLOAT | res);
}
