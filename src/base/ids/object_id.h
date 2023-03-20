#pragma once

#include <cstdint>
#include <type_traits>

enum class RDF_MASK {
    IRI_INLINED             = 0x02,
    IRI_EXTERN              = 0x03,
    STRING_INLINED          = 0x06,
    STRING_EXTERN           = 0x07,
    STRING_LANG_INLINED     = 0x08,
    STRING_LANG_EXTERN      = 0x09,
    STRING_DATATYPE_INLINED = 0x0A,
    STRING_DATATYPE_EXTERN  = 0x0B,
    BOOL                    = 0x0F,
    ANON                    = 0x10,
    DATETIME                = 0x13,
    DECIMAL                 = 0x14,
};

class ObjectId {
public:
    static constexpr auto MAX_INLINED_BYTES = 7;  // Ids have 8 bytes, 1 for type and 7 remaining
    static constexpr auto TYPE_OFFSET       = 56; // total_bits - bits_for_type: 64 - 8

    static constexpr uint64_t TYPE_MASK  = 0xFF'00000000000000UL;
    static constexpr uint64_t VALUE_MASK = 0x00'FFFFFFFFFFFFFFUL;

    // TODO: mover?
    static constexpr uint64_t MASK_NAMED_NODE_INLINED      = 0x00'00000000000000UL;
    static constexpr uint64_t MASK_NAMED_NODE_EXTERN       = 0x01'00000000000000UL;
    static constexpr uint64_t MASK_IRI_INLINED             = 0x02'00000000000000UL;
    static constexpr uint64_t MASK_IRI_EXTERN              = 0x03'00000000000000UL;
    static constexpr uint64_t MASK_STRING_INLINED          = 0x06'00000000000000UL;
    static constexpr uint64_t MASK_STRING_EXTERN           = 0x07'00000000000000UL;
    static constexpr uint64_t MASK_STRING_LANG_INLINED     = 0x08'00000000000000UL;
    static constexpr uint64_t MASK_STRING_LANG_EXTERN      = 0x09'00000000000000UL;
    static constexpr uint64_t MASK_STRING_DATATYPE_INLINED = 0x0A'00000000000000UL;
    static constexpr uint64_t MASK_STRING_DATATYPE_EXTERN  = 0x0B'00000000000000UL;
    static constexpr uint64_t MASK_NEGATIVE_INT            = 0x0C'00000000000000UL;
    static constexpr uint64_t MASK_POSITIVE_INT            = 0x0D'00000000000000UL;
    static constexpr uint64_t MASK_FLOAT                   = 0x0E'00000000000000UL;
    static constexpr uint64_t MASK_BOOL                    = 0x0F'00000000000000UL;
    static constexpr uint64_t MASK_ANON                    = 0x10'00000000000000UL;
    static constexpr uint64_t MASK_EDGE                    = 0x11'00000000000000UL;
    static constexpr uint64_t MASK_PATH                    = 0x12'00000000000000UL;
    static constexpr uint64_t MASK_DATETIME                = 0x13'00000000000000UL;
    static constexpr uint64_t MASK_DECIMAL_INLINED         = 0x14'00000000000000UL;
    static constexpr uint64_t MASK_DECIMAL_EXTERN          = 0x15'00000000000000UL;

    static_assert(MASK_NEGATIVE_INT < MASK_POSITIVE_INT, "Integers won't be ordered properly in the B+Tree.");

    static constexpr uint64_t NULL_OBJECT_ID      = 0;
    static constexpr uint64_t OBJECT_ID_NOT_FOUND = UINT64_MAX;

    uint64_t id;

    explicit ObjectId(uint64_t id) : id(id) { }

    ObjectId() : id(NULL_OBJECT_ID) { }

    static inline ObjectId get_null() noexcept {
        return ObjectId(NULL_OBJECT_ID);
    }

    static inline ObjectId get_not_found() noexcept {
        return ObjectId(OBJECT_ID_NOT_FOUND);
    }

    inline bool is_null() const noexcept {
        return id == NULL_OBJECT_ID;
    }

    inline bool is_not_found() const noexcept {
        return id == OBJECT_ID_NOT_FOUND;
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
