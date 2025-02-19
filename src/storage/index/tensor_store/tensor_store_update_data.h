#pragma once

#include <cstdint>
#include <ostream>
#include <string>

/**
 * Helper struct for logging tensor store updates
 */
struct TensorStoreUpdateData {
    std::string tensor_store_name;

    bool created { false };

    uint_fast32_t inserted_tensors { 0 };
    uint_fast32_t overwritten_tensors { 0 };
    uint_fast32_t removed_tensors { 0 };

    friend std::ostream& operator<<(std::ostream& os, const TensorStoreUpdateData& data)
    {
        os << "{\"name\": \"" << data.tensor_store_name << "\"";
        os << ", \"created\": " << (data.created ? "true" : "false");
        os << ", \"inserted_tensors\": " << data.inserted_tensors;
        os << ", \"overwritten_tensors\": " << data.overwritten_tensors;
        os << ", \"removed_tensors\": " << data.removed_tensors;
        os << "}";
        return os;
    }
};