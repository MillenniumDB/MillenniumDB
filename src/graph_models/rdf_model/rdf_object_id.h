#pragma once

#include <cassert>

#include "graph_models/object_id.h"

namespace RDF_OID {
    enum class GenericType {
        NULL_ID = 0x00,
        BLANK,
        IRI,
        STRING,
        NUMERIC,
        DATE,
        BOOL,
        PATH,
        TENSOR
        // GEOMETRY, ?
    };

    enum class GenericSubType {
        NULL_ID = 0x00,
        BLANK,
        IRI,
        STRING_SIMPLE,
        STRING_XSD,
        STRING_LANG,
        STRING_DATATYPE,
        INTEGER,
        FLOAT,
        DOUBLE,
        DECIMAL,
        DATE,
        BOOL,
        PATH,
        TENSOR_FLOAT,
        TENSOR_DOUBLE
        // POINT, ?
    };

    enum class Type {
        NULL_ID = 0x00,
        BLANK_INLINED,
        BLANK_TMP,
        IRI_INLINE,
        IRI_INLINE_INT_SUFFIX,
        IRI_EXTERN,
        IRI_TMP,
        IRI_UUID_UPPER,
        IRI_UUID_UPPER_TMP,
        IRI_UUID_LOWER,
        IRI_UUID_LOWER_TMP,
        IRI_HEX_UPPER,
        IRI_HEX_UPPER_TMP,
        IRI_HEX_LOWER,
        IRI_HEX_LOWER_TMP,
        STRING_SIMPLE_INLINE,
        STRING_SIMPLE_EXTERN,
        STRING_SIMPLE_TMP,
        STRING_XSD_INLINE,
        STRING_XSD_EXTERN,
        STRING_XSD_TMP,
        STRING_LANG_INLINE,
        STRING_LANG_EXTERN,
        STRING_LANG_TMP,
        STRING_DATATYPE_INLINE,
        STRING_DATATYPE_EXTERN,
        STRING_DATATYPE_TMP,
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
        TENSOR_FLOAT_INLINE,
        TENSOR_FLOAT_EXTERN,
        TENSOR_FLOAT_TMP,
        TENSOR_DOUBLE_INLINE,
        TENSOR_DOUBLE_EXTERN,
        TENSOR_DOUBLE_TMP
        // POINT, ?
    };

    static constexpr int MAX_INLINE_LEN_STRING = 7;
    static constexpr int MAX_INLINE_LEN_STRING_DATATYPE = 5;
    static constexpr int MAX_INLINE_LEN_STRING_LANG = 5;
    static constexpr int MAX_INLINE_LEN_IRI = 6;

