#ifndef BASE__VAR_ID_H_
#define BASE__VAR_ID_H_

#include <cstdint>
#include <type_traits>

class VarId {
public:
    uint_fast32_t id;

    explicit VarId(uint_fast32_t id)
        : id(id) { }

    ~VarId() = default;

    inline bool operator<(const VarId& rhs) const noexcept {
        return id < rhs.id;
    }

    inline bool operator<=(const VarId& rhs) const noexcept {
        return id <= rhs.id;
    }

    inline bool operator==(const VarId& rhs) const noexcept {
        return id == rhs.id;
    }

    inline bool operator!=(const VarId& rhs) const noexcept {
        return id != rhs.id;
    }
};

static_assert(std::is_trivially_copyable<VarId>::value);

#endif // BASE__VAR_ID_H_
