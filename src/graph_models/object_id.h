#pragma once

#include <cstdint>
#include <type_traits>

class ObjectId {
public:
    static constexpr auto MAX_INLINED_BYTES = 7;  // Ids have 8 bytes, 1 for type and 7 remaining
    static constexpr auto TYPE_OFFSET       = 56; // total_bits - bits_for_type: 64 - 8

    // [4 bits generic type][2 bits sub type][2 bits mod]
    // MOD:
    // inline   0b00
    // external 0b01
    // tmp      0b10
    // or sub-type differentiation

    static constexpr uint64_t VALUE_MASK        = 0x00'FFFFFFFFFFFFFFUL;

    static constexpr uint64_t TYPE_MASK         = 0xFF'00000000000000UL; // 0b1111'11'11
    static constexpr uint64_t SUB_TYPE_MASK     = 0xFC'00000000000000UL; // 0b1111'11'00
    static constexpr uint64_t GENERIC_TYPE_MASK = 0xF0'00000000000000UL; // 0b1111'00'00

    static constexpr uint64_t MOD_MASK          = 0x03'00000000000000UL; // 0b0000'00'11
    static constexpr uint64_t MOD_INLINE        = 0x00'00000000000000UL; // 0b0000'00'00
    static constexpr uint64_t MOD_EXTERNAL      = 0x01'00000000000000UL; // 0b0000'00'01
    static constexpr uint64_t MOD_TMP           = 0x02'00000000000000UL; // 0b0000'00'10

    static constexpr uint64_t MASK_LITERAL_TAG  = 0x00'FFF00000000000UL; // 12 bits for language/datatype of string
    static constexpr uint64_t MASK_EXTERNAL_ID  = 0x00'000FFFFFFFFFFFUL; // 44 least significant bits for external/tmp id

    static constexpr uint64_t MASK_IRI_PREFIX   = 0x00'FF000000000000UL;
    static constexpr uint64_t MASK_IRI_CONTENT  = 0x00'00FFFFFFFFFFFFUL;

    static constexpr int IRI_INLINE_BYTES        = 6; // Number of bytes of IRI, excluding prefix id, stored inline
    static constexpr int STR_INLINE_BYTES        = 7; // Number of bytes of string stored inline
    static constexpr int NAMED_NODE_INLINE_BYTES = 7; // Number of bytes of named node stored inline
    static constexpr int STR_DT_INLINE_BYTES     = 5; // Number of bytes of string, excluding datatype id, stored inline
    static constexpr int STR_LANG_INLINE_BYTES   = 5; // Number of bytes of string, excluding language id, stored inline


    //                                                               Indicates mask to apply before comparing
    //                                                                                                  V
    static constexpr uint64_t MASK_NULL                    = 0x00'00000000000000UL; // 0b0000'00'00  GENERIC

    static constexpr uint64_t MASK_ANON                    = 0x10'00000000000000UL; // 0b0001'00'00  GENERIC/SUBTYPE
    static constexpr uint64_t MASK_ANON_INLINED            = 0x10'00000000000000UL; // 0b0001'00'00     TYPE
    static constexpr uint64_t MASK_ANON_TMP                = 0x12'00000000000000UL; // 0b0001'00'10     TYPE

    static constexpr uint64_t MASK_NAMED_NODE              = 0x20'00000000000000UL; // 0b0010'00'00  GENERIC
    static constexpr uint64_t MASK_NAMED_NODE_INLINED      = 0x20'00000000000000UL; // 0b0010'00'00    TYPE
    static constexpr uint64_t MASK_NAMED_NODE_EXTERN       = 0x21'00000000000000UL; // 0b0010'00'01    TYPE
    static constexpr uint64_t MASK_NAMED_NODE_TMP          = 0x22'00000000000000UL; // 0b0010'00'10    TYPE

    static constexpr uint64_t MASK_IRI                     = 0x30'00000000000000UL; // 0b0011'00'00  GENERIC
    static constexpr uint64_t MASK_IRI_INLINED             = 0x30'00000000000000UL; // 0b0011'00'00    TYPE
    static constexpr uint64_t MASK_IRI_EXTERN              = 0x31'00000000000000UL; // 0b0011'00'01    TYPE
    static constexpr uint64_t MASK_IRI_TMP                 = 0x32'00000000000000UL; // 0b0011'00'10    TYPE

