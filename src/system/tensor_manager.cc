#include "tensor_manager.h"

#include "graph_models/rdf_model/rdf_object_id.h"
#include "macros/aligned_alloc.h"
#include "misc/bytes_encoder.h"
#include "misc/fatal_error.h"
#include "system/file_manager.h"

#include <cassert>
#include <fcntl.h>

// memory for object
static typename std::aligned_storage<sizeof(TensorManager), alignof(TensorManager)>::type tensor_manager_buf;

// global object
TensorManager& tensor_manager = reinterpret_cast<TensorManager&>(tensor_manager_buf);

void TensorManager::init(uint64_t static_buffer_size, uint64_t dynamic_buffer_size)
{
    const auto aligned_static_buffer_size = (static_buffer_size / BLOCK_SIZE) * BLOCK_SIZE;
    const auto aligned_dynamic_buffer_size = (dynamic_buffer_size / BLOCK_SIZE) * BLOCK_SIZE;
    new (&tensor_manager) TensorManager(aligned_static_buffer_size, aligned_dynamic_buffer_size);
}

TensorManager::TensorManager(uint64_t aligned_static_buffer_size, uint64_t aligned_dynamic_buffer_size) :
    static_buffer_size { aligned_static_buffer_size },
    dynamic_buffer_size { aligned_dynamic_buffer_size },
    num_frames { dynamic_buffer_size / BLOCK_SIZE },
    tensor_file_id { file_manager.get_file_id(TensorManager::TENSORS_FILENAME) },
    static_buffer { reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(static_buffer_size)) },
    dynamic_buffer { reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(dynamic_buffer_size)) },
    frames { new TensorManager::Frame[num_frames] }
{
    if (static_buffer == nullptr || dynamic_buffer == nullptr || frames == nullptr) {
        FATAL_ERROR("Could not allocate TensorManager buffers, try using a smaller size");
    }

    const std::size_t tensor_file_size = lseek(tensor_file_id.id, 0, SEEK_END);
    std::size_t remaining = std::min(tensor_file_size, static_buffer_size);

#ifdef POSIX_FADV_NOREUSE
    posix_fadvise(tensor_file_id.id, 0, remaining, POSIX_FADV_NOREUSE);
#endif
#ifdef POSIX_FADV_RANDOM
    posix_fadvise(tensor_file_id.id, remaining, tensor_file_size - remaining, POSIX_FADV_RANDOM);
#endif

    // on linux pread reads at most 0x7ffff000 (2,147,479,552 bytes). For safety we read by chunks of 1 GB
    constexpr std::size_t MAX_READ_SIZE = 1024UL * 1024 * 1024; // 1 GB

    std::size_t offset = 0;
    while (remaining > 0) {
        const auto read_size = std::min(MAX_READ_SIZE, remaining);
        const auto read_res = pread(tensor_file_id.id, static_buffer + offset, read_size, offset);
        if (read_res == -1) {
            perror("read failed");
            FATAL_ERROR("Could not read tensor static buffer");
        }
        remaining -= read_res;
        offset += read_res;
    }

    for (uint64_t i = 0; i < num_frames; ++i) {
        frames[i].bytes = dynamic_buffer + (i * BLOCK_SIZE);
    }
}

TensorManager::~TensorManager()
{
    MDB_ALIGNED_FREE(static_buffer);
    MDB_ALIGNED_FREE(dynamic_buffer);
    delete[] frames;
}

uint64_t TensorManager::get_id(const char* bytes, std::size_t num_bytes)
{
    std::shared_lock lock(tensors_hash_mutex);

    return tensors_hash.get_bytes_id(bytes, num_bytes);
}

uint64_t TensorManager::create_id(const char* bytes, std::size_t num_bytes)
{
    return create_bytes_id(bytes, num_bytes);
}

uint64_t TensorManager::get_or_create_id(const char* bytes, std::size_t num_bytes)
{
    const uint64_t existing_tensor_id = get_id(bytes, num_bytes);
    if (existing_tensor_id != ObjectId::MASK_NOT_FOUND) {
        return existing_tensor_id;
    }

    return create_bytes_id(bytes, num_bytes);
}

