#pragma once

#include <cstdint>
#include <memory>
#include <ostream>
#include <shared_mutex>
#include <string>
#include <vector>

#include "macros/count_zeros.h"
#include "storage/char_iter.h"
#include "storage/file_id.h"
#include "storage/index/hash/strings_hash/strings_hash.h"

class CharIter;

class StringManager {
public:
    static constexpr uint64_t STRING_BLOCK_SIZE = 1024*1024*64; // 64 MB

    // metadata only is in the first block
    static constexpr uint64_t METADATA_SIZE = 8; // 8 bytes for last_block_offset

    // minimum space left in a page to consider writing a new string in that page.
    // if the space left is less than this, the next string must start at the next page.
    static constexpr uint64_t MIN_PAGE_REMAINING_BYTES = 9;

    static constexpr uint64_t DEFAULT_LOAD_STR = 2ULL * 1024 * 1024 * 1024;

    // necessary to be called before first usage
    // max_initial_populate_size in Bytes
    static void init(uint64_t max_initial_populate_size);

    ~StringManager();

    std::unique_ptr<CharIter> get_char_iter(uint64_t id) const;

    void print(std::ostream& os, uint64_t id) const;

    // returns the length of the data, assumes buffer is big enough
    uint64_t print_to_buffer(char* buffer, uint64_t id);

    uint64_t get_bytes_id(const char* bytes, uint64_t size) const;

    uint64_t get_str_id(const std::string& str) const {
        return get_bytes_id(str.data(), str.size());
    }

    // !!! NOT THREAD-SAFE !!!
    uint64_t get_or_create(const char* bytes, uint64_t size);

    bool bytes_eq(const char* bytes, uint64_t size, uint64_t id) const;

    bool str_eq(const std::string& str, uint64_t string_id) const {
        return bytes_eq(str.data(), str.size(), string_id);
    }

    static inline int compare(CharIter& lhs, CharIter& rhs) {
        unsigned char c1, c2;
        do {
            c1 = static_cast<unsigned char>(lhs.next_char());
            c2 = static_cast<unsigned char>(rhs.next_char());
            if (c1 == '\0')
                return c1 - c2;
        } while (c1 == c2);

        return c1 - c2;
    }

    // returns the encoded string length, and writes how many bytes were read
    static uint64_t get_string_len(char* ptr, uint64_t* bytes_for_len) {
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
    static inline uint64_t get_bytes_for_len(uint64_t str_len) {
        uint64_t significant_bits = 64 - MDB_COUNT_LEADING_ZEROS_64(str_len);
        return (significant_bits / 7) + (significant_bits % 7 != 0);
    }

    // assumes there is enough space in buffer to write the bytes of the encoded str_len
    // returns how many bytes are used to encode the length `str_len`
    static size_t write_encoded_strlen(char* buffer, uint64_t str_len) {
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

    inline char* get_string_block(uint64_t block_number) const { return string_blocks[block_number]; }

    inline uint64_t get_last_block_offset() { return last_block_offset; }

    inline uint64_t get_total_blocks() { return string_blocks.size(); }

private:
    StringManager(uint64_t max_initial_populate_size);

    void append_new_block();

    inline void update_last_block_offset() {
        auto ptr = reinterpret_cast<uint64_t*>(string_blocks[0]);
        *ptr = last_block_offset;
    }

    FileId str_file_id;

    StringsHash str_hash;

    uint64_t last_block_offset;

    std::vector<char*> string_blocks;

    // for str_hash read/writes
    mutable std::shared_mutex mutex;
};

extern StringManager& string_manager; // global object
