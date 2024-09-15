#pragma once

#include <string>

#include "network/sparql/service/response_parser.h"

namespace SPARQL {

int send_service_request(
    bool https,
    const std::string& host,
    const std::string& port,
    const std::string& target,
    const std::string& body,
    std::string& response,
    Format& format);
}
