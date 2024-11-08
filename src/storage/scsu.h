#pragma once

#include <cstdint>

namespace SCSU {

uint_fast32_t compress(const char* in, uint_fast32_t count, char* out);
uint_fast32_t decompress(const char* in, uint_fast32_t count, char* out);

} // namespace SCSU
