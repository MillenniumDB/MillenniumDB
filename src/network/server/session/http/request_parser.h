#pragma once

#include <boost/beast.hpp>

#include "network/server/protocol.h"
#include "network/sparql/response_type.h"

namespace Common {
namespace RequestParser {

inline std::string
get_auth(boost::beast::http::request<boost::beast::http::string_body>& req)
{
    for (auto& header : req) {
        if (header.name_string() == "Authorization") {
            std::string header_val = header.value();
            // Expect something like
            // "Bearer dd5cdc4a-a746-4075-852d-0d58a8a928ce"

            auto ptr = header_val.c_str();

            while (*ptr != '\0' && *ptr != ' ')
                ptr++;

            if (*ptr == '\0') return "";

            auto end_ptr = ptr;

            while (*end_ptr != '\0' && *end_ptr != ' ')
                end_ptr++;

            return std::string(ptr, end_ptr - ptr);
        }
    }
    return "";
}


inline std::tuple<std::string, std::string>
parse_auth(boost::beast::http::request<boost::beast::http::string_body>& req)
{
    // Expect body to be like:
    // "user:password"

    auto body = req.body();
    const char* start_usr_ptr = body.c_str();
    const char* end_usr_ptr = start_usr_ptr;

    while (*end_usr_ptr != '\0' && *end_usr_ptr != ':') {
        end_usr_ptr++;
    }

    const char* start_passwd_ptr = end_usr_ptr;
    if (*start_passwd_ptr != '\0') {
        start_passwd_ptr++;
    }

    return std::make_pair(
        std::string(start_usr_ptr, end_usr_ptr - start_usr_ptr),
        std::string(start_passwd_ptr)
    );
}


inline std::tuple<uint_fast32_t, std::string>
parse_cancel(boost::beast::http::request<boost::beast::http::string_body>& req)
{
    // Expect body to be like:
    // "worker_id:token"
    // example: "1:dd5cdc4a-a746-4075-852d-0d58a8a928ce"

    auto body = req.body();
    char* token_p;
    uint_fast32_t worker_id = std::strtol(body.c_str(), &token_p, 10);

    // in case body is malformed
    if (*token_p != '\0')
        token_p++;

    return std::make_pair(worker_id, std::string(token_p));
}

}} // namespace Common::RequestParser
