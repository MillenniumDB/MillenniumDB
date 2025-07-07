#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "storage/index/text_search/normalize_funcs.h"
#include "storage/index/text_search/tokenize_funcs.h"

typedef bool TestFunction();

bool test_nfkd_casefold_normalize()
{
    std::vector<std::pair<std::string, std::string>> input_expected = {
        {"unchanged",    "unchanged"},
        {  "ÑanDú",        "nandu"},
        {   "Vrgoč",        "vrgoc"},
        {      "㌀", "アハート"},
        { "i⁹i₉",         "i9i9"},
        {       "ǆ",           "dz"},
    };

    for (const auto& [input, expected] : input_expected) {
        const auto normalized = TextSearch::nfkd_casefold_normalize(input);
        if (normalized != expected) {
            std::cerr << "Normalized \"" << input << "\" to \"" << normalized << "\", expected \"" << expected
                      << "\"\n";
            return false;
        }
    }

    return true;
}

bool test_ws_tokenizers()
{
    const auto input = "Spider-Man 3 (2007)";
    const std::vector<std::string> expected_ws_split_punct = { "Spider", "Man", "3", "2007" };
    const std::vector<std::string> expected_ws_remove_punct = { "SpiderMan", "3", "2007" };
    const std::vector<std::string> expected_ws_keep_punct = { "Spider-Man", "3", "(2007)" };

    const auto tokenized_ws_split_punct = TextSearch::whitespace_split_punctuation_tokenize(input);
    const auto tokenized_ws_remove_punct = TextSearch::whitespace_remove_punctuation_tokenize(input);
    const auto tokenized_ws_keep_punct = TextSearch::whitespace_keep_punctuation_tokenize(input);

    auto cmp_str_vec = [](const std::vector<std::string>& lhs, const std::vector<std::string>& rhs) {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        for (uint64_t i = 0; i < lhs.size(); ++i) {
            if (lhs[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    };

    if (!cmp_str_vec(tokenized_ws_split_punct, expected_ws_split_punct)) {
        std::cerr << "Whitespace split punctuation tokenize failed\n";
        return false;
    }

    if (!cmp_str_vec(tokenized_ws_remove_punct, expected_ws_remove_punct)) {
        std::cerr << "Whitespace remove punctuation tokenize failed\n";
        return false;
    }

    if (!cmp_str_vec(tokenized_ws_keep_punct, expected_ws_keep_punct)) {
        std::cerr << "Whitespace keep punctuation tokenize failed\n";
        return false;
    }

    return true;
}

int main()
{
    std::vector<TestFunction*> tests { &test_nfkd_casefold_normalize, &test_ws_tokenizers };

    for (uint64_t i = 0; i < tests.size(); ++i) {
        try {
            if (!tests[i]()) {
                std::cerr << "Test #" << i << " failed" << std::endl;
                return EXIT_FAILURE;
            }
            std::cout << "Test #" << i << " passed" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Test #" << i << " failed due to an exception: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
