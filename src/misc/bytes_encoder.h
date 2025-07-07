#pragma once

#include <cassert>
#include <cstdint>
#include <fstream>
#include <utility>

// utility for encoding/decoding bytes to/from disk
// for sizes we compress the number of bytes written
struct BytesEncoder {
    // write str into buffer and returns its size
    static inline uint64_t read_bytes(std::fstream& file, char* buffer)
    {
        uint64_t size { 0 };
        uint64_t shift_size { 0 };

        while (true) {
            char c;
            file.read(&c, 1);
            auto ptr = reinterpret_cast<unsigned char*>(&c);
            uint64_t b = *ptr;

            if (b <= 127) {
                size |= b << shift_size;
                break;
            } else {
                size |= (b & 0x7FUL) << shift_size;
            }
            shift_size += 7;
        }

        file.read(buffer, size);

        assert(size > 0);
        return size;
    }

    // reads size from buffer and returns { size, num_bytes_read }
    static inline std::pair<uint64_t, uint64_t> read_size(const char* buffer)
    {
        auto ptr = reinterpret_cast<const unsigned char*>(buffer);

        uint64_t size { 0 };
        uint64_t shift_size { 0 };
        uint64_t num_bytes_read { 0 };
        while (true) {
            const uint64_t byte = *ptr;
            ++ptr;
            ++num_bytes_read;

            if (byte <= 127) {
                size |= byte << shift_size;
                break;
            } else {
                size |= (byte & 0x7FUL) << shift_size;
            }

            shift_size += 7;
        }

        assert(size > 0);
        return { size, num_bytes_read };
    }

    // writes size into buffer and returns num_bytes_written
    static inline uint64_t write_size(char* buffer, uint64_t size)
    {
        assert(size > 0);

        auto* ptr = buffer;

        uint64_t remaining_bytes { size };
        uint64_t num_bytes_written { 0 };
        while (remaining_bytes != 0) {
            if (remaining_bytes <= 127) {
                *ptr = static_cast<char>(remaining_bytes);
            } else {
                *ptr = static_cast<char>(remaining_bytes & 0x7FUL) | 0x80;
            }
            remaining_bytes = remaining_bytes >> 7;
            ++ptr;
            ++num_bytes_written;
        }

        return num_bytes_written;
    }
};
