#pragma once

#include <atomic>
#include <cstdint>
#include <ostream>
#include <shared_mutex>
#include <string>

#include <boost/unordered/unordered_flat_map.hpp>

#include "macros/count_zeros.h"
#include "storage/file_id.h"
#include "storage/index/hash/strings_hash/strings_hash.h"

class StringManager {
public:
    static constexpr uint64_t MAX_STRING_SIZE = 1024 * 1024 * 64; // 64 MB

    static constexpr uint64_t BLOCK_SIZE = 1024 * 64; // 64 KB

    // we suppose no string will need more than these bytes to encode its length
    static constexpr size_t MAX_LEN_BYTES = 4;

    static_assert((1 << (MAX_LEN_BYTES * 7)) >= MAX_STRING_SIZE, "2^(7*MAX_LEN_BYTES) does not fit the MAX_STRING_SIZE");

    // minimum space left in a page to consider writing a new string in that page.
    // if the space left is less than this, the next string must start at the next page.
    // MAX_STRING_SIZE is used because that way we never need 2 blocks to get the string length
    static constexpr uint64_t MIN_PAGE_REMAINING_BYTES = MAX_LEN_BYTES;

    // The first portion of the string file that is always in memory
    static constexpr uint64_t DEFAULT_STATIC_BUFFER = 1ULL * 1024 * 1024 * 1024;

    // Buffer to manage the remaining of the string file (not in static buffer)
    static constexpr uint64_t DEFAULT_DYNAMIC_BUFFER = 1ULL * 1024 * 1024 * 1024;

    static_assert(MAX_STRING_SIZE % BLOCK_SIZE == 0);

    // necessary to be called before first usage
    // sizes in Bytes
    static void init(uint64_t static_buffer_size, uint64_t dynamic_buffer_size);

    StringManager(const StringManager&) = delete;

    StringManager(StringManager&&) = delete;

    ~StringManager();

    void print(std::ostream& os, uint64_t id);

    // returns the length of the data, assumes buffer is big enough
    uint64_t print_to_buffer(char* buffer, uint64_t id);

    uint64_t get_bytes_id(const char* bytes, uint64_t size);

    uint64_t get_str_id(const std::string& str)
    {
        return get_bytes_id(str.data(), str.size());
    }

    // !!! NOT THREAD-SAFE !!!
    uint64_t get_or_create(const char* bytes, uint64_t size);

    bool bytes_eq(const char* bytes, uint64_t size, uint64_t id);

    bool str_eq(const std::string& str, uint64_t string_id)
    {
        return bytes_eq(str.data(), str.size(), string_id);
    }

    static inline int compare(const char* lhs, const char* rhs, size_t lhs_size, size_t rhs_size)
    {
        unsigned char c1, c2;
        do {
            if (lhs_size == 0 || rhs_size == 0)
                return lhs_size - rhs_size;

            c1 = static_cast<unsigned char>(*lhs);
            c2 = static_cast<unsigned char>(*rhs);
            lhs++;
            rhs++;
            lhs_size--;
            rhs_size--;
        } while (c1 == c2);

        return c1 - c2;
    }

    // returns the encoded string length, and writes how many bytes were read
    static uint64_t get_string_len(char* ptr, uint64_t* bytes_for_len)
    {
        auto decode_ptr = reinterpret_cast<unsigned char*>(ptr);
        uint64_t decoded = 0;
        uint64_t shift_size = 0;
        *bytes_for_len = 0;
        while (true) {
            uint64_t b = *decode_ptr;
            decode_ptr++;
            (*bytes_for_len)++;

            if (b <= 127) {
                decoded |= b << shift_size;
                break;
            } else {
                decoded |= (b & 0x7FUL) << shift_size;
            }

            shift_size += 7;
        }
        return decoded;
    }

    // returns how many bytes are used to encode the length `str_len`.
    static inline uint64_t get_bytes_for_len(uint64_t str_len)
    {
        uint64_t significant_bits = 64 - MDB_COUNT_LEADING_ZEROS_64(str_len);
        return (significant_bits / 7) + (significant_bits % 7 != 0);
    }

    // assumes there is enough space in buffer to write the bytes of the encoded str_len
    // returns how many bytes are used to encode the length `str_len`
    static size_t write_encoded_strlen(char* buffer, uint64_t str_len)
    {
        size_t bytes_for_len = 0;
        auto* ptr = buffer;
        uint64_t remaining_len = str_len;
        while (remaining_len != 0) {
            if (remaining_len <= 127) {
                *ptr = static_cast<char>(remaining_len);
            } else {
                *ptr = static_cast<char>(remaining_len & 0x7FUL) | 0x80;
            }
            remaining_len = remaining_len >> 7;
            ptr++;
            bytes_for_len++;
        }
        return bytes_for_len;
    }

private:
    struct Frame {
        uint64_t block_id;

        char* bytes;

        // count of objects using this page
        std::atomic<uint32_t> pins;

        // used by the replacement policy
        bool second_chance;

        Frame() :
            block_id(0),
            bytes(nullptr),
            pins(0),
            second_chance(false)
        { }
    };

    char* static_buffer;

    uint64_t static_buffer_size;

    char* dynamic_buffer;

    Frame* frames;

    uint64_t frames_size;

    FileId str_file_id;

    uint64_t clock = 0;

    StringsHash str_hash;

    // for str_hash read/writes
    std::shared_mutex str_hash_mutex;

    std::mutex frame_mutex;

    // block_number => Frame*
    boost::unordered_flat_map<uint64_t, Frame*> frame_map;

    StringManager(uint64_t static_buffer_size, uint64_t dynamic_buffer_frames);

    // returns a block with a pinned frame
    Frame& get_block(uint64_t block_id);

    Frame& get_frame_available();
};

extern StringManager& string_manager; // global object
