#pragma once

#include <string>

#include "network/sparql/service/response_parser.h"

int consume(bool https,
            const std::string& host,
            const std::string& port,
            const std::string& target,
            const std::string& body,
            std::string& response,
            Format& format);