    static constexpr uint64_t MASK_STRING                  = 0x40'00000000000000UL; // 0b0100'00'00  GENERIC
    static constexpr uint64_t MASK_STRING_SIMPLE           = 0x40'00000000000000UL; // 0b0100'00'00    SUBTYPE
    static constexpr uint64_t MASK_STRING_SIMPLE_INLINED   = 0x40'00000000000000UL; // 0b0100'00'00      TYPE
    static constexpr uint64_t MASK_STRING_SIMPLE_EXTERN    = 0x41'00000000000000UL; // 0b0100'00'01      TYPE
    static constexpr uint64_t MASK_STRING_SIMPLE_TMP       = 0x42'00000000000000UL; // 0b0100'00'10      TYPE
    static constexpr uint64_t MASK_STRING_XSD              = 0x44'00000000000000UL; // 0b0100'01'00    SUBTYPE
    static constexpr uint64_t MASK_STRING_XSD_INLINED      = 0x44'00000000000000UL; // 0b0100'01'00      TYPE
    static constexpr uint64_t MASK_STRING_XSD_EXTERN       = 0x45'00000000000000UL; // 0b0100'01'01      TYPE
    static constexpr uint64_t MASK_STRING_XSD_TMP          = 0x46'00000000000000UL; // 0b0100'01'10      TYPE
    static constexpr uint64_t MASK_STRING_LANG             = 0x48'00000000000000UL; // 0b0100'10'00    SUBTYPE
    static constexpr uint64_t MASK_STRING_LANG_INLINED     = 0x48'00000000000000UL; // 0b0100'10'00      TYPE
    static constexpr uint64_t MASK_STRING_LANG_EXTERN      = 0x49'00000000000000UL; // 0b0100'10'01      TYPE
    static constexpr uint64_t MASK_STRING_LANG_TMP         = 0x4A'00000000000000UL; // 0b0100'10'10      TYPE
    static constexpr uint64_t MASK_STRING_DATATYPE         = 0x4C'00000000000000UL; // 0b0100'11'00    SUBTYPE
    static constexpr uint64_t MASK_STRING_DATATYPE_INLINED = 0x4C'00000000000000UL; // 0b0100'11'00      TYPE
    static constexpr uint64_t MASK_STRING_DATATYPE_EXTERN  = 0x4D'00000000000000UL; // 0b0100'11'01      TYPE
    static constexpr uint64_t MASK_STRING_DATATYPE_TMP     = 0x4E'00000000000000UL; // 0b0100'11'10      TYPE

    static constexpr uint64_t MASK_NUMERIC                 = 0x50'00000000000000UL; // 0b0101'00'00  GENERIC
    static constexpr uint64_t MASK_INT                     = 0x50'00000000000000UL; // 0b0101'00'00    SUBTYPE
    static constexpr uint64_t MASK_NEGATIVE_INT            = 0x50'00000000000000UL; // 0b0101'00'00      TYPE   MOD used to differentiate
    static constexpr uint64_t MASK_POSITIVE_INT            = 0x51'00000000000000UL; // 0b0101'00'01      TYPE   positive and negative ints
    static constexpr uint64_t MASK_DECIMAL                 = 0x54'00000000000000UL; // 0b0101'01'00    SUBTYPE
    static constexpr uint64_t MASK_DECIMAL_INLINED         = 0x54'00000000000000UL; // 0b0101'01'00      TYPE
    static constexpr uint64_t MASK_DECIMAL_EXTERN          = 0x55'00000000000000UL; // 0b0101'01'01      TYPE
    static constexpr uint64_t MASK_DECIMAL_TMP             = 0x56'00000000000000UL; // 0b0101'01'10      TYPE
    static constexpr uint64_t MASK_FLOAT                   = 0x58'00000000000000UL; // 0b0101'10'00    SUBTYPE
    static constexpr uint64_t MASK_DOUBLE                  = 0x5C'00000000000000UL; // 0b0101'11'00    SUBTYPE
    static constexpr uint64_t MASK_DOUBLE_EXTERN           = 0x5D'00000000000000UL; // 0b0101'11'01      TYPE
    static constexpr uint64_t MASK_DOUBLE_TMP              = 0x5E'00000000000000UL; // 0b0101'11'10      TYPE

    static constexpr uint64_t MASK_DT                      = 0x60'00000000000000UL; // 0b0110'00'00  GENERIC
    static constexpr uint64_t MASK_DT_DATE                 = 0x60'00000000000000UL; // 0b0110'00'00    SUBTYPE
    static constexpr uint64_t MASK_DT_TIME                 = 0x64'00000000000000UL; // 0b0110'01'00    SUBTYPE
    static constexpr uint64_t MASK_DT_DATETIME             = 0x68'00000000000000UL; // 0b0110'10'00    SUBTYPE
    static constexpr uint64_t MASK_DT_DATETIMESTAMP        = 0x6C'00000000000000UL; // 0b0110'11'00    SUBTYPE

