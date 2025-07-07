#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <shared_mutex>

#include <boost/unordered_map.hpp>

#include "graph_models/common/datatypes/tensor/tensor.h"
#include "storage/file_id.h"
#include "storage/index/hash/tensors_hash/tensors_hash.h"

class TensorManager {
public:
    static constexpr char TENSORS_FILENAME[] = "tensors.dat";

    static constexpr uint64_t MAX_TENSOR_BYTES = 1024 * 1024 * 64; // 64 MB

    static constexpr std::size_t BLOCK_SIZE = 1024 * 64; // 64 KB

    static constexpr std::size_t MAX_SIZE_BYTES = 4; // maximum bytes for number tensor bytes

    // if page has less than this value, the tensor will be written into the next page,
    // in order to prevent reading its size between two pages
    static constexpr std::size_t MIN_PAGE_REMAINING_BYTES = MAX_SIZE_BYTES;

    static_assert(
        (1 << (MAX_SIZE_BYTES * 7)) >= MAX_TENSOR_BYTES,
        "2 ^ (7 * MAX_LEN_BYTES) does not fit in MAX_TENSOR_BYTES"
    );

    static_assert(MIN_PAGE_REMAINING_BYTES == MAX_SIZE_BYTES, "Pages must fit at least the the size");

    // first portion of tensor file that will be in-memory
    static constexpr uint64_t DEFAULT_STATIC_BUFFER = 1ULL * 1024 * 1024 * 1024; // 1 GB

    // buffer to manage the rest of the tensor file
    static constexpr uint64_t DEFAULT_DYNAMIC_BUFFER = 1ULL * 1024 * 1024 * 1024; // 1 GB

    static_assert(MAX_TENSOR_BYTES % BLOCK_SIZE == 0);

    static void init(uint64_t aligned_static_buffer_size, uint64_t aligned_dynamic_buffer_size);

    TensorManager(const TensorManager&) = delete;
    TensorManager& operator=(const TensorManager&) = delete;

    ~TensorManager();

    uint64_t get_id(const char* bytes, std::size_t num_bytes);

    uint64_t create_id(const char* bytes, std::size_t num_bytes);

    uint64_t get_or_create_id(const char* bytes, std::size_t num_bytes);

    template<typename T>
    tensor::Tensor<T> get_tensor(ObjectId tensor_oid);

    // check if the bytes equals to the stored tensor_id
    bool bytes_eq(const char* bytes, std::size_t num_bytes, uint64_t tensor_id);

private:
    struct Frame {
        uint64_t block_id;

        char* bytes;

        std::atomic<uint32_t> pins;

        bool second_chance;

        Frame() :
            block_id { 0 },
            bytes { nullptr },
            pins { 0 },
            second_chance { false }
        { }
    };

    std::size_t clock { 0 };

    std::size_t static_buffer_size;
    std::size_t dynamic_buffer_size;
    std::size_t num_frames;

    std::mutex frame_mutex;

    std::shared_mutex tensors_hash_mutex;

    FileId tensor_file_id;

    char* static_buffer;
    char* dynamic_buffer;
    Frame* frames;

    boost::unordered_map<uint64_t, Frame*> frame_map;

    TensorsHash tensors_hash;

    TensorManager(uint64_t static_buffer_size, uint64_t dynamic_buffer_size);

    Frame& get_frame_available();

    Frame& get_frame(uint64_t block_id);

    uint64_t create_bytes_id(const char* bytes, uint64_t num_bytes);
};

extern TensorManager& tensor_manager; // global object
