#pragma once

#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>

namespace TextSearch {

enum class NORMALIZE_TYPE : uint8_t {
    IDENTITY,
    NFKD_CASEFOLD
};

inline std::string to_string(const NORMALIZE_TYPE& type)
{
    switch (type) {
    case NORMALIZE_TYPE::IDENTITY:
        return "IDENTITY";
    case NORMALIZE_TYPE::NFKD_CASEFOLD:
        return "NFKD_CASEFOLD";
    default:
        return "UNKNOWN_NORMALIZE_TYPE";
    }
}

static constexpr auto DEFAULT_NORMALIZE_TYPE = NORMALIZE_TYPE::NFKD_CASEFOLD;

inline std::ostream& operator<<(std::ostream& os, const NORMALIZE_TYPE& type)
{
    return os << to_string(type);
}

// Function signature to normalize tokens during indexation, and queries when searching
typedef std::string NormalizeFuncType(const std::string&);

NormalizeFuncType noop_normalize;
NormalizeFuncType nfkd_casefold_normalize;

inline NormalizeFuncType* get_normalize_func(NORMALIZE_TYPE type)
{
    switch (type) {
    case NORMALIZE_TYPE::IDENTITY:
        return &noop_normalize;
    case NORMALIZE_TYPE::NFKD_CASEFOLD:
        return &nfkd_casefold_normalize;
    default:
        throw std::logic_error(
            "Unmanaged normalization function with type " + std::to_string(static_cast<uint8_t>(type))
        );
    }
}

} // namespace TextSearch
