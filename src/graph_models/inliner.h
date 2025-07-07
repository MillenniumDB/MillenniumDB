#pragma once

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <ostream>

class Inliner {
public:
    // assumes null-terminated string
    static uint64_t inline_string(const char* str) {
        uint64_t res = 0;
        int shift_size = 8*6;
        for (const char* i = str; *i != '\0'; i++) {
            // MUST convert to uint8_t and then to uint64_t.
            // Shift with shift_size >=32 is undefined behaviour.
            uint8_t byte = *i;
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return res;
    }

    // Used 5 bytes on RDF literals with datatype or language (this attributes uses 2 additional bytes)
    static uint64_t inline_string5(const char* str) {
        uint64_t res = 0;
        int shift_size = 8*4;
        for (const char* i = str; *i != '\0'; i++) {
            // MUST convert to uint8_t and then to uint64_t.
            // Shift with shift_size >=32 is undefined behaviour.
            uint8_t byte = *i;
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return res;
    }

    static uint64_t inline_iri(const char* str) {
        uint64_t res = 0;
        int shift_size = 8*5;
        for (const char* i = str; *i != '\0' && shift_size >= 0; i++) {
            // MUST convert to uint8_t and then to uint64_t.
            // Shift with shift_size >=32 is undefined behaviour.
            uint8_t byte = *i;
            uint64_t byte64 = static_cast<uint64_t>(byte);
            res |= byte64 << shift_size;
            shift_size -= 8;
        }
        return res;
    }

    template <uint_fast32_t N>
    static uint64_t decode(uint64_t val) {
        static_assert(N > 0 && N <= 7); // max inlined bytes is 7
        uint64_t res = 0; // Ensure null-termination.
        uint8_t* c = reinterpret_cast<uint8_t*>(&res);
        int shift_size = (N - 1) * 8;
        for (uint_fast32_t i = 0; i < N; i++) {
            uint8_t byte = (val >> shift_size) & 0xFF;
            c[i]         = byte;
            shift_size  -= 8;
        }
        return res;
    }

    // transforms the least N significant bytes of id into a string
    template <uint_fast32_t N>
    static std::string get_string_inlined(uint64_t id) {
        char buff[N];
        int suffix_shift_size = 8 * (N - 1);

        uint_fast32_t str_len = N;
        for (uint_fast32_t i = 0; i < N; i++) {
            uint8_t byte = (id >> suffix_shift_size) & 0xFF;
            char c = byte;
            if (c == '\0') {
                str_len = i;
                break;
            }
            buff[i] = c;
            suffix_shift_size -= 8;
        }

        return std::string(buff, str_len);
    }

    // transforms the least N significant bytes of id into a string
    template <uint_fast32_t N>
    static void print_string_inlined(std::ostream& os, uint64_t id) {
        char buff[N];
        int suffix_shift_size = 8 * (N - 1);

        uint_fast32_t str_len = N;
        for (uint_fast32_t i = 0; i < N; i++) {
            uint8_t byte = (id >> suffix_shift_size) & 0xFF;
            char c = byte;
            if (c == '\0') {
                str_len = i;
                break;
            }
            buff[i] = c;
            suffix_shift_size -= 8;
        }

        os.write(buff, str_len);
    }

    // transforms the least N significant bytes of id into a string
    template <uint_fast32_t N>
    static size_t print_string_inlined(char* out, uint64_t id) {
        int suffix_shift_size = 8 * (N - 1);

        size_t str_len = N;
        for (uint_fast32_t i = 0; i < N; i++) {
            uint8_t byte = (id >> suffix_shift_size) & 0xFF;
            char c = byte;
            if (c == '\0') {
                str_len = i;
                break;
            }
            out[i] = c;
            suffix_shift_size -= 8;
        }

        return str_len;
    }
};
