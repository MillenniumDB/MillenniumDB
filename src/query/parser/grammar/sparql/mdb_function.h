#pragma once

#include <cstdint>
#include <ostream>
#include <string>

namespace SPARQL::MDBFunction {

static constexpr char TEXT_SEARCH[] = "http://millenniumdb.com/function/textSearch";

enum class MDBFunctionType : uint8_t {
    NONE,
    TEXT_SEARCH,
};

inline MDBFunctionType get_mdb_function_type(const std::string& iri_str)
{
    if (iri_str == TEXT_SEARCH) {
        return MDBFunctionType::TEXT_SEARCH;
    }

    return MDBFunctionType::NONE;
}

inline std::ostream& operator<<(std::ostream& os, MDBFunctionType mdb_function_type)
{
    switch (mdb_function_type) {
    case MDBFunctionType::TEXT_SEARCH:
        return os << "TEXT_SEARCH";
    default:
        return os << "UNKNOWN";
    }
}

} // namespace SPARQL::MDBFunction