#pragma once

#include <functional>
#include <type_traits>

#include "storage/file_id.h"


struct TensorId {
    FileId file_id;
    uint32_t tensor_index;

    TensorId(FileId file_id_, uint32_t tensor_index_) :
        file_id { file_id_ },
        tensor_index { tensor_index_ } { }

    bool operator==(const TensorId& other) const {
        return file_id == other.file_id && tensor_index == other.tensor_index;
    }

    struct Hasher {
        uint64_t operator()(const TensorId k) const {
            return k.file_id.id | (k.tensor_index << 6);
        }
    };
};


template<>
struct std::hash<TensorId> {
    std::size_t operator()(TensorId const& k) const noexcept {
        return k.file_id.id | (k.tensor_index << 6);
    }
};


static_assert(std::is_trivially_copyable<TensorId>::value);