#pragma once

#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace TextSearch {

enum class TOKENIZE_TYPE : uint8_t {
    IDENTITY,
    WHITESPACE_SPLIT_PUNCTUATION,
    WHITESPACE_REMOVE_PUNCTUATION,
    WHITESPACE_KEEP_PUNCTUATION
};

inline std::string to_string(const TOKENIZE_TYPE& type)
{
    switch (type) {
    case TOKENIZE_TYPE::IDENTITY:
        return "IDENTITY";
    case TOKENIZE_TYPE::WHITESPACE_SPLIT_PUNCTUATION:
        return "WHITESPACE_SPLIT_PUNCTUATION";
    case TOKENIZE_TYPE::WHITESPACE_REMOVE_PUNCTUATION:
        return "WHITESPACE_REMOVE_PUNCTUATION";
    case TOKENIZE_TYPE::WHITESPACE_KEEP_PUNCTUATION:
        return "WHITESPACE_KEEP_PUNCTUATION";
    default:
        return "UNKNOWN_TOKENIZE_TYPE";
    }
}

static constexpr auto DEFAULT_TOKENIZE_TYPE = TOKENIZE_TYPE::WHITESPACE_SPLIT_PUNCTUATION;

inline std::ostream& operator<<(std::ostream& os, const TOKENIZE_TYPE& type)
{
    return os << to_string(type);
}

// Function signature used to tokenize the text being indexed
typedef std::vector<std::string> TokenizeFuncType(const std::string&);

// Given the string "Spider-Man 3 (2007)":

TokenizeFuncType noop_tokenize;                          // Tokens (1): ["Spider-Man 3 (2007)"]
TokenizeFuncType whitespace_remove_punctuation_tokenize; // Tokens (3): ["SpiderMan", "3", "2007"]
TokenizeFuncType whitespace_split_punctuation_tokenize;  // Tokens (4): ["Spider", "Man", "3", "2007"]
TokenizeFuncType whitespace_keep_punctuation_tokenize;   // Tokens (4): ["Spider-Man", "3", "(2007)"]

inline TokenizeFuncType* get_tokenize_func(TOKENIZE_TYPE type)
{
    switch (type) {
    case TOKENIZE_TYPE::IDENTITY:
        return &noop_tokenize;
    case TOKENIZE_TYPE::WHITESPACE_SPLIT_PUNCTUATION:
        return &whitespace_split_punctuation_tokenize;
    case TOKENIZE_TYPE::WHITESPACE_REMOVE_PUNCTUATION:
        return &whitespace_remove_punctuation_tokenize;
    case TOKENIZE_TYPE::WHITESPACE_KEEP_PUNCTUATION:
        return &whitespace_keep_punctuation_tokenize;
    default:
        throw std::logic_error(
            "Unmanaged tokenization function with type " + std::to_string(static_cast<uint8_t>(type))
        );
    }
}

} // namespace TextSearch
