#pragma once

#include <cstring>
#include <cstdint>
#include <iomanip>
#include <math.h>

class DecimalInlined {
public:
    static constexpr uint64_t INVALID_ID = UINT64_MAX;

    static uint64_t get_decimal_id(const char* str) {
        uint64_t ret = 0;
        char* ptr    = const_cast<char*>(str);

        if (*ptr == '-') {
            // Set sign bit
            ret |= 1ULL << 55;
            ptr++;
        }
        // Skip plus sign
        else if (*ptr == '+') {
            ptr++;
        }

        uint64_t number = 0;
        uint64_t sep    = 0;
        if (*ptr != '.') {
            // Handle integer part
            number = strtoull(ptr, &ptr, 10);
            if (number > 0x0007'FFFF'FFFF'FFFF) {
                // Integer overflow
                return INVALID_ID;
            }
        }
        if (*ptr == '.') {
            // Handle fractional part
            char* start = ++ptr;
            uint64_t fraction = strtoull(ptr, &ptr, 10);
            // Store separator only if fractional part is not zero
            if (fraction > 0) {
                // Remove trailing zeros
                while (fraction % 10 == 0) {
                    fraction /= 10;
                    ptr--;
                }

                if (fraction > 0x0007'FFFF'FFFF'FFFF) {
                    // Fractional overflow
                    return INVALID_ID;
                }

                sep = ptr - start;
                uint64_t times = 10;
                while (times < fraction) {
                    times *= 10;
                }
                number = number * times + fraction;
                if (number > 0x0007'FFFF'FFFF'FFFF) {
                    // Integer overflow (after adding fractional part)
                    return INVALID_ID;
                }
            }
        }

        ret |= sep;
        ret |= (number << 4);

        return ret;
    }

    uint64_t id;

    DecimalInlined(uint64_t _id) : id(_id) { }

    inline bool operator==(const DecimalInlined& rhs) const noexcept {
        return this->id == rhs.id;
    }

    inline bool operator!=(const DecimalInlined& rhs) const noexcept {
        return this->id != rhs.id;
    }

    // Returns the decimal string (normalized)
    std::string get_value_string() const {
        uint64_t decimal_id = id;
        std::stringstream ss;

        // Check sign bit
        if (decimal_id & (1ULL << 55)) {
            ss << '-';
            decimal_id &= ~(1ULL << 55);
        }
        uint64_t sep = decimal_id & 0xF;
        uint64_t number = decimal_id >> 4;
        if (sep) {
            ss << number / (uint64_t)std::pow(10, sep)
               << '.'
               // This is necessary for numbers like 0.0001
               << std::setw(sep)
               << std::setfill('0')
               << number % (uint64_t)std::pow(10, sep);
        } else {
            ss << number
               << '.'
               << '0';
        }
        return ss.str();
    }
};