    static constexpr uint64_t MASK_BOOL                    = 0x70'00000000000000UL; // 0b0111'00'00  GENERIC

    static constexpr uint64_t MASK_EDGE                    = 0x80'00000000000000UL; // 0b1000'00'00  GENERIC
    static constexpr uint64_t MASK_PATH                    = 0x90'00000000000000UL; // 0b1001'00'00  GENERIC
    static constexpr uint64_t MASK_NOT_FOUND               = 0xA0'00000000000000UL; // 0b1010'00'00  GENERIC

    static constexpr uint64_t MASK_IRI_UUID_LOWER          = 0xA1'00000000000000UL; // 0b1010'00'01
    static constexpr uint64_t MASK_IRI_UUID_LOWER_TMP      = 0xA2'00000000000000UL; // 0b1010'00'10
    static constexpr uint64_t MASK_IRI_UUID_UPPER          = 0xA5'00000000000000UL; // 0b1010'01'01
    static constexpr uint64_t MASK_IRI_UUID_UPPER_TMP      = 0xA6'00000000000000UL; // 0b1010'01'10

    static constexpr uint64_t MASK_IRI_HEX_LOWER           = 0xA9'00000000000000UL; // 0b1010'10'01
    static constexpr uint64_t MASK_IRI_HEX_LOWER_TMP       = 0xAA'00000000000000UL; // 0b1010'10'10
    static constexpr uint64_t MASK_IRI_HEX_UPPER           = 0xAD'00000000000000UL; // 0b1010'11'01
    static constexpr uint64_t MASK_IRI_HEX_UPPER_TMP       = 0xAE'00000000000000UL; // 0b1010'11'10

    static_assert(MASK_NEGATIVE_INT < MASK_POSITIVE_INT, "Integers won't be ordered properly in the B+Tree.");
    static_assert(MASK_NEGATIVE_INT < 0x80'00000000000000UL, "Integer IDs can't be subtracted without overflow.");
    static_assert(MASK_POSITIVE_INT < 0x80'00000000000000UL, "Integer IDs can't be subtracted without overflow.");

    static constexpr uint64_t NULL_ID    = MASK_NULL;
    static constexpr uint64_t BOOL_FALSE = MASK_BOOL | 0UL;
    static constexpr uint64_t BOOL_TRUE  = MASK_BOOL | 1UL;

    uint64_t id;

    explicit constexpr ObjectId(uint64_t id) : id(id) { }

    ObjectId() = default;

    static constexpr ObjectId get_null() noexcept {
        return ObjectId(NULL_ID);
    }

    static constexpr ObjectId get_not_found() noexcept {
        return ObjectId(MASK_NOT_FOUND);
    }

    inline uint64_t get_type() const noexcept {
        return id & TYPE_MASK;
    }

    inline uint64_t get_sub_type() const noexcept {
        return id & SUB_TYPE_MASK;
    }

    inline uint64_t get_mod() const noexcept {
        return id & MOD_MASK;
    }

    inline uint64_t get_value() const noexcept {
        return id & VALUE_MASK;
    }

    inline bool is_null() const noexcept {
        return id == NULL_ID;
    }

    inline bool is_tmp() const noexcept {
        return (id & MOD_MASK) == MOD_TMP;
    }

    inline bool is_true() const noexcept {
        return id == BOOL_TRUE;
    }

    inline bool is_false() const noexcept {
        return id == BOOL_FALSE;
    }

    inline bool is_not_found() const noexcept {
        return id == MASK_NOT_FOUND;
    }

    inline bool is_valid() const noexcept {
        // Any other future "invalid" types should be added here.
        // Invalid types are those that should be skipped in contexts
        // such as aggregation functions.
        auto type = id & GENERIC_TYPE_MASK;
        return (type != MASK_NULL && type != MASK_NOT_FOUND);
    }

    inline bool operator==(const ObjectId& rhs) const noexcept {
        return id == rhs.id;
    }

    inline bool operator!=(const ObjectId& rhs) const noexcept {
        return id != rhs.id;
    }

    inline bool operator<(const ObjectId& rhs) const noexcept {
        return id < rhs.id;
    }

    inline bool operator>(const ObjectId& rhs) const noexcept {
        return id > rhs.id;
    }

    inline bool operator<=(const ObjectId& rhs) const noexcept {
        return id <= rhs.id;
    }

    inline bool operator>=(const ObjectId& rhs) const noexcept {
        return id >= rhs.id;
    }
};

static_assert(std::is_trivially_copyable<ObjectId>::value);
