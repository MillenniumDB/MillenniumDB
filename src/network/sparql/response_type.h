#pragma once

#include <string>

#include "query/exceptions.h"

namespace SPARQL {

enum class ResponseType {
    JSON,
    XML,
    TSV,
    CSV,
    TURTLE,
};

inline std::string response_type_to_string(ResponseType response_type) {
    switch (response_type) {
        case ResponseType::JSON: return "JSON";
        case ResponseType::XML: return "XML";
        case ResponseType::TSV: return "TSV";
        case ResponseType::CSV: return "CSV";
        case ResponseType::TURTLE: return "TURTLE";
        default:
            throw LogicException("Unmanaged ResposeType in response_type_to_string");
    }
}

} // namespace SPARQL
