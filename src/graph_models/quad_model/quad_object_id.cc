#include "quad_object_id.h"

#include "graph_models/common/conversions.h"
#include "graph_models/object_id.h"
#include "system/string_manager.h"
#include "system/tmp_manager.h"

namespace QuadObjectId {

ObjectId get_fixed_node_inside(const std::string& str) {
    assert(!str.empty());
    switch (str[0]) {
    case '"': { // "string"
        assert(str.size() >= 2);
        // delete surrounding quotes
        std::string tmp = str.substr(1, str.size() - 2);
        return get_string(tmp);
    }
    case '_': {
        assert(str.size() >= 3);
        // delete first 2 characters "_a" or "_e"
        std::string tmp = str.substr(2, str.size() - 2);
        auto number = std::stoi(tmp);
        if (str[1] == 'a') {
            return ObjectId(number | ObjectId::MASK_ANON_INLINED);
        } else {
            assert(str[1] == 'e');
            return ObjectId(number | ObjectId::MASK_EDGE);
        }
    }
    case 't': {
        if (str == "true") {
            return ObjectId(ObjectId::BOOL_TRUE);
        } else {
            return get_named_node(str);
        }
    }
    case 'f': {
        if (str == "false") {
            return ObjectId(ObjectId::BOOL_FALSE);
        } else {
            return get_named_node(str);
        }
    }
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '+':
    case '-':
        if (str.find_first_not_of("0123456789+-") == std::string::npos) {
            // is int
            return Common::Conversions::pack_int(std::stoll(str));
        } else {
            // is float
            return Common::Conversions::pack_float(std::strtof(str.c_str(), nullptr));
        }

    default:
        return get_named_node(str);
    }
}

ObjectId get_edge(const std::string& str) {
    assert(str.size() >= 3);
    std::string tmp = str.substr(2, str.size() - 2);
    assert(str[1] == 'e');
    auto number = std::stoi(tmp);
    return ObjectId(number | ObjectId::MASK_EDGE);
}


ObjectId get_named_node(const std::string& str) {
    if (str.size() <= ObjectId::NAMED_NODE_INLINE_BYTES) {
        uint64_t res = 0;
        int shift_size = 8 * (ObjectId::NAMED_NODE_INLINE_BYTES - 1);
        // MUST convert to uint8_t and then to uint64_t.
        // Shift with shift_size >=32 is undefined behaviour.
        for (uint8_t byte : str) {
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return ObjectId(res | ObjectId::MASK_NAMED_NODE_INLINED);
    } else {
        auto str_id = string_manager.get_str_id(str);
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            return ObjectId(ObjectId::MASK_NAMED_NODE_EXTERN | str_id);
        } else {
            return ObjectId(ObjectId::MASK_NAMED_NODE_TMP | tmp_manager.get_str_id(str));
        }
    }
}


ObjectId get_string(const std::string& str) {
    if (str.size() <= ObjectId::STR_INLINE_BYTES) {
        uint64_t res = 0;
        int shift_size = 8 * (ObjectId::STR_INLINE_BYTES - 1);
        // MUST convert to uint8_t and then to uint64_t.
        // Shift with shift_size >=32 is undefined behaviour.
        for (uint8_t byte : str) {
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return ObjectId(res | ObjectId::MASK_STRING_SIMPLE_INLINED);
    } else {
        auto str_id = string_manager.get_str_id(str);
        if (str_id != ObjectId::MASK_NOT_FOUND) {
            return ObjectId(ObjectId::MASK_STRING_SIMPLE_EXTERN | str_id);
        } else {
            return ObjectId(ObjectId::MASK_STRING_SIMPLE_TMP | tmp_manager.get_str_id(str));
        }
    }
}
} // namespace QuadObjectId
