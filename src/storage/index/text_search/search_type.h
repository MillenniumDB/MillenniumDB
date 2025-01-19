#pragma once

#include <ostream>

namespace TextSearch {

// Used to differentiate between Match and Prefix
// Either type can also be with or without errors
enum class SearchType { Match, Prefix };

inline std::ostream& operator<<(std::ostream& os, const SearchType& search_type) {
    switch (search_type) {
        case SearchType::Match:
            return os << "MATCH";
        case SearchType::Prefix:
            return os << "PREFIX";
        default:
            return os << "UNKNOWN_SEARCH_TYPE";
    }
}

} // namespace TextSearch
