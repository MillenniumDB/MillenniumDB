#include "iri_compression.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace HexCompression {

inline bool is_hex_upper(char byte)
{
    return (('A' <= byte && byte <= 'F') || ('0' <= byte && byte <= '9'));
}

inline bool is_hex_lower(char byte)
{
    return (('a' <= byte && byte <= 'f') || ('0' <= byte && byte <= '9'));
}

size_t get_lower_hex_length(const char* str, size_t str_len)
{
    size_t hex_length = 0;
    for (int i = str_len - 1; i >= 0; --i) {
        if (is_hex_lower(str[i])) {
            hex_length++;
            if (hex_length >= MAX_HEX_LEN_TO_COMPRESS) {
                break;
            }
        } else {
            break;
        }
    }
    return hex_length;
}

size_t get_upper_hex_length(const char* str, size_t str_len)
{
    size_t hex_length = 0;
    for (int i = str_len - 1; i >= 0; --i) {
        if (is_hex_upper(str[i])) {
            hex_length++;
            if (hex_length >= MAX_HEX_LEN_TO_COMPRESS) {
                break;
            }
        } else {
            break;
        }
    }
    return hex_length;
}

size_t compress(const char* iri, size_t iri_size, size_t uncompressed_hex_len, char* result)
{
    int compressed_hex_len = uncompressed_hex_len / 2;

    // result+1 because we use 1 byte to store the length of the hex suffix
    memcpy(result + 1, iri, iri_size - compressed_hex_len * 2);
    *result = static_cast<uint8_t>(compressed_hex_len);
    result += iri_size - compressed_hex_len * 2 + 1;

    char temp[] = { 0, 0, 0 };

    for (size_t i = iri_size - compressed_hex_len * 2; i < iri_size - 1; i += 2) {
        temp[0] = iri[i];
        temp[1] = iri[i + 1];

        *result = (unsigned char) strtol(temp, NULL, 16);
        result++;
    }

    return iri_size - compressed_hex_len + 1;
}

size_t decompress_lower(char* iri_compressed, size_t iri_size)
{
    // first byte tells the hex length
    auto hex_bytes = *reinterpret_cast<const unsigned char*>(iri_compressed);

    assert(iri_size > hex_bytes);

    // last (hex_bytes) bytes will transfor into (2*hex_bytes)

    char* copy = iri_compressed + iri_size + hex_bytes;
    memcpy(copy, iri_compressed, iri_size);

    uint32_t non_hex_size = iri_size - (hex_bytes + 1);

    // copy again the non hex byes
    memcpy(iri_compressed, copy + 1, non_hex_size);

    char* out = iri_compressed + non_hex_size;
    char* read = copy + non_hex_size + 1;

    for (size_t i = 0; i < iri_size; ++i) {
        sprintf(out, "%02x", static_cast<unsigned char>(*read));
        out += 2;
        read++;
    }

    return iri_size - 1 + hex_bytes;
}

size_t decompress_upper(char* iri_compressed, size_t iri_size)
{
    // first byte tells the hex length
    auto hex_bytes = *reinterpret_cast<const unsigned char*>(iri_compressed);

    assert(iri_size > hex_bytes);

    // last (hex_bytes) bytes will transfor into (2*hex_bytes)

    char* copy = iri_compressed + iri_size + hex_bytes;
    memcpy(copy, iri_compressed, iri_size);

    uint32_t non_hex_size = iri_size - (hex_bytes + 1);

    // copy again the non hex byes
    memcpy(iri_compressed, copy + 1, non_hex_size);

    char* out = iri_compressed + non_hex_size;
    char* read = copy + non_hex_size + 1;

    for (size_t i = 0; i < iri_size; ++i) {
        sprintf(out, "%02X", static_cast<unsigned char>(*read));
        out += 2;
        read++;
    }

    return iri_size - 1 + hex_bytes;
}
} // namespace HexCompression

namespace UUIDCompression {
inline bool check_len_iri(size_t iri_size)
{
    return iri_size >= 36;
}

bool compress_lower(const char* iri, size_t iri_size, char* result)
{
    if (!check_len_iri(iri_size)) {
        return false;
    }
    const char* initial_iri = iri;
    size_t result_iri_pos = iri_size - 36;
    iri += result_iri_pos;

    char temp[3];
    temp[2] = '\0';
    for (int i = 0; i < 35;) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            if (iri[i] == '-') {
                i++;
                continue;
            } else {
                return false;
            }
        }

        if (!HexCompression::is_hex_lower(iri[i]) || !HexCompression::is_hex_lower(iri[i + 1])) {
            return false;
        }

        temp[0] = iri[i];
        temp[1] = iri[i + 1];

        result[result_iri_pos] = (unsigned char) strtol(temp, NULL, 16);
        result_iri_pos++;
        i += 2;
    }
    memcpy(result, initial_iri, iri_size - 36);
    return true;
}

bool compress_upper(const char* iri, size_t iri_size, char* result)
{
    if (!check_len_iri(iri_size)) {
        return false;
    }
    const char* initial_iri = iri;
    size_t result_iri_pos = iri_size - 36;
    iri += result_iri_pos;

    char temp[3];
    temp[2] = '\0';
    for (int i = 0; i < 35;) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            if (iri[i] == '-') {
                i++;
                continue;
            } else {
                return false;
            }
        }

        if (!HexCompression::is_hex_upper(iri[i]) || !HexCompression::is_hex_upper(iri[i + 1])) {
            return false;
        }

        temp[0] = iri[i];
        temp[1] = iri[i + 1];

        result[result_iri_pos] = (unsigned char) strtol(temp, NULL, 16);
        result_iri_pos++;
        i += 2;
    }
    memcpy(result, initial_iri, iri_size - 36);
    return true;
}

size_t decompress_lower(char* iri_compressed, size_t iri_size)
{
    // last 16 bytes will transfor into 36 characters
    // so we copy those 16 bytes reserving 20 bytes after the string
    char* out = iri_compressed + (iri_size - 16);
    char* read = iri_compressed + (iri_size + 20);

    // this writes a copy into read
    memcpy(read, out, 16);

    for (int i = 0; i < 16; ++i) {
        if (i == 4 || i == 6 || i == 8 || i == 10) {
            *out = '-';
            out++;
        }
        sprintf(out, "%02x", static_cast<unsigned char>(*read));
        read++;
        out += 2;
    }
    return iri_size + 20;
}

size_t decompress_upper(char* iri_compressed, size_t iri_size)
{
    // last 16 bytes will transfor into 36 characters
    // so we copy those 16 bytes reserving 20 bytes after the string
    char* out = iri_compressed + (iri_size - 16);
    char* read = iri_compressed + (iri_size + 20);

    // this writes a copy into read
    memcpy(read, out, 16);

    for (int i = 0; i < 16; ++i) {
        if (i == 4 || i == 6 || i == 8 || i == 10) {
            *out = '-';
            out++;
        }
        sprintf(out, "%02X", static_cast<unsigned char>(*read));
        read++;
        out += 2;
    }
    return iri_size + 20;
}
} // namespace UUIDCompression
