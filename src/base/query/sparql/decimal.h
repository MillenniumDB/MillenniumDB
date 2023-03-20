#pragma once

#include <cstring>
#include <iomanip>

class Decimal {
public:
    static std::string normalize(const std::string& str) {
        // Normalized format: (0|[1-9][0-9]*).(0|[0-9]*[1-9])
        std::stringstream ss;

        if (str[0] == '-') {
            ss << '-';
        }
        // Skip leading zeros and signs
        size_t start = str.find_first_not_of("+-0");
        if (start == std::string::npos) {
            // All zeros (e.g. 00000000)
            ss << "0.0";
            return ss.str();
        }

        // Check if the number has a decimal point
        size_t sep = str.find_first_of('.', start);
        // Has not a decimal point (e.g. 123)
        if (sep == std::string::npos) {
            ss << str.substr(start) << ".0";
        }
        // Has a decimal point
        else {
            if (sep == start) {
                // No integer part (e.g. .123)
                ss << '0';
            }
            // Skip trailing zeros
            size_t end = str.find_last_not_of('0');
            // Has not a fractional part (e.g. 123.)
            if (end == sep) {
                ss << str.substr(start, sep - start) << ".0";
            }
            // Has a fractional part (e.g. 123.456)
            else {
                ss << str.substr(start, end - start + 1);
            }
        }

        return ss.str();
    }
    std::string str;

    Decimal(const std::string str) : str(str) { }
};