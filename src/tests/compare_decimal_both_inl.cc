#include "base/graph_object/decimal_inlined.h"

#include <iostream>
#include <math.h>
#include <string>
#include <vector>

int compare(uint64_t lhs, uint64_t rhs) {
    if (lhs == rhs) {
        return 0;
    }

    // Check sign bit
    uint64_t sign_mask = 1ULL << 55;
    int64_t  sign_diff = (lhs & sign_mask) - (rhs & sign_mask);
    if (sign_diff != 0) {
        return sign_diff > 0 ? -1 : 1;
    }

    int64_t diff = 0;

    // Get separators
    uint64_t separator_mask = 0xF;
    uint64_t lhs_sep = (lhs & separator_mask);
    uint64_t rhs_sep = (rhs & separator_mask);
    int64_t  sep_diff = lhs_sep - rhs_sep;
    // Get numbers
    uint64_t number_mask  = 0x007F'FFFF'FFFF'FFF0;
    uint64_t lhs_number = (lhs & number_mask) >> 4;
    uint64_t rhs_number = (rhs & number_mask) >> 4;

    // Handle different separators
    if (sep_diff != 0) {
        uint64_t lhs_int = lhs_number / (uint64_t)std::pow(10, lhs_sep);
        uint64_t rhs_int = rhs_number / (uint64_t)std::pow(10, rhs_sep);
        int64_t int_diff = lhs_int - rhs_int;
        if (int_diff != 0) {
            // Different integer part
            diff = int_diff > 0 ? 1 : -1;
        } else {
            // Equal integer part, compare fractional part
            uint64_t lhs_frac = lhs_number % (uint64_t)std::pow(10, lhs_sep);
            uint64_t rhs_frac = rhs_number % (uint64_t)std::pow(10, rhs_sep);
            // Pad with zeros if needed
            if (lhs_sep > rhs_sep) {
                rhs_frac *= (uint64_t)std::pow(10, lhs_sep - rhs_sep);
            } else {
                lhs_frac *= (uint64_t)std::pow(10, rhs_sep - lhs_sep);
            }
            int64_t frac_diff = lhs_frac - rhs_frac;
            diff = frac_diff > 0 ? 1 : -1;
        }
    } 
    // Handle same separators
    else {
        int64_t number_diff = lhs_number - rhs_number;
        diff = number_diff > 0 ? 1 : -1;
    }
    // Flip if the result if both signs were negative
    if (lhs & sign_mask) {
        diff *= -1;
    }
    return diff;
}

int main() {
    std::vector<std::string> tests_str = {
        "-10.10",
        "-1.1",
        "-1.",
        "-.1",
        "0",
        ".1",
        "1.",
        "1.1",
        "10.10",
        "+100.100",
    };

    std::vector<uint64_t> tests;
    for (auto &str : tests_str) {
        tests.emplace_back(DecimalInlined::get_decimal_id(str.c_str()));
    }

    for (size_t i=0; i<tests.size(); i++) {
        // EQ
        auto res_eq = compare(tests[i], tests[i]);
        if (res_eq != 0) {
            return 1;
        }
        for (size_t j=i+1; j<tests.size(); j++) {
            // LT and GT
            auto res_lt = compare(tests[i], tests[j]);
            auto res_gt = compare(tests[j], tests[i]);

            if (res_lt >= 0 || res_gt <= 0) {
                return 1;
            }
        }
    }

    return 0;
}