#pragma once

#include <cstdint>
#include <ostream>

namespace TextSearch {

// Used to differentiate between Match and Prefix
// Either type can also be with or without errors
enum class SearchType : uint8_t {
    MATCH,
    PREFIX
};

inline std::ostream& operator<<(std::ostream& os, const SearchType& search_type) {
    switch (search_type) {
        case SearchType::MATCH:
            return os << "MATCH";
        case SearchType::PREFIX:
            return os << "PREFIX";
        default:
            return os << "UNKNOWN_SEARCH_TYPE";
    }
}

} // namespace TextSearch
