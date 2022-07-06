#include "graph_object_visitor.h"

#include "base/exceptions.h"
#include "query_optimizer/quad_model/quad_model.h"

GraphObjectVisitor::GraphObjectVisitor(bool create_if_not_exists) :
    create_if_not_exists (create_if_not_exists) { }


ObjectId GraphObjectVisitor::operator()(const GraphObject& obj) const {
    switch (obj.type) {
        case GraphObjectType::NAMED_INLINED: {
            std::string str(obj.value.named_inlined.id);
            uint64_t res = 0;
            int shift_size = 8*6;
            // MUST convert to uint8_t and then to uint64_t.
            // Shift with shift_size >=32 is undefined behaviour.
            for (uint8_t byte : str) {
                uint64_t byte64 = static_cast<uint64_t>(byte);
                res |= byte64 << shift_size;
                shift_size -= 8;
            }
            return ObjectId(res | ObjectId::IDENTIFIABLE_INLINED_MASK);
        }

        case GraphObjectType::NAMED_TMP: {
            if (create_if_not_exists) {
                bool created;
                auto external_id = quad_model.strings_hash().get_or_create_id(*obj.value.named_tmp.name, &created);
                return ObjectId(external_id | ObjectId::IDENTIFIABLE_EXTERNAL_MASK);
            } else {
                auto external_id = quad_model.strings_hash().get_id(*obj.value.named_tmp.name);
                if (external_id == ObjectId::OBJECT_ID_NOT_FOUND) {
                    return ObjectId::get_not_found();
                } else {
                    return ObjectId(external_id | ObjectId::IDENTIFIABLE_EXTERNAL_MASK);
                }
            }
        }

        case GraphObjectType::STR_INLINED: {
            std::string str(obj.value.str_inlined.id);

            uint64_t res = 0;
            int shift_size = 8*6;
            // MUST convert to uint8_t and then to uint64_t.
            // Shift with shift_size >=32 is undefined behaviour.
            for (uint8_t byte : str) {
                uint64_t byte64 = static_cast<uint64_t>(byte);
                res |= byte64 << shift_size;
                shift_size -= 8;
            }
            return ObjectId(res | ObjectId::VALUE_INLINE_STR_MASK);
        }

        case GraphObjectType::STR_TMP: {
            if (create_if_not_exists) {
                bool created;
                auto external_id = quad_model.strings_hash().get_or_create_id(*obj.value.str_tmp.str, &created);
                return ObjectId(external_id | ObjectId::VALUE_EXTERNAL_STR_MASK);
            } else {
                auto external_id = quad_model.strings_hash().get_id(*obj.value.str_tmp.str);
                if (external_id == ObjectId::OBJECT_ID_NOT_FOUND) {
                    return ObjectId::get_not_found();
                } else {
                    return ObjectId(external_id | ObjectId::VALUE_EXTERNAL_STR_MASK);
                }
            }
        }

        case GraphObjectType::INT: {
            int64_t int_value = obj.value.i;

            if (int_value < 0) {
                int_value *= -1;

                // check if it needs more than 7 bytes
                if ( (int_value & 0xFF00'0000'0000'0000UL) == 0) {
                    int_value = (~int_value) & 0x00FF'FFFF'FFFF'FFFFUL;
                    return ObjectId(ObjectId::VALUE_NEGATIVE_INT_MASK | int_value);
                } else {
                    // VALUE_EXTERNAL_INT_MASK
                    throw NotSupportedException("BIG INTEGERS NOT SUPPORTED YET");
                }
            }
            else {
                // check if it needs more than 7 bytes
                if ( (int_value & 0xFF00'0000'0000'0000UL) == 0) {
                    return ObjectId(ObjectId::VALUE_POSITIVE_INT_MASK | int_value);
                } else {
                    // VALUE_EXTERNAL_INT_MASK
                    throw NotSupportedException("BIG INTEGERS NOT SUPPORTED YET");
                }
            }
        }

        case GraphObjectType::FLOAT: {
            static_assert(sizeof(obj.value.f) == 4);
            unsigned char bytes[sizeof(obj.value.f)];
            memcpy(bytes, &obj.value.f, sizeof(obj.value.f));

            uint64_t res = 0;
            int shift_size = 0;
            for (std::size_t i = 0; i < sizeof(bytes); ++i) {
                uint64_t byte = bytes[i];
                res |= byte << shift_size;
                shift_size += 8;
            }
            return ObjectId(ObjectId::VALUE_FLOAT_MASK | res);
        }

        case GraphObjectType::BOOL: {
            if (obj.value.b) {
                return ObjectId(ObjectId::VALUE_BOOL_MASK | 0x01);
            } else {
                return ObjectId(ObjectId::VALUE_BOOL_MASK | 0x00);
            }
        }

        case GraphObjectType::NAMED_EXTERNAL:
            return ObjectId(obj.value.named_external.external_id | ObjectId::IDENTIFIABLE_EXTERNAL_MASK);

        case GraphObjectType::STR_EXTERNAL:
            return ObjectId(obj.value.str_external.external_id | ObjectId::VALUE_EXTERNAL_STR_MASK);

        case GraphObjectType::EDGE:
            return ObjectId(ObjectId::CONNECTION_MASK | obj.value.edge.id);

        case GraphObjectType::ANON:
            return ObjectId(ObjectId::ANONYMOUS_NODE_MASK | obj.value.anon.id);

        case GraphObjectType::NULL_OBJ:
            return ObjectId::get_null();

        case GraphObjectType::NOT_FOUND:
            return ObjectId::get_not_found();

        case GraphObjectType::PATH:
            return ObjectId(ObjectId::VALUE_PATH_MASK | obj.value.path.path_id);
    }
    // unreachable
    return ObjectId::get_null();
}