    inline constexpr Type get_type(ObjectId oid) {
        switch (oid.id >> 56) {
        case (ObjectId::MASK_NULL >> 56):
            return Type::NULL_ID;
        case (ObjectId::MASK_ANON_INLINED >> 56):
            return Type::BLANK_INLINED;
        case (ObjectId::MASK_ANON_TMP >> 56):
            return Type::BLANK_TMP;

        case (ObjectId::MASK_IRI_INLINED >> 56):
            return Type::IRI_INLINE;
        case (ObjectId::MASK_IRI_EXTERN >> 56):
            return Type::IRI_EXTERN;
        case (ObjectId::MASK_IRI_TMP >> 56):
            return Type::IRI_TMP;

        case (ObjectId::MASK_IRI_UUID_LOWER >> 56):
            return Type::IRI_EXTERN;
        case (ObjectId::MASK_IRI_UUID_LOWER_TMP >> 56):
            return Type::IRI_TMP;
        case (ObjectId::MASK_IRI_UUID_UPPER >> 56):
            return Type::IRI_EXTERN;
        case (ObjectId::MASK_IRI_UUID_UPPER_TMP >> 56):
            return Type::IRI_TMP;

        case (ObjectId::MASK_IRI_HEX_LOWER >> 56):
            return Type::IRI_EXTERN;
        case (ObjectId::MASK_IRI_HEX_LOWER_TMP >> 56):
            return Type::IRI_TMP;
        case (ObjectId::MASK_IRI_HEX_UPPER >> 56):
            return Type::IRI_EXTERN;
        case (ObjectId::MASK_IRI_HEX_UPPER_TMP >> 56):
            return Type::IRI_TMP;

        case (ObjectId::MASK_STRING_SIMPLE_INLINED >> 56):
            return Type::STRING_SIMPLE_INLINE;
        case (ObjectId::MASK_STRING_SIMPLE_EXTERN >> 56):
            return Type::STRING_SIMPLE_EXTERN;
        case (ObjectId::MASK_STRING_SIMPLE_TMP >> 56):
            return Type::STRING_SIMPLE_TMP;

        case (ObjectId::MASK_STRING_XSD_INLINED >> 56):
            return Type::STRING_XSD_INLINE;
        case (ObjectId::MASK_STRING_XSD_EXTERN >> 56):
            return Type::STRING_XSD_EXTERN;
        case (ObjectId::MASK_STRING_XSD_TMP >> 56):
            return Type::STRING_XSD_TMP;

        case (ObjectId::MASK_STRING_LANG_INLINED >> 56):
            return Type::STRING_LANG_INLINE;
        case (ObjectId::MASK_STRING_LANG_EXTERN >> 56):
            return Type::STRING_LANG_EXTERN;
        case (ObjectId::MASK_STRING_LANG_TMP >> 56):
            return Type::STRING_LANG_TMP;

        case (ObjectId::MASK_STRING_DATATYPE_INLINED >> 56):
            return Type::STRING_DATATYPE_INLINE;
        case (ObjectId::MASK_STRING_DATATYPE_EXTERN >> 56):
            return Type::STRING_DATATYPE_EXTERN;
        case (ObjectId::MASK_STRING_DATATYPE_TMP >> 56):
            return Type::STRING_DATATYPE_TMP;

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

        case (ObjectId::MASK_PATH >> 56):
            return Type::PATH;

        case (ObjectId::MASK_DT_DATE >> 56):
            return Type::DATE;
        case (ObjectId::MASK_DT_TIME >> 56):
            return Type::TIME;
        case (ObjectId::MASK_DT_DATETIME >> 56):
            return Type::DATETIME;
        case (ObjectId::MASK_DT_DATETIMESTAMP >> 56):
            return Type::DATETIMESTAMP;

        case (ObjectId::MASK_TENSOR_FLOAT_INLINED >> 56):
            return Type::TENSOR_FLOAT_INLINE;
        case (ObjectId::MASK_TENSOR_FLOAT_EXTERN >> 56):
            return Type::TENSOR_FLOAT_EXTERN;
        case (ObjectId::MASK_TENSOR_FLOAT_TMP >> 56):
            return Type::TENSOR_FLOAT_TMP;
        case (ObjectId::MASK_TENSOR_DOUBLE_INLINED >> 56):
            return Type::TENSOR_DOUBLE_INLINE;
        case (ObjectId::MASK_TENSOR_DOUBLE_EXTERN >> 56):
            return Type::TENSOR_DOUBLE_EXTERN;
        case (ObjectId::MASK_TENSOR_DOUBLE_TMP >> 56):
            return Type::TENSOR_DOUBLE_TMP;

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
            case Type::BLANK_INLINED:
            case Type::BLANK_TMP:
                return GenericSubType::BLANK;
            case Type::IRI_INLINE:
            case Type::IRI_INLINE_INT_SUFFIX:
            case Type::IRI_EXTERN:
            case Type::IRI_TMP:
            case Type::IRI_UUID_UPPER:
            case Type::IRI_UUID_UPPER_TMP:
            case Type::IRI_UUID_LOWER:
            case Type::IRI_UUID_LOWER_TMP:
            case Type::IRI_HEX_UPPER:
            case Type::IRI_HEX_UPPER_TMP:
            case Type::IRI_HEX_LOWER:
            case Type::IRI_HEX_LOWER_TMP:
                return GenericSubType::IRI;
            case Type::STRING_SIMPLE_INLINE:
            case Type::STRING_SIMPLE_EXTERN:
            case Type::STRING_SIMPLE_TMP:
                return GenericSubType::STRING_SIMPLE;
            case Type::STRING_XSD_INLINE:
            case Type::STRING_XSD_EXTERN:
            case Type::STRING_XSD_TMP:
                return GenericSubType::STRING_XSD;
            case Type::STRING_LANG_INLINE:
            case Type::STRING_LANG_EXTERN:
            case Type::STRING_LANG_TMP:
                return GenericSubType::STRING_LANG;
            case Type::STRING_DATATYPE_INLINE:
            case Type::STRING_DATATYPE_EXTERN:
            case Type::STRING_DATATYPE_TMP:
                return GenericSubType::STRING_DATATYPE;
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
            case Type::TENSOR_FLOAT_INLINE:
            case Type::TENSOR_FLOAT_EXTERN:
            case Type::TENSOR_FLOAT_TMP:
                return GenericSubType::TENSOR_FLOAT;
            case Type::TENSOR_DOUBLE_INLINE:
            case Type::TENSOR_DOUBLE_EXTERN:
            case Type::TENSOR_DOUBLE_TMP:
                return GenericSubType::TENSOR_DOUBLE;
        }
        assert(false);
        return GenericSubType::NULL_ID;
    }

    inline GenericType get_generic_type(ObjectId oid) {
        switch (get_type(oid)) {
            case Type::NULL_ID:
                return GenericType::NULL_ID;
            case Type::BLANK_INLINED:
            case Type::BLANK_TMP:
                return GenericType::BLANK;
            case Type::IRI_INLINE:
            case Type::IRI_INLINE_INT_SUFFIX:
            case Type::IRI_EXTERN:
            case Type::IRI_TMP:
            case Type::IRI_UUID_UPPER:
            case Type::IRI_UUID_UPPER_TMP:
            case Type::IRI_UUID_LOWER:
            case Type::IRI_UUID_LOWER_TMP:
            case Type::IRI_HEX_UPPER:
            case Type::IRI_HEX_UPPER_TMP:
            case Type::IRI_HEX_LOWER:
            case Type::IRI_HEX_LOWER_TMP:
                return GenericType::IRI;
            case Type::STRING_SIMPLE_INLINE:
            case Type::STRING_SIMPLE_EXTERN:
            case Type::STRING_SIMPLE_TMP:
            case Type::STRING_XSD_INLINE:
            case Type::STRING_XSD_EXTERN:
            case Type::STRING_XSD_TMP:
            case Type::STRING_LANG_INLINE:
            case Type::STRING_LANG_EXTERN:
            case Type::STRING_LANG_TMP:
            case Type::STRING_DATATYPE_INLINE:
            case Type::STRING_DATATYPE_EXTERN:
            case Type::STRING_DATATYPE_TMP:
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
            case Type::TENSOR_FLOAT_INLINE:
            case Type::TENSOR_FLOAT_EXTERN:
            case Type::TENSOR_FLOAT_TMP:
            case Type::TENSOR_DOUBLE_INLINE:
            case Type::TENSOR_DOUBLE_EXTERN:
            case Type::TENSOR_DOUBLE_TMP:
                return GenericType::TENSOR;
            default:
                assert(false);
                return GenericType::NULL_ID;
        }
    }
}
