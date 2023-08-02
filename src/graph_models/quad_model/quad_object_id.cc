#include "quad_object_id.h"

#include "graph_models/object_id.h"
#include "storage/string_manager.h"
#include "storage/tmp_manager.h"

ObjectId QuadObjectId::get_fixed_node_inside(const std::string& str) {
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
        // TODO: support other forms of float (".01") ?
        if (str.find_first_not_of("0123456789+-") == std::string::npos) {
            // is int
            int64_t encoded_number = std::stoll(str);
            uint64_t mask = ObjectId::MASK_POSITIVE_INT;

            // TODO: should check if is it outside min/max possible values
            if (encoded_number < 0) {
                mask = ObjectId::MASK_NEGATIVE_INT;
                encoded_number *= -1;
                encoded_number = (~encoded_number) & ObjectId::VALUE_MASK;
            }
            return ObjectId(mask | encoded_number);
        } else {
            // is float
            float number = std::stof(str);
            auto src = reinterpret_cast<unsigned char*>(&number);

            auto oid = ObjectId::MASK_FLOAT;
            oid |= static_cast<uint64_t>(src[0]);
            oid |= static_cast<uint64_t>(src[1]) << 8;
            oid |= static_cast<uint64_t>(src[2]) << 16;
            oid |= static_cast<uint64_t>(src[3]) << 24;

            return ObjectId(oid);
        }

    default:
        return get_named_node(str);
    }
}

ObjectId QuadObjectId::get_edge(const std::string& str) {
    assert(str.size() >= 3);
    std::string tmp = str.substr(2, str.size() - 2);
    assert(str[1] == 'e');
    auto number = std::stoi(tmp);
    return ObjectId(number | ObjectId::MASK_EDGE);
}


ObjectId QuadObjectId::get_named_node(const std::string& str) {
    if (str.size() <= ObjectId::NAMED_NODE_INLINE_BYTES) {
        uint64_t res = 0;
        int shift_size = 8*(ObjectId::NAMED_NODE_INLINE_BYTES-1);
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


ObjectId QuadObjectId::get_string(const std::string& str) {
    if (str.size() <= ObjectId::STR_INLINE_BYTES) {
        uint64_t res = 0;
        int shift_size = 8*(ObjectId::STR_INLINE_BYTES-1);
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
