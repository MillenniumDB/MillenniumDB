#pragma once

#include <cstring>
#include <cassert>
#include <utility>

#include "graph_models/object_id.h"
#include "query/var_id.h"

class Binding {
private:
    ObjectId* object_ids; // array

public:
    std::size_t size;

    Binding() : object_ids(nullptr), size(0) { }

    Binding(std::size_t size) : object_ids(new ObjectId[size]), size(size) {
        for (std::size_t i = 0; i < size; i++) {
            object_ids[i] = ObjectId::get_null();
        }
    }

    Binding(const Binding& other) = delete;

    ~Binding() {
        delete[] object_ids;
    }

    void operator=(Binding&& other) {
        assert(this->object_ids == nullptr);
        size = other.size;
        this->object_ids = std::exchange(other.object_ids, nullptr);
    }

    bool operator==(const Binding& other) const noexcept {
        if (size != other.size) {
            return false;
        }
        for (std::size_t i = 0; i < size; i++) {
            if ((*this)[VarId(i)] != other[VarId(i)])
                return false;
        }
        return true;
    }

    inline void add_all(const Binding& other) noexcept {
        assert(this->size == other.size);
        std::memcpy(this->object_ids, other.object_ids, sizeof(ObjectId) * other.size);
    }

    inline void add(VarId var_id, ObjectId obj_id) noexcept {
        object_ids[var_id.id] = obj_id;
    }

    inline ObjectId operator[](VarId var_id) const noexcept {
        return object_ids[var_id.id];
    }
};
