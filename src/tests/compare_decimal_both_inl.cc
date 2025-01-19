#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "graph_models/rdf_model/comparisons.h"
#include "graph_models/common/datatypes/decimal.h"
#include "graph_models/common/datatypes/decimal_inlined.h"

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

    std::vector<ObjectId> tests;
    bool error;
    for (auto &str : tests_str) {
        tests.emplace_back(ObjectId(Decimal(str, &error).to_internal()));
        assert(!error);
    }

    bool test_error = false;

    for (size_t i = 0; i < tests.size(); i++) {
        auto dec = tests[i];

        auto self_compare = SPARQL::Comparisons::compare(dec, dec);

        if (self_compare != 0) {
            std::cout << tests_str[i] << " != " << tests_str[i] << "\n";
            test_error = true;
        }

        for (size_t j=i+1; j<tests.size(); j++) {
            auto dec1 = tests[i];
            auto dec2 = tests[j];
            // LT and GT
            auto res_lt = SPARQL::Comparisons::compare(dec1, dec2);
            auto res_gt = SPARQL::Comparisons::compare(dec2, dec1);

            if (res_lt >= 0) {
                std::cout << "compare(" << tests_str[i] << ", " << tests_str[j] << ") >= 0\n";
                test_error = true;
            }

            if (res_gt <= 0) {
                std::cout << "compare(" << tests_str[j] << ", " << tests_str[i] << ") <= 0\n";
                test_error = true;
            }
        }
    }
    return test_error ? 1 : 0;
}
