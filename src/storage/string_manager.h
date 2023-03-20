#pragma once

#include <atomic>
#include <cassert>
#include <memory>
#include <mutex>
#include <ostream>
#include <string>
#include <vector>

#include "execution/graph_object/char_iter.h"
#include "storage/file_id.h"
#include "storage/file_manager.h"
#include "storage/index/hash/strings_hash/strings_hash.h"
#include "third_party/robin_hood/robin_hood.h"

class CharIter;

class StringManager {
public:
    static constexpr size_t STRING_BLOCK_SIZE = 1024*1024*64; // 64 MB

    // metadata only is in the first block
    static constexpr size_t METADATA_SIZE = 8; // 8 bytes for end

    // necessary to be called before first usage
    static void init();
    ~StringManager();

    std::unique_ptr<CharIter> get_char_iter(uint64_t id) const;

    void print(std::ostream& os, uint64_t id) const;

    uint64_t get_str_id(const std::string&, bool create_if_not_exists = false);

    // must check the string doesn't exist before calling this method
    uint64_t create_new(const std::string&);

    bool str_eq(const std::string& str, uint64_t string_id) const;

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
    static size_t get_string_len(char* ptr, size_t* bytes_for_len) {
        auto decode_ptr = reinterpret_cast<unsigned char*>(ptr);
        size_t decoded = 0;
        size_t shift_size = 0;
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

    inline char* get_string_block(size_t block_number) const { return string_blocks[block_number]; }

private:
    StringManager();

    void append_new_block();

    inline void update_last_block_offset() {
        auto ptr = reinterpret_cast<uint64_t*>(string_blocks[0]);
        *ptr = last_block_offset;
    }

    FileId str_file_id;

    StringsHash str_hash;

    uint64_t last_block_offset;

    std::vector<char*> string_blocks;
};

extern StringManager& string_manager; // global object
