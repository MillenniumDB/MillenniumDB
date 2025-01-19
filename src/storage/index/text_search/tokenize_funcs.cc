#include "tokenize_funcs.h"

#include <cctype>

namespace TextSearch {

std::vector<std::string> noop_tokenize(const std::string& string)
{
    return { string };
}

std::vector<std::string> whitespace_remove_punctuation_tokenize(const std::string& string)
{
    std::vector<std::string> tokens;

    std::string current_token;
    for (const auto& ch : string) {
        if (std::isspace(ch)) {
            if (!current_token.empty()) {
                tokens.emplace_back(std::move(current_token));
                current_token.clear();
            }
        } else if (!std::ispunct(ch)) {
            current_token += ch;
        }
    }

    if (!current_token.empty()) {
        tokens.emplace_back(std::move(current_token));
    }

    return tokens;
}

std::vector<std::string> whitespace_split_punctuation_tokenize(const std::string& string)
{
    std::vector<std::string> tokens;

    std::string current_token;
    for (const auto& ch : string) {
        if (std::isspace(ch) || std::ispunct(ch)) {
            if (!current_token.empty()) {
                tokens.emplace_back(std::move(current_token));
                current_token.clear();
            }
        } else {
            current_token += ch;
        }
    }

    if (!current_token.empty()) {
        tokens.emplace_back(std::move(current_token));
    }

    return tokens;
}

std::vector<std::string> whitespace_keep_punctuation_tokenize(const std::string& string)
{
    std::vector<std::string> tokens;

    std::string current_token;
    for (const auto& ch : string) {
        if (std::isspace(ch)) {
            if (!current_token.empty()) {
                tokens.emplace_back(std::move(current_token));
                current_token.clear();
            }
        } else {
            current_token += ch;
        }
    }

    if (!current_token.empty()) {
        tokens.emplace_back(std::move(current_token));
    }

    return tokens;
}

} // namespace TextSearch
