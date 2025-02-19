#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace HexCompression {
// The minimum length of the IRI for the hexadecimal suffix to be compressed
constexpr size_t MIN_LEN_TO_COMPRESS = 11;

// The maximum length of the hexadecimal suffix that can be compressed. 510 = 255 * 2
constexpr size_t MAX_HEX_LEN_TO_COMPRESS = 510;

// The minimum length of the hexadecimal suffix to be compressed
constexpr size_t MIN_HEX_LEN_TO_COMPRESS = 4;

bool is_hex_upper(char byte);
bool is_hex_lower(char byte);

size_t get_lower_hex_length(const char* str, size_t str_len);
size_t get_upper_hex_length(const char* str, size_t str_len);

size_t compress(const char* iri, size_t iri_size, size_t uncompressed_hex_len, char* result);

size_t decompress_lower(char* iri, size_t iri_size);
size_t decompress_upper(char* iri, size_t iri_size);
} // namespace HexCompression

namespace UUIDCompression {
bool check_len_iri(size_t iri_size);

bool compress_lower(const char* iri, size_t iri_size, char* result);
bool compress_upper(const char* iri, size_t iri_size, char* result);

size_t decompress_lower(char* iri_compressed, size_t iri_size);
size_t decompress_upper(char* iri_compressed, size_t iri_size);
} // namespace UUIDCompression
