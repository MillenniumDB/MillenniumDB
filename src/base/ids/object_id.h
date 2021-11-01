#ifndef BASE__OBJECT_ID_H_
#define BASE__OBJECT_ID_H_

#include <cstdint>
#include <type_traits>

class ObjectId {
public:
    static constexpr uint64_t NULL_OBJECT_ID      = 0;
    static constexpr uint64_t OBJECT_ID_NOT_FOUND = UINT64_MAX;

    uint64_t id;

    explicit ObjectId(uint64_t id) :
        id(id) { }

    ObjectId() :
        id(NULL_OBJECT_ID) { } // initialized with NULL_OBJECT_ID

    ~ObjectId() = default;

    static inline ObjectId get_null()      noexcept { return ObjectId(NULL_OBJECT_ID); }
    static inline ObjectId get_not_found() noexcept { return ObjectId(OBJECT_ID_NOT_FOUND); }

    inline bool is_null()      const noexcept { return id == NULL_OBJECT_ID; }
    inline bool is_not_found() const noexcept { return id == OBJECT_ID_NOT_FOUND; }

    inline bool operator==(const ObjectId& rhs) const noexcept { return id == rhs.id; }
    inline bool operator!=(const ObjectId& rhs) const noexcept { return id != rhs.id; }
    inline bool operator<(const ObjectId& rhs)  const noexcept { return id <  rhs.id; }
    inline bool operator>(const ObjectId& rhs)  const noexcept { return id >  rhs.id; }
    inline bool operator<=(const ObjectId& rhs) const noexcept { return id <= rhs.id; }
    inline bool operator>=(const ObjectId& rhs) const noexcept { return id >= rhs.id; }
};

static_assert(std::is_trivially_copyable<ObjectId>::value);

#endif // BASE__OBJECT_ID_H_
