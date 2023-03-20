#include "base/graph_object/datetime.h"

#include <string>
#include <tuple>
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
    
    // Check precision bit
    uint64_t precision_mask = 1ULL << 54;
    int64_t  precision_diff = (lhs & precision_mask) - (rhs & precision_mask);
    if (precision_diff != 0) {
        diff = precision_diff > 0 ? 1 : -1;
    }
    // Handle same precision
    else {
        uint64_t datetime_mask = 0x00'3F'FFFF'FFFF'FFFF;
        int64_t  datetime_diff = (lhs & datetime_mask) - (rhs & datetime_mask);
        diff = datetime_diff > 0 ? 1 : -1;
    }
    // Flip if the result if both signs were negative
    if (lhs & sign_mask) {
        diff *= -1;
    }
    return diff;
}

int main() {
    std::vector<std::string> tests_str = {
        "-9999999999-00-00T00:00:00Z",
        "-2222-01-01T01:01:01Z",
        "-1111-01-01T01:01:01Z",
        "1111-01-01T01:01:01Z",
        "2222-01-01T01:01:01Z",
        "9999999999-00-00T00:00:00Z",
    };

    std::vector<uint64_t> tests;
    for (auto &str : tests_str) {
        tests.emplace_back(DateTime::get_datetime_id(str.c_str()));
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