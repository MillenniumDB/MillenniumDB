#include "string_manager.h"

#include <cassert>
#include <fcntl.h>
#include <mutex>

#include "graph_models/object_id.h"
#include "macros/aligned_alloc.h"
#include "misc/bytes_encoder.h"
#include "misc/fatal_error.h"
#include "query/query_context.h"
#include "system/file_manager.h"

// memory for the object
static typename std::aligned_storage<sizeof(StringManager), alignof(StringManager)>::type string_manager_buf;
// global object
StringManager& string_manager = reinterpret_cast<StringManager&>(string_manager_buf);

void StringManager::init(uint64_t static_buffer_size, uint64_t dynamic_buffer_size)
{
    auto static_buffer_size_aligned = (static_buffer_size / BLOCK_SIZE) * BLOCK_SIZE; // To be multiple of BLOCK_SIZE
    new (&string_manager) StringManager(static_buffer_size_aligned, dynamic_buffer_size / BLOCK_SIZE); // placement new
}

StringManager::StringManager(uint64_t static_buffer_size, uint64_t dynamic_buffer_frames) :
    static_buffer(reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(static_buffer_size))),
    static_buffer_size(static_buffer_size),
    dynamic_buffer(reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(BLOCK_SIZE * dynamic_buffer_frames))),
    frames(new StringManager::Frame[dynamic_buffer_frames]),
    frames_size(dynamic_buffer_frames),
    str_file_id(file_manager.get_file_id(StringManager::STRINGS_FILENAME)),
    str_hash("str_hash")
{
    if (static_buffer == nullptr || dynamic_buffer == nullptr || frames == nullptr) {
        FATAL_ERROR("Could not allocate StringManager buffers, try using a smaller size");
    }

    uint64_t string_file_size = lseek(str_file_id.id, 0, SEEK_END);

    auto bytes_to_copy = std::min(string_file_size, static_buffer_size);

    #ifdef POSIX_FADV_NOREUSE
    posix_fadvise(str_file_id.id, 0, bytes_to_copy, POSIX_FADV_NOREUSE);
    #endif
    #ifdef POSIX_FADV_RANDOM
    posix_fadvise(str_file_id.id, bytes_to_copy, string_file_size - bytes_to_copy, POSIX_FADV_RANDOM);
    #endif

    // On linux pread won't return more than 0x7ffff000 (2,147,479,552) bytes read
    // so we need to iterate until the read is complete
    uint64_t offset = 0;
    while (bytes_to_copy != 0) {
        auto read_res = pread(str_file_id.id, static_buffer + offset, BLOCK_SIZE, offset);
        if (read_res == -1) {
            perror("read failed");
            FATAL_ERROR("Could not read StringManager static buffer");
        }
        bytes_to_copy -= read_res;
        offset += read_res;
    }

    for (uint64_t i = 0; i < frames_size; i++) {
        frames[i].bytes = dynamic_buffer + (i * BLOCK_SIZE);
    }
}

StringManager::~StringManager()
{
    MDB_ALIGNED_FREE(static_buffer);
    MDB_ALIGNED_FREE(dynamic_buffer);
    delete[] frames;
}

void StringManager::print(std::ostream& os, uint64_t id)
{
    uint64_t remaining;
    uint64_t current_block_number = id / BLOCK_SIZE;

    if (id < static_buffer_size) {
        char* ptr = static_buffer + id;
        const auto [len, bytes_for_len] = BytesEncoder::read_size(ptr);
        ptr += bytes_for_len;

        auto bytes_left_in_static_buffer = (static_buffer_size - (id + bytes_for_len));
        auto bytes_to_copy = std::min(bytes_left_in_static_buffer, len);
        os.write(ptr, bytes_to_copy);

        remaining = len - bytes_to_copy;

    } else {
        auto& first_block = get_block(current_block_number);
        auto offset = (id % BLOCK_SIZE);
        char* ptr = first_block.bytes + offset;

        const auto [len, bytes_for_len] = BytesEncoder::read_size(ptr);
        ptr += bytes_for_len;

        uint64_t bytes_after_len = BLOCK_SIZE - (offset + bytes_for_len);
        auto bytes_to_copy = std::min(bytes_after_len, len);
        os.write(ptr, bytes_to_copy);
        first_block.pins--;

        remaining = len - bytes_to_copy;
    }

    while (remaining > 0) {
        current_block_number++;
        auto& current_block = get_block(current_block_number);
        auto bytes_to_copy = std::min(remaining, BLOCK_SIZE);
        os.write(current_block.bytes, bytes_to_copy);
        current_block.pins--;

        remaining -= bytes_to_copy;
    }
}

