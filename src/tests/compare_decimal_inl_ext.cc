#include "base/graph_object/decimal_inlined.h"
#include "base/query/sparql/decimal.h"

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

// Exactly the same method as in src/tests/compare_decimal_both_ext.cc
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
    // 3. Compare fractional part
    // Compare fractional part digits
    int frac_diff = strcmp(lhs.c_str() + lhs_sep, rhs.c_str() + rhs_sep);
    if (frac_diff != 0) {
        return lhs_neg ? -frac_diff : frac_diff;
    }

    return 0;
}

int compare(DecimalInlined lhs, std::string& rhs) {
    std::string lhs_external   = lhs.get_value_string();
    std::string rhs_normalized = Decimal::normalize(rhs);

    return compare_external(lhs_external, rhs_normalized);
}

int main() {
    // On this tests the left hand side is always lower than the right hand side
    std::vector<std::pair<DecimalInlined, std::string>> tests = {
        { DecimalInlined(DecimalInlined::get_decimal_id("-11.1111")), "-1.10000000000000000000000000000000000000001" },
        { DecimalInlined(DecimalInlined::get_decimal_id("-1")), "-.010000000000000000000000000000000000000001" },
        { DecimalInlined(DecimalInlined::get_decimal_id("-0.01")), "-.000100000000000000000000000000000000001" },

        { DecimalInlined(DecimalInlined::get_decimal_id("0")), ".30303030303030303030303030303030303" },
        { DecimalInlined(DecimalInlined::get_decimal_id("0")), "30303030303030303030303030303030303" },
        { DecimalInlined(DecimalInlined::get_decimal_id(".01")), ".01000000000000000000000000000000000000001" },
        { DecimalInlined(DecimalInlined::get_decimal_id("1")), "1111111111111111111111111111111111111111111111" },
        { DecimalInlined(DecimalInlined::get_decimal_id("1.1")), "1.10000000000000000000000000000000000000001" },
        { DecimalInlined(DecimalInlined::get_decimal_id("-1.1")), "1.10000000000000000000000000000000000000001" },
    };

    for (auto& test : tests) {
        int result = compare(test.first, test.second);
        if (result >= 0) {
            return 1;
        }
    }
    return 0;
}