#pragma once


#include <atomic>
#include <cassert>
#include <cstdint>

#include "tensor_id.h"


class VTensor {
    friend class TensorStore;

public:
    // Get the start memory position of size() allocated bytes
    inline float* get_tensor() const noexcept {
        return tensor;
    }

    // Containes fiel_id and tensor_index of this page
    TensorId tensor_id;

private:
    VTensor* next_version;

    VTensor* prev_version;

    uint64_t version_number;

    // Start memory address of the tensor
    float* tensor;

    // Count of objects using this page
    std::atomic<uint32_t> pins;

    // Used by the replacement policy by the TensorManager
    bool second_chance;

    // True if data in memory is different from disk
    bool dirty;

    VTensor() noexcept :
        tensor_id { FileId(FileId::UNASSIGNED), 0 },
        next_version { nullptr },
        prev_version { nullptr },
        tensor { nullptr },
        pins { 0 },
        second_chance { false },
        dirty { false } { }

    void pin() noexcept {
        ++pins;
        second_chance = true;
    }

    void unpin() noexcept {
        assert(pins > 0 && "Cannot unpin if pin count is 0");
        --pins;
    }

    void set_tensor(float* tensor) {
        this->tensor = tensor;
    }

    void reassign(TensorId tensor_id) noexcept {
        assert(!dirty && "Cannot reassign VTensor if it is dirty");
        assert(pins == 0 && "Cannot reassign VTensor if it is pinned");
        assert(second_chance == false && "Should not reassign VTensor if usage is not 0");

        this->tensor_id = tensor_id;
        this->pins = 1;
        this->second_chance = true;
    }

    // this does not pin the tensor, necessary for tensor versioning
    void reassign_tensor_id(TensorId tensor_id) noexcept {
        assert(!dirty && "Cannot reassign VTensor if it is dirty");
        assert(pins == 0 && "Cannot reassign VTensor if it is pinned");
        assert(second_chance == false && "Should not reassign VTensor if usage is not 0");

        this->tensor_id = tensor_id;
    }
};