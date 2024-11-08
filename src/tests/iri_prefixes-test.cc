#include <iostream>

#include "graph_models/rdf_model/iri_prefixes.h"

typedef bool TestFunction();

bool equal(const char* a, size_t a_count, const char* b, size_t b_count) {
    if (a_count != b_count) {
        return false;
    }
    auto end = a + a_count;
    while (a < end) {
        if (*a++ != *b++) {
            return false;
        }
    }
    return true;
}


bool simple() {
    std::vector<std::string> prefixes_strings = {
        "http",
        "http://",
        "http://www.",
        "https",
        "https://",
        "https://www.",
        "x",
        "xxx",
        "xxxxx",
        "yyyyy",
        "yyy",
        "y",
    };

    IriPrefixes prefixes;
    prefixes.init(std::move(prefixes_strings));

    std::vector<std::pair<std::string, std::string>> tests = {
        { "http://hello.com", "http://" },
        { "https://hello.com", "https://" },
        { "ftp://hello.com", "" },
        { "https://www.hello.com", "https://www." },
        { "https://www.ñññ.com", "https://www." },
        { "httpñ", "http" },
        { "httñ", "" },
        { "", "" },
        { "htt", "" },
        { "https:/", "https" },
        { "x", "x" },
        { "xx", "x" },
        { "xxx", "xxx" },
        { "xxxx", "xxx" },
        { "xxxxx", "xxxxx" },
        { "xxxxxx", "xxxxx" },
        { "y", "y" },
        { "yy", "y" },
        { "yyy", "yyy" },
        { "yyyy", "yyy" },
        { "yyyyy", "yyyyy" },
        { "yyyyyy", "yyyyy" },
    };

    auto error = false;

    for (auto& [string, expected] : tests) {

        auto [received_id, _] = prefixes.get_prefix_id(string.data(), string.size());
        auto received = prefixes.get_prefix(received_id);


        if (received != expected) {
            std::cerr << "String: " << string
                << ", Expected: " << expected
                << ", Received: " << received
                << "\n";
            error = true;
        }
    }

    return error;
}


int main() {
    std::vector<TestFunction*> tests;

    tests.push_back(&simple);

    auto error = false;

    for (auto& test_func : tests) {
        if (test_func()) {
            error = true;
        }
    }

    return error;
}
