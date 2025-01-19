#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>


namespace HexCompression {
// The minimum length of the IRI for the hexadecimal suffix to be compressed
constexpr std::size_t MIN_LEN_TO_COMPRESS = 11;

// The maximum length of the hexadecimal suffix that can be compressed. 510 = 255 * 2
constexpr std::size_t MAX_HEX_LEN_TO_COMPRESS = 510;

// The minimum length of the hexadecimal suffix to be compressed
constexpr std::size_t MIN_HEX_LEN_TO_COMPRESS = 4;

inline bool is_hex_upper(char byte);
inline bool is_hex_lower(char byte);

std::size_t get_lower_hex_length(const char* str, size_t str_len);
std::size_t get_upper_hex_length(const char* str, size_t str_len);

std::size_t compress(const char* iri, std::size_t iri_size, std::size_t uncompressed_hex_len, char* result);
std::size_t decompress_lower(const char* iri, std::size_t iri_size, char* result);
std::size_t decompress_upper(const char* iri, std::size_t iri_size, char* result);
} // namespace HexCompression


namespace UUIDCompression {

inline bool check_len_iri(std::size_t iri_size);

bool compress_lower(const char* iri, std::size_t iri_size, char* result);
bool compress_upper(const char* iri, std::size_t iri_size, char* result);
void decompress_lower(char* iri_compressed, std::size_t iri_size, char* result);
void decompress_upper(char* iri_compressed, std::size_t iri_size, char* result);

} // namespace UUIDCompression
