#pragma once

#include <string>

#include <boost/unordered/unordered_flat_map.hpp>

#include "query/exceptions.h"

namespace SPARQL {
struct TextIndexOptions {
    std::string predicate;
    std::string normalization { "nfkdCasefold" }; // optional
    std::string tokenization { "wsSplitPunc" }; // optional

    boost::unordered_flat_map<std::string, bool> opt2seen = {
        {    "predicate", false},
        {"normalization", false},
        { "tokenization", false}
    };

    void validate() const
    {
        for (const auto& [key, is_set] : opt2seen) {
            if (!is_set && (key != "normalization" && key != "tokenization")) {
                throw QueryException("Missing HNSW Index option: \"" + key + "\"");
            }
        }

        if (normalization != "identity" && normalization != "nfkdCasefold") {
            throw QueryException(
                "Invalid normalization: \"" + normalization
                + "\". Expected one of the following: { identity, nfkdCasefold }"
            );
        }

        if (tokenization != "identity" && tokenization != "wsSplitPunc" && tokenization != "wsRmPunc"
            && tokenization != "wsKeepPunc")
        {
            throw QueryException(
                "Invalid tokenization: \"" + tokenization
                + "\". Expected one of the following: { identity, wsSplitPunc, wsRmPunc, wsKeepPunc }"
            );
        }
    }

    std::vector<std::string> valid_options() const
    {
        std::vector<std::string> keys;
        keys.reserve(opt2seen.size());
        for (const auto& [key, _] : opt2seen) {
            keys.emplace_back(key);
        }
        return keys;
    }
};
} // namespace SPARQL
