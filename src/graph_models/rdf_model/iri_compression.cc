#include "iri_compression.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>


namespace HexCompression {

inline bool is_hex_upper(char byte) {
    return (('A' <= byte && byte <= 'F') || ('0' <= byte && byte <= '9'));
}

inline bool is_hex_lower(char byte) {
    return (('a' <= byte && byte <= 'f') || ('0' <= byte && byte <= '9'));
}

std::size_t get_lower_hex_length(const char* str, size_t str_len) {
    std::size_t hex_length = 0;
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

std::size_t get_upper_hex_length(const char* str, size_t str_len) {
    std::size_t hex_length = 0;
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


std::size_t compress(const char* iri, std::size_t iri_size, std::size_t uncompressed_hex_len, char* result) {
    int compressed_hex_len = uncompressed_hex_len / 2;

    // result+1 because we use 1 byte to store the length of the hex suffix
    memcpy(result + 1, iri, iri_size - compressed_hex_len * 2);
    *result = static_cast<uint8_t>(compressed_hex_len);
    result += iri_size - compressed_hex_len * 2 + 1;

    char temp[] = { 0, 0, 0 };

    for (std::size_t i = iri_size - compressed_hex_len * 2; i < iri_size - 1; i += 2) {
        temp[0] = iri[i];
        temp[1] = iri[i + 1];

        *result = (unsigned char) strtol(temp, NULL, 16);
        result++;
    }

    return iri_size - compressed_hex_len + 1;
}

std::size_t decompress_lower(const char* iri, std::size_t iri_size, char* result) {
    std::size_t hex_characters = *reinterpret_cast<const unsigned char*>(iri);

    // we skip the first byte since
    memcpy(result, iri + 1, iri_size - hex_characters - 1);
    result += iri_size - hex_characters - 1;

    for (std::size_t i = iri_size - hex_characters; i < iri_size; ++i) {
        sprintf(result, "%02x", static_cast<unsigned char>(iri[i]));
        result += 2;
    }
    return iri_size - 1 + hex_characters;
}

std::size_t decompress_upper(const char* iri, std::size_t iri_size, char* result) {
    std::size_t hex_characters = *reinterpret_cast<const unsigned char*>(iri);

    // we skip the first byte since
    memcpy(result, iri + 1, iri_size - hex_characters - 1);
    result += iri_size - hex_characters - 1;

    for (std::size_t i = iri_size - hex_characters; i < iri_size; ++i) {
        sprintf(result, "%02X", static_cast<unsigned char>(iri[i]));
        result += 2;
    }
    return iri_size - 1 + hex_characters;
}
} // namespace HexCompression


namespace UUIDCompression {

inline bool check_len_iri(std::size_t iri_size) {
    return iri_size >= 36;
}

bool compress_lower(const char* iri, std::size_t iri_size, char* result) {
    if (!check_len_iri(iri_size)) {
        return false;
    }
    std::size_t result_iri_pos = iri_size - 36;

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
    memcpy(result, iri, iri_size - 36);
    return true;
}

bool compress_upper(const char* iri, std::size_t iri_size, char* result) {
    if (!check_len_iri(iri_size)) {
        return false;
    }
    std::size_t result_iri_pos = iri_size - 36;

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
    memcpy(result, iri, iri_size - 36);
    return true;
}

void decompress_lower(char* iri_compressed, std::size_t iri_size, char* result) {
    memcpy(result, iri_compressed, iri_size - 16);
    result += iri_size - 16;

    for (int i = 0; i < 16; ++i) {
        if (i == 4 || i == 6 || i == 8 || i == 10) {
            *result = '-';
            result++;
        }
        sprintf(result, "%02x", static_cast<unsigned char>(iri_compressed[i]));
        result += 2;
    }

    *result = '\0';
}

void decompress_upper(char* iri_compressed, std::size_t iri_size, char* result) {
    memcpy(result, iri_compressed, iri_size - 16);
    result += iri_size - 16;

    for (int i = 0; i < 16; ++i) {
        if (i == 4 || i == 6 || i == 8 || i == 10) {
            *result = '-';
            result++;
        }
        sprintf(result, "%02X", static_cast<unsigned char>(iri_compressed[i]));
        result += 2;
    }

    *result = '\0';
}
} // namespace UUIDCompression
