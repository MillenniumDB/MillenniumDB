#ifndef BASE__BINDING_ID_H_
#define BASE__BINDING_ID_H_

#include <cstring>

#include "base/ids/object_id.h"
#include "base/ids/var_id.h"

class BindingId {
private:
    const std::size_t size;
    ObjectId* object_ids; // array

public:
    BindingId(const std::size_t size) :
        size       (size),
        object_ids (new ObjectId[size]) { }

    // to prevent using
    // auto b = op.begin()
    // instead of
    // auto& b = op.begin()
    BindingId(const BindingId& other) = delete;

    ~BindingId() {
        delete[] object_ids;
    }

    inline void add_all(BindingId& other) noexcept {
        std::memcpy(this->object_ids, other.object_ids, sizeof(ObjectId) * other.size);
    }

    inline void add(VarId var_id, ObjectId obj_id) noexcept {
        object_ids[var_id.id] = obj_id;
    }

    inline ObjectId operator[](VarId var_id) const noexcept {
        return object_ids[var_id.id];
    }

    inline std::size_t var_count() const noexcept {
        return size;
    }
};

#endif // BASE__BINDING_ID_H_