uint64_t StringManager::print_to_buffer(char* buffer, uint64_t id)
{
    uint64_t res;

    uint64_t remaining;
    uint64_t current_block_number = id / BLOCK_SIZE;

    if (id < static_buffer_size) {
        char* ptr = static_buffer + id;
        const auto [len, bytes_for_len] = BytesEncoder::read_size(ptr);
        res = len;
        ptr += bytes_for_len;

        auto bytes_left_in_static_buffer = (static_buffer_size - (id + bytes_for_len));
        auto bytes_to_copy = std::min(bytes_left_in_static_buffer, len);
        memcpy(buffer, ptr, bytes_to_copy);
        buffer += bytes_to_copy;

        remaining = len - bytes_to_copy;

    } else {
        auto& first_block = get_block(current_block_number);
        auto offset = (id % BLOCK_SIZE);
        char* ptr = first_block.bytes + offset;

        const auto [len, bytes_for_len] = BytesEncoder::read_size(ptr);
        res = len;
        ptr += bytes_for_len;

        uint64_t bytes_after_len = BLOCK_SIZE - (offset + bytes_for_len);
        auto bytes_to_copy = std::min(bytes_after_len, len);
        memcpy(buffer, ptr, bytes_to_copy);
        first_block.pins--;

        remaining = len - bytes_to_copy;
        buffer += bytes_to_copy;
    }

    while (remaining > 0) {
        current_block_number++;
        auto& current_block = get_block(current_block_number);
        auto bytes_to_copy = std::min(remaining, BLOCK_SIZE);
        memcpy(buffer, current_block.bytes, bytes_to_copy);
        current_block.pins--;

        remaining -= bytes_to_copy;
        buffer += bytes_to_copy;
    }

    return res;
}

bool StringManager::bytes_eq(const char* bytes, uint64_t size, uint64_t id)
{
    auto buffer = get_query_ctx().get_buffer1();
    uint64_t str_len = print_to_buffer(buffer, id);

    if (str_len != size) {
        return false;
    }

    return StringManager::compare(buffer, bytes, str_len, size) == 0;
}

uint64_t StringManager::get_bytes_id(const char* bytes, uint64_t size)
{
    std::shared_lock lock(str_hash_mutex);

    // this may end up calling StringManager::bytes_eq multiple times
    return str_hash.get_str_id(bytes, size);
}

// IMPORTANT: supposing only one thread will call this method at a time
uint64_t StringManager::get_or_create(const char* str, uint64_t str_len)
{
    {
        std::shared_lock lock(str_hash_mutex);
        auto existing_id = str_hash.get_str_id(str, str_len);
        if (existing_id != ObjectId::MASK_NOT_FOUND) {
            return existing_id;
        }
    }
    // need to create a new ID

    // changes on disk are done immediately
    char len_buf[MIN_PAGE_REMAINING_BYTES] = {0,0,0,0};

    uint64_t old_file_size = lseek(str_file_id.id, 0, SEEK_END);

    size_t remaining_in_block = BLOCK_SIZE - (old_file_size % StringManager::BLOCK_SIZE);

    if (remaining_in_block < MIN_PAGE_REMAINING_BYTES) {
        auto write_res = write(str_file_id.id, len_buf, remaining_in_block);
        if (write_res == -1) {
            throw std::runtime_error("Could not write into file");
        }
        old_file_size += remaining_in_block;
    }

    auto new_id = old_file_size;

    const auto bytes_for_len = BytesEncoder::write_size(len_buf, str_len);

    auto write_res = write(str_file_id.id, len_buf, bytes_for_len);
    if (write_res == -1) {
        throw std::runtime_error("Could not write into string file");
    }

    write_res = write(str_file_id.id, str, str_len);
    if (write_res == -1) {
        throw std::runtime_error("Could not write into string file");
    }

    uint64_t remaining = str_len;
    uint64_t current_block_number = new_id / BLOCK_SIZE;

    auto* str_ptr = str;

    if (old_file_size < static_buffer_size) {
        // some part of the str fits in static buffer
        char* ptr = static_buffer + new_id;
        memcpy(ptr, len_buf, bytes_for_len);
        ptr += bytes_for_len;

        const auto bytes_left_in_static_buffer = static_buffer_size - (new_id + bytes_for_len);
        const auto bytes_to_copy = std::min(bytes_left_in_static_buffer, str_len);
        memcpy(ptr, str_ptr, bytes_to_copy);

        remaining -= bytes_to_copy;
        str_ptr += bytes_to_copy;
    } else {
        // no data fits in the static buffer
        auto& first_block = get_block(current_block_number);
        const auto offset = new_id % BLOCK_SIZE;
        char* ptr = first_block.bytes + offset;

        memcpy(ptr, len_buf, bytes_for_len);
        ptr += bytes_for_len;

        const auto bytes_after_len = BLOCK_SIZE - (offset + bytes_for_len);
        const auto bytes_to_copy = std::min(bytes_after_len, str_len);
        memcpy(ptr, str_ptr, bytes_to_copy);
        first_block.pins--;

        remaining -= bytes_to_copy;
        str_ptr += bytes_to_copy;
    }

    while (remaining > 0) {
        // handle remaining bytes
        ++current_block_number;
        auto& current_block = get_block(current_block_number);
        const auto bytes_to_copy = std::min(remaining, BLOCK_SIZE);
        memcpy(current_block.bytes, str_ptr, bytes_to_copy);
        current_block.pins--;

        remaining -= bytes_to_copy;
        str_ptr += bytes_to_copy;
    }

    std::unique_lock lock(str_hash_mutex);
    str_hash.create_str_id(str, str_len, new_id);

    return new_id;
}

StringManager::Frame& StringManager::get_frame_available()
{
    Frame* frame;
    do {
        clock++;
        clock = clock < frames_size ? clock : 0;

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

StringManager::Frame& StringManager::get_block(uint64_t block_id)
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

        const auto read_res = pread(str_file_id.id, frame.bytes, BLOCK_SIZE, block_id * BLOCK_SIZE);
        if (read_res == -1) {
            throw std::runtime_error("Could not read string file block");
        }

        return frame;
    } else {
        Frame* frame = it->second;
        frame->pins++;
        frame->second_chance = true;

        return *frame;
    }
}
