#pragma once

#include <regex>
#include <string>

#include <boost/unordered/unordered_flat_map.hpp>

#include "query/exceptions.h"

namespace MQL {
struct HNSWIndexOptions {
    std::string property;
    std::string metric;
    uint64_t dimension;
    uint64_t max_edges;
    uint64_t max_candidates;

    boost::unordered_flat_map<std::string, bool> opt2seen = {
        {     "property", false},
        {       "metric", false},
        {    "dimension", false},
        {     "maxEdges", false},
        {"maxCandidates", false}
    };

    std::vector<std::string> valid_options() const
    {
        std::vector<std::string> keys;
        keys.reserve(opt2seen.size());
        for (const auto& [key, _] : opt2seen) {
            keys.emplace_back(key);
        }
        return keys;
    }

    static bool is_valid_property_key(const std::string& key)
    {
        static const std::regex pattern(R"([A-Za-z][A-Za-z0-9_]*)");
        return std::regex_match(key, pattern);
    }

    void validate() const
    {
        for (const auto& [key, is_set] : opt2seen) {
            if (!is_set) {
                throw QueryException("Missing HNSW Index option: \"" + key + "\"");
            }
        }

        if (!is_valid_property_key(property)) {
            throw QueryException("Invalid property: \"" + property + "\"");
        }

        if (metric != "cosineDistance" && metric != "manhattanDistance" && metric != "euclideanDistance") {
            throw QueryException(
                "Invalid metric: \"" + metric
                + "\". Expected one of the following: { cosineDistance, manhattanDistance, "
                  "euclideanDistance }"
            );
        }

        if (dimension == 0) {
            throw QueryException("Invalid dimension: must be a positive integer");
        }

        if (max_edges == 0) {
            throw QueryException("Invalid maxEdges: must be a positive integer");
        }

        if (max_candidates == 0) {
            throw QueryException("Invalid maxCandidates: must be a positive integer");
        }
    }
};
} // namespace MQL
