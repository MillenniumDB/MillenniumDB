#pragma once

#include <cstdint>
#include <type_traits>

class ObjectId {
public:
    static constexpr auto MAX_INLINED_BYTES = 7; // Ids have 8 bytes, 1 for type and 7 remaining
    static constexpr auto TYPE_OFFSET       = 56; // total_bits - bits_for_type: 64 - 8

    static constexpr uint64_t TYPE_MASK  = 0xFF'00000000000000UL;
    static constexpr uint64_t VALUE_MASK = 0x00'FFFFFFFFFFFFFFUL;

    // TYPES
    static constexpr uint64_t IDENTIFIABLE_EXTERNAL_MASK = 0x00'00000000000000UL;
    static constexpr uint64_t IDENTIFIABLE_INLINED_MASK  = 0x01'00000000000000UL;
    static constexpr uint64_t ANONYMOUS_NODE_MASK        = 0x02'00000000000000UL;
    static constexpr uint64_t CONNECTION_MASK            = 0x03'00000000000000UL;
    static constexpr uint64_t VALUE_EXTERNAL_STR_MASK    = 0x04'00000000000000UL;
    static constexpr uint64_t VALUE_INLINE_STR_MASK      = 0x05'00000000000000UL;
    static constexpr uint64_t VALUE_NEGATIVE_INT_MASK    = 0x06'00000000000000UL;
    static constexpr uint64_t VALUE_POSITIVE_INT_MASK    = 0x07'00000000000000UL;

    // TODO: big numbers (> 2^56) not supported yet
    // static constexpr uint64_t VALUE_EXTERN_POS_INT_MASK  = 0x08'00000000000000UL;
    // static constexpr uint64_t VALUE_EXTERN_NEG_INT_MASK  = 0x09'00000000000000UL;

    static constexpr uint64_t VALUE_FLOAT_MASK           = 0x0A'00000000000000UL;
    static constexpr uint64_t VALUE_BOOL_MASK            = 0x0B'00000000000000UL;
    static constexpr uint64_t VALUE_PATH_MASK            = 0x0C'00000000000000UL;

    static_assert(VALUE_NEGATIVE_INT_MASK < VALUE_POSITIVE_INT_MASK,
                  "INLINED INTEGERS WON'T BE ORDERED PROPERTLY IN THE BPT.");

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
