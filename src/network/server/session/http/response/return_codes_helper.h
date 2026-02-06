#pragma once

#include <chrono>
#include <ostream>

#include "macros/time.h"
#include "network/sparql/response_type.h"
#include "query/exceptions.h"
#include "query/executor/query_executor/gql/return_executor.h"
#include "query/executor/query_executor/mql/return_type.h"

namespace MDBServer {

inline void send_ok(std::ostream& os)
{
    os << "HTTP/1.1 200 OK\r\n"
          "Content-Type: text/plain\r\n"
          "Connection: close\r\n"
          "Content-Length: 2\r\n"
          "\r\n"
          "OK";
}

inline void send_not_found(std::ostream& os)
{
    os << "HTTP/1.1 404 Not Found\r\n"
          "Content-Type: text/plain\r\n"
          "Connection: close\r\n"
          "Content-Length: 9\r\n"
          "\r\n"
          "Not found";
}

inline void send_timeout(std::ostream& os)
{
    os << "HTTP/1.1 408 Request Timeout\r\n"
          "Content-Type: text/plain\r\n"
          "Connection: close\r\n"
          "Content-Length: 15\r\n"
          "\r\n"
          "Query timed out";
}

inline void send_bad_request(std::ostream& os, std::string_view body)
{
    os << "HTTP/1.1 400 Bad Request\r\n"
          "Content-Type: text/plain\r\n"
       << "Content-Length: " << body.size() << "\r\n"
       << "Connection: close\r\n"
          "\r\n"
       << body;
}

inline void send_internal_error(std::ostream& os, std::string_view body)
{
    os << "HTTP/1.1 500 Internal Server Error\r\n"
          "Content-Type: text/plain\r\n"
       << "Content-Length: " << body.size() << "\r\n"
       << "Connection: close\r\n"
          "\r\n"
       << body;
}

inline void send_unauthorized(std::ostream& os)
{
    os << "HTTP/1.1 401 Unauthorized\r\n"
          "WWW-Authenticate: Bearer\r\n"
          "Content-Length: 13\r\n"
          "Connection: close\r\n"
          "\r\n"
          "Access denied";
}

inline void send_options(std::ostream& os)
{
    os << "HTTP/1.1 204 No Content\r\n"
          "Access-Control-Allow-Origin: *\r\n"
          "Access-Control-Allow-Headers: Content-Type, Accept, Authorization\r\n"
          "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
          "Content-Length: 0\r\n"
          "Connection: keep-alive\r\n"
          "\r\n";
    return;
}

inline void send_token_authorized(
    std::ostream& os,
    const std::string& auth_token,
    std::chrono::system_clock::time_point valid_until
)
{
    auto time = mdb_format_time(valid_until, "%Y-%m-%dT%H:%M:%S");
    std::string body = "{\"token\":\"" + auth_token + "\",\"valid_until\":\"" + time + "\"}";

    os << "HTTP/1.1 200 OK\r\n"
       << "Content-Type: application/json; charset=utf-8\r\n"
       << "Content-Length: " << body.size() << "\r\n"
       << "Connection: keep-alive\r\n"
          "Access-Control-Allow-Origin: *\r\n"
          "\r\n"
       << body;
}

inline void send_query_header(std::ostream& os, std::string_view content_type)
{
    os << "HTTP/1.1 200 OK\r\n";
    os << "Content-Type: " << content_type << "\r\n";
    os << "Access-Control-Allow-Origin: *\r\n"
          "Access-Control-Allow-Headers: Content-Type, Accept, Authorization\r\n"
          "Access-Control-Allow-Methods: GET, POST\r\n"
          "Connection: keep-alive\r\n"
          "Server: MillenniumDB\r\n"
          "\r\n";
}

inline std::string_view get_content_type(GQL::ReturnType return_type)
{
    switch (return_type) {
    case GQL::ReturnType::CSV:
        return "Content-Type: text/csv; charset=utf-8";
    case GQL::ReturnType::TSV:
        return "Content-Type: text/tab-separated-values; charset=utf-8";
    default:
        throw LogicException(
            "Response type not implemented: " + std::to_string(static_cast<int>(return_type))
        );
    }
}

inline std::string_view get_content_type(MQL::ReturnType return_type)
{
    switch (return_type) {
    case MQL::ReturnType::CSV:
        return "Content-Type: text/csv; charset=utf-8";
    case MQL::ReturnType::TSV:
        return "Content-Type: text/tab-separated-values; charset=utf-8";
    default:
        throw LogicException(
            "Response type not implemented: " + std::to_string(static_cast<int>(return_type))
        );
    }
}

inline std::string_view get_content_type(SPARQL::ResponseType return_type)
{
    switch (return_type) {
    case SPARQL::ResponseType::CSV:
        return "Content-Type: text/csv; charset=utf-8";
    case SPARQL::ResponseType::TSV:
        return "Content-Type: text/tab-separated-values; charset=utf-8";
    case SPARQL::ResponseType::JSON:
        return "Content-Type: application/json; charset=utf-8";
    case SPARQL::ResponseType::XML:
        return "Content-Type: application/sparql-results+xml; charset=utf-8";
    case SPARQL::ResponseType::TURTLE:
        return "Content-Type: application/turtle; charset=utf-8";
    default:
        throw LogicException(
            "Response type not implemented: " + SPARQL::response_type_to_string(return_type)
        );
    }
}

} // namespace MDBServer
