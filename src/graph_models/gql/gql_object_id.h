#pragma once

#include <cassert>

#include "graph_models/object_id.h"

namespace GQL_OID {
    enum class GenericType {
        NULL_ID = 0x00,
        STRING,
        NUMERIC,
        DATE,
        BOOL,
        PATH,
        NODE,
        EDGE,
        LABEL,
        KEY,
        LIST,
        // GEOMETRY, ?
    };

    enum class GenericSubType {
        NULL_ID = 0x00,
        // URI,
        STRING_SIMPLE,
        INTEGER,
        FLOAT,
        DOUBLE,
        DECIMAL,
        DATE,
        BOOL,
        PATH,
        NODE,
        EDGE,
        LABEL,
        KEY,
        LIST,
        // POINT, ?
    };

    enum class Type {
        NULL_ID = 0x00,
        // URI,
        STRING_SIMPLE_INLINE,
        STRING_SIMPLE_EXTERN,
        STRING_SIMPLE_TMP,
        INT56_INLINE,
        INT64_EXTERN,
        INT64_TMP,
        FLOAT32,
        DOUBLE64_EXTERN,
        DOUBLE64_TMP,
        DECIMAL_INLINE,
        DECIMAL_EXTERN,
        DECIMAL_TMP,
        DATE,
        TIME,
        DATETIME,
        DATETIMESTAMP,
        BOOL,
        PATH,
        NODE,
        DIRECTED_EDGE,
        UNDIRECTED_EDGE,
        NODE_LABEL,
        EDGE_LABEL,
        NODE_KEY,
        EDGE_KEY,
        LIST,
        // POINT, ?
    };

    static constexpr int MAX_INLINE_LEN_STRING = 7;

    inline constexpr Type get_type(ObjectId oid) {
        switch (oid.id >> 56) {
        case (ObjectId::MASK_NULL >> 56):
            return Type::NULL_ID;

        case (ObjectId::MASK_NODE >> 56):
            return Type::NODE;
        case (ObjectId::MASK_DIRECTED_EDGE >> 56):
            return Type::DIRECTED_EDGE;
        case (ObjectId::MASK_UNDIRECTED_EDGE >> 56):
            return Type::UNDIRECTED_EDGE;
        case (ObjectId::MASK_NODE_LABEL >> 56):
            return Type::NODE_LABEL;
        case (ObjectId::MASK_EDGE_LABEL >> 56):
            return Type::EDGE_LABEL;
        case (ObjectId::MASK_NODE_KEY >> 56):
            return Type::NODE_KEY;
        case (ObjectId::MASK_EDGE_KEY >> 56):
            return Type::EDGE_KEY;
        case (ObjectId::MASK_LIST >> 56):
            return Type::LIST;

        case (ObjectId::MASK_STRING_SIMPLE_INLINED >> 56):
            return Type::STRING_SIMPLE_INLINE;
        case (ObjectId::MASK_STRING_SIMPLE_EXTERN >> 56):
            return Type::STRING_SIMPLE_EXTERN;
        case (ObjectId::MASK_STRING_SIMPLE_TMP >> 56):
            return Type::STRING_SIMPLE_TMP;

        case (ObjectId::MASK_FLOAT >> 56):
            return Type::FLOAT32;
        case (ObjectId::MASK_NEGATIVE_INT >> 56):
            return Type::INT56_INLINE;
        case (ObjectId::MASK_POSITIVE_INT >> 56):
            return Type::INT56_INLINE;
        case (ObjectId::MASK_DECIMAL_INLINED >> 56):
            return Type::DECIMAL_INLINE;
        case (ObjectId::MASK_DECIMAL_EXTERN >> 56):
            return Type::DECIMAL_EXTERN;
        case (ObjectId::MASK_DECIMAL_TMP >> 56):
            return Type::DECIMAL_TMP;
        case (ObjectId::MASK_DOUBLE_EXTERN >> 56):
            return Type::DOUBLE64_EXTERN;
        case (ObjectId::MASK_DOUBLE_TMP >> 56):
            return Type::DOUBLE64_TMP;

        case (ObjectId::MASK_BOOL >> 56):
            return Type::BOOL;

        case (ObjectId::MASK_GQL_PATH >> 56):
            return Type::PATH;

        case (ObjectId::MASK_DT_DATE >> 56):
            return Type::DATE;
        case (ObjectId::MASK_DT_TIME >> 56):
            return Type::TIME;
        case (ObjectId::MASK_DT_DATETIME >> 56):
            return Type::DATETIME;
        case (ObjectId::MASK_DT_DATETIMESTAMP >> 56):
            return Type::DATETIMESTAMP;

        default:
            assert(false);
            return Type::NULL_ID;
        }
        // return static_cast<Type>(oid.id);
    }

