#include "base/query/sparql/decimal.h"

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

int compare_external(std::string& lhs, std::string& rhs) {
    // *** ASSUMPTIONS ***
    // The format of both strings is:
    // (-)?(0|[1-9][0-9]*).(0|[0-9]*[1-9])

    // 1. Compare signs
    bool lhs_neg = lhs[0] == '-';
    bool rhs_neg = rhs[0] == '-';
    int sign_diff = lhs_neg - rhs_neg;
    if (sign_diff != 0) {
        return sign_diff > 0 ? -1 : 1;
    }

    // 2. Compare integer part
    size_t lhs_sep = lhs.find('.');
    size_t rhs_sep = rhs.find('.');
    // Compare integer part length
    int intlen_diff = lhs_sep - rhs_sep;
    if (intlen_diff != 0) {
        return lhs_neg ? -intlen_diff : intlen_diff;
    }
    // NOTE: lhs_sep == rhs_sep at this point
    // Compare integer part digits
    int int_diff = strncmp(lhs.c_str(), rhs.c_str(), lhs_sep);
    if (int_diff != 0) {
        return lhs_neg ? -int_diff : int_diff;
    }
    // 3. Compare fractional part (if it exists)
    // Compare fractional part digits
    int frac_diff = strcmp(lhs.c_str() + lhs_sep, rhs.c_str() + rhs_sep);
    if (frac_diff != 0) {
        return lhs_neg ? -frac_diff : frac_diff;
    }

    return 0;
}

int compare(std::string& lhs, std::string& rhs) {
    std::string lhs_normalized = Decimal::normalize(lhs);
    std::string rhs_normalized = Decimal::normalize(rhs);

    return compare_external(lhs_normalized, rhs_normalized);
}

int main() {
    std::vector<std::string> tests = {
        "-14040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040",
        "-4040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040",
        "-30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303.22",
        "-30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303.2",
        "-30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303.1",
        "-.30403030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303",
        "-.303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303035",
        "-.30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303",
        ".30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303",
        ".303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303035",
        ".30403030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303",
        "30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303.1",
        "30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303.2",
        "30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303.22",
        "4040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040",
        "14040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040"
    };

    for (size_t i=0; i<tests.size(); i++) {
        // EQ
        auto res_eq = compare(tests[i], tests[i]);
        if (res_eq != 0) {
            std::cout << "EQ: " << tests[i] << " != " << tests[i] << std::endl;
            return 1;
        }
        for (size_t j=i+1; j<tests.size(); j++) {
            // LT and GT
            auto res_lt = compare(tests[i], tests[j]);
            auto res_gt = compare(tests[j], tests[i]);

            if (res_lt >= 0 || res_gt <= 0) {
                std::cout << "NOT LT:\n" << tests[i] << "\n" << tests[j] << std::endl;
                return 1;
            }
        }
    }

    return 0;
}