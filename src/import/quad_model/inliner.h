#pragma once

#include <cstdint>
#include <cstring>

#include "base/exceptions.h"
#include "base/ids/object_id.h"

class Inliner {
public:
    static uint64_t inline_int(int64_t int_value) {
        if (int_value < 0) {
            int_value *= -1;

            // check if it needs more than 7 bytes
            if ( (int_value & 0xFF00'0000'0000'0000UL) == 0) {
                int_value = (~int_value) & 0x00FF'FFFF'FFFF'FFFFUL;
                return ObjectId::VALUE_NEGATIVE_INT_MASK | int_value;
            } else {
                // VALUE_EXTERNAL_INT_MASK
                throw NotSupportedException("BIG INTEGERS NOT SUPPORTED YET");
                return 0;
            }
        } else {
            // check if it needs more than 7 bytes
            if ( (int_value & 0xFF00'0000'0000'0000UL) == 0) {
                return ObjectId::VALUE_POSITIVE_INT_MASK | int_value;
            } else {
                // VALUE_EXTERNAL_INT_MASK
                throw NotSupportedException("BIG INTEGERS NOT SUPPORTED YET");
                return 0;
            }
        }
    }

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

    static uint64_t inline_float(float f) {
        unsigned char bytes[sizeof(float)];
        std::memcpy(bytes, &f, sizeof(float));

        uint64_t res = 0;
        int shift_size = 0;
        for (std::size_t i = 0; i < sizeof(bytes); ++i) {
            uint64_t byte = bytes[i];
            res |= byte << shift_size;
            shift_size += 8;
        }
        return res | ObjectId::VALUE_FLOAT_MASK;
    }
};