    inline GenericSubType get_generic_sub_type(ObjectId oid) {
        switch (get_type(oid)) {
            case Type::NULL_ID:
                return GenericSubType::NULL_ID;
            case Type::NODE:
                return GenericSubType::NODE;
            case Type::DIRECTED_EDGE:
                return GenericSubType::EDGE;
            case Type::UNDIRECTED_EDGE:
                return GenericSubType::EDGE;
            case Type::NODE_LABEL:
                return GenericSubType::LABEL;
            case Type::EDGE_LABEL:
                return GenericSubType::LABEL;
            case Type::NODE_KEY:
                return GenericSubType::KEY;
            case Type::EDGE_KEY:
                return GenericSubType::KEY;
            case Type::LIST:
                return GenericSubType::LIST;
            case Type::STRING_SIMPLE_INLINE:
            case Type::STRING_SIMPLE_EXTERN:
            case Type::STRING_SIMPLE_TMP:
                return GenericSubType::STRING_SIMPLE;
            case Type::INT56_INLINE:
            case Type::INT64_EXTERN:
            case Type::INT64_TMP:
                return GenericSubType::INTEGER;
            case Type::FLOAT32:
                return GenericSubType::FLOAT;
            case Type::DOUBLE64_EXTERN:
            case Type::DOUBLE64_TMP:
                return GenericSubType::DOUBLE;
            case Type::DECIMAL_INLINE:
            case Type::DECIMAL_EXTERN:
            case Type::DECIMAL_TMP:
                return GenericSubType::DECIMAL;
            case Type::BOOL:
                return GenericSubType::BOOL;
            case Type::DATE:
            case Type::TIME:
            case Type::DATETIME:
            case Type::DATETIMESTAMP:
                return GenericSubType::DATE;
            case Type::PATH:
                return GenericSubType::PATH;
        }
        assert(false);
        return GenericSubType::NULL_ID;
    }

    inline GenericType get_generic_type(ObjectId oid) {
        switch (get_type(oid)) {
            case Type::NULL_ID:
                return GenericType::NULL_ID;
            case Type::NODE:
                return GenericType::NODE;
            case Type::DIRECTED_EDGE:
                return GenericType::EDGE;
            case Type::UNDIRECTED_EDGE:
                return GenericType::EDGE;
            case Type::NODE_LABEL:
                return GenericType::LABEL;
            case Type::EDGE_LABEL:
                return GenericType::LABEL;
            case Type::NODE_KEY:
                return GenericType::KEY;
            case Type::EDGE_KEY:
                return GenericType::KEY;
            case Type::LIST:
                return GenericType::LIST;
            case Type::STRING_SIMPLE_INLINE:
            case Type::STRING_SIMPLE_EXTERN:
            case Type::STRING_SIMPLE_TMP:
                return GenericType::STRING;
            case Type::INT56_INLINE:
            case Type::INT64_EXTERN:
            case Type::INT64_TMP:
            case Type::FLOAT32:
            case Type::DOUBLE64_EXTERN:
            case Type::DOUBLE64_TMP:
            case Type::DECIMAL_INLINE:
            case Type::DECIMAL_EXTERN:
            case Type::DECIMAL_TMP:
                return GenericType::NUMERIC;
            case Type::BOOL:
                return GenericType::BOOL;
            case Type::DATE:
            case Type::TIME:
            case Type::DATETIME:
            case Type::DATETIMESTAMP:
                return GenericType::DATE;
            case Type::PATH:
                return GenericType::PATH;
            default:
                assert(false);
                return GenericType::NULL_ID;
        }
    }
}