template<typename T>
tensor::Tensor<T> TensorManager::get_tensor(ObjectId tensor_oid)
{
#ifndef NDEBUG
    const auto sub_t = RDF_OID::get_generic_sub_type(tensor_oid);
    switch (sub_t) {
    case RDF_OID::GenericSubType::TENSOR_FLOAT: {
        assert((std::is_same_v<T, float>) &&"T must be float");
        break;
    }
    case RDF_OID::GenericSubType::TENSOR_DOUBLE: {
        assert((std::is_same_v<T, double>) &&"T must be double");
        break;
    }
    default: {
        assert(false && "invalid get_tensor call");
        break;
    }
    }
#endif

    tensor::Tensor<T> res;
    char* buffer;

    const uint64_t tensor_id = tensor_oid.get_value();

    uint64_t current_block_id = tensor_id / BLOCK_SIZE;

    std::size_t remaining;

    if (tensor_id < static_buffer_size) {
        char* ptr = static_buffer + tensor_id;
        const auto [size, num_bytes_read] = BytesEncoder::read_size(ptr);
        ptr += num_bytes_read;

        assert(size % sizeof(T) == 0 && "size must be multiple of sizeof(T)");

        res.resize(size / sizeof(T));
        buffer = reinterpret_cast<char*>(res.data());

        const auto bytes_left = static_buffer_size - (tensor_id + num_bytes_read);

        const auto bytes_to_copy = std::min(bytes_left, size);
        std::memcpy(buffer, ptr, bytes_to_copy);
        buffer += bytes_to_copy;

        remaining = size - bytes_to_copy;
    } else {
        auto& first_block = get_frame(current_block_id);
        const auto offset = tensor_id % BLOCK_SIZE;
        char* ptr = first_block.bytes + offset;
        const auto [size, num_bytes_read] = BytesEncoder::read_size(ptr);

        assert(size % sizeof(T) == 0 && "size must be multiple of sizeof(T)");

        res.resize(size / sizeof(T));
        buffer = reinterpret_cast<char*>(res.data());

        const auto bytes_left = BLOCK_SIZE - (offset + num_bytes_read);
        const auto bytes_to_copy = std::min(bytes_left, size);
        std::memcpy(buffer, ptr, bytes_to_copy);
        --first_block.pins;
        buffer += bytes_to_copy;

        remaining = size - bytes_to_copy;
    }

    while (remaining > 0) {
        ++current_block_id;
        auto& current_block = get_frame(current_block_id);
        const auto bytes_to_copy = std::min(remaining, BLOCK_SIZE);
        std::memcpy(buffer, current_block.bytes, bytes_to_copy);
        --current_block.pins;
        buffer += bytes_to_copy;

        remaining -= bytes_to_copy;
    }

    return res;
}

bool TensorManager::bytes_eq(const char* bytes, std::size_t num_bytes, uint64_t tensor_id)
{
    uint64_t current_block_id = tensor_id / BLOCK_SIZE;

    std::size_t remaining;

    if (tensor_id < static_buffer_size) {
        char* ptr = static_buffer + tensor_id;
        const auto [size, num_bytes_read] = BytesEncoder::read_size(ptr);

        if (size != num_bytes) {
            return false;
        }

        ptr += num_bytes_read;

        const auto bytes_left = static_buffer_size - (tensor_id + num_bytes_read);

        const auto bytes_to_cmp = std::min(bytes_left, size);
        const auto cmp = std::memcmp(bytes, ptr, bytes_to_cmp);
        if (cmp != 0) {
            return false;
        }

        bytes += bytes_to_cmp;
        remaining = size - bytes_to_cmp;
    } else {
        auto& first_block = get_frame(current_block_id);
        const auto offset = tensor_id % BLOCK_SIZE;
        char* ptr = first_block.bytes + offset;
        const auto [size, num_bytes_read] = BytesEncoder::read_size(ptr);

        if (size != num_bytes) {
            return false;
        }

        ptr += num_bytes_read;

        const auto bytes_left = BLOCK_SIZE - (tensor_id + num_bytes_read);

        const auto bytes_to_cmp = std::min(bytes_left, size);
        const auto cmp = std::memcmp(bytes, ptr, bytes_to_cmp);
        if (cmp != 0) {
            return false;
        }

        --first_block.pins;
        bytes += bytes_to_cmp;

        remaining = size - bytes_to_cmp;
    }

    while (remaining > 0) {
        ++current_block_id;
        auto& current_block = get_frame(current_block_id);

        const auto bytes_to_cmp = std::min(remaining, BLOCK_SIZE);
        const auto cmp = std::memcmp(bytes, current_block.bytes, bytes_to_cmp);
        if (cmp != 0) {
            return false;
        }

        --current_block.pins;
        bytes += bytes_to_cmp;

        remaining -= bytes_to_cmp;
    }

    return true;
}

TensorManager::Frame& TensorManager::get_frame_available()
{
    Frame* frame;
    do {
        clock = (clock + 1) % num_frames;

        frame = &frames[clock];

        if (frame->pins != 0) {
            continue;
        }

        if (frame->second_chance) {
            frame->second_chance = false;
            continue;
        }

        return *frame;
    } while (true);
}

TensorManager::Frame& TensorManager::get_frame(uint64_t block_id)
{
    std::unique_lock<std::mutex> lock(frame_mutex);

    auto it = frame_map.find(block_id);
    if (it == frame_map.end()) {
        auto& frame = get_frame_available();

        if (frame.block_id != 0) {
            frame_map.erase(frame.block_id);
        }

        frame.pins = 1;
        frame.block_id = block_id;
        frame.second_chance = true;

        frame_map.emplace(block_id, &frame);

        const auto read_res = pread(tensor_file_id.id, frame.bytes, BLOCK_SIZE, block_id * BLOCK_SIZE);
        if (read_res == -1) {
            throw std::runtime_error("Could not read tensor file block");
        }

        return frame;
    } else {
        Frame* frame = it->second;
        ++(frame->pins);
        frame->second_chance = true;

        return *frame;
    }
}

uint64_t TensorManager::create_bytes_id(const char* bytes, uint64_t num_bytes)
{
    char size_buf[MAX_SIZE_BYTES] = { 0, 0, 0, 0 };

    std::size_t old_file_size = lseek(tensor_file_id.id, 0, SEEK_END);

    const std::size_t remaining_in_block = BLOCK_SIZE - (old_file_size % BLOCK_SIZE);

    if (remaining_in_block < MIN_PAGE_REMAINING_BYTES) {
        // there is not enought space in current page, just fill it up to next page
        const auto write_res = write(tensor_file_id.id, size_buf, remaining_in_block);
        if (write_res == -1) {
            throw std::runtime_error("Could not write into tensor file");
        }
        old_file_size += remaining_in_block;
    }

    const uint64_t new_id = old_file_size;

    const auto num_bytes_written = BytesEncoder::write_size(size_buf, num_bytes);
    auto write_res = write(tensor_file_id.id, size_buf, num_bytes_written);
    if (write_res == -1) {
        throw std::runtime_error("Could not write into tensor file");
    }

    write_res = write(tensor_file_id.id, bytes, num_bytes);
    if (write_res == -1) {
        throw std::runtime_error("Could not write into tensor file");
    }

    // load into buffer
    std::size_t remaining = num_bytes;
    uint64_t current_block_id = new_id / BLOCK_SIZE;
    auto* bytes_ptr = bytes;

    if (old_file_size < static_buffer_size) {
        // some part of the bytes fits in static buffer
        char* ptr = static_buffer + new_id;
        std::memcpy(ptr, size_buf, num_bytes_written);
        ptr += num_bytes_written;

        const auto bytes_left = static_buffer_size - (new_id + num_bytes_written);
        const auto bytes_to_copy = std::min(bytes_left, num_bytes);
        std::memcpy(ptr, bytes, bytes_to_copy);

        remaining -= bytes_to_copy;
        bytes_ptr += bytes_to_copy;
    } else {
        // no data fits in the static buffer
        auto& first_block = get_frame(current_block_id);
        const auto offset = new_id % BLOCK_SIZE;
        char* ptr = first_block.bytes + offset;

        std::memcpy(ptr, size_buf, num_bytes_written);
        ptr += num_bytes_written;

        const auto bytes_left = BLOCK_SIZE - (offset + num_bytes_written);
        const auto bytes_to_copy = std::min(bytes_left, num_bytes);
        std::memcpy(ptr, bytes_ptr, bytes_to_copy);
        --first_block.pins;

        remaining -= bytes_to_copy;
        bytes_ptr += bytes_to_copy;
    }

    while (remaining > 0) {
        ++current_block_id;
        auto& current_block = get_frame(current_block_id);
        const auto bytes_to_copy = std::min(remaining, BLOCK_SIZE);
        std::memcpy(current_block.bytes, bytes_ptr, bytes_to_copy);
        --current_block.pins;

        remaining -= bytes_to_copy;
        bytes_ptr += bytes_to_copy;
    }

    std::unique_lock lock(tensors_hash_mutex);
    tensors_hash.create_bytes_id(bytes, num_bytes, new_id);

    return new_id;
}

template tensor::Tensor<float> TensorManager::get_tensor<float>(ObjectId);
template tensor::Tensor<double> TensorManager::get_tensor<double>(ObjectId);
