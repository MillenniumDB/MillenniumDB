#pragma once

#include <boost/beast.hpp>

#include "network/server/protocol.h"
#include "network/sparql/response_type.h"
#include "network/sparql/url_helper.h"
#include "query/executor/query_executor/gql/return_executor.h"

namespace GQL { namespace RequestParser {

inline MDBServer::Protocol::RequestType get_request_type(const std::string_view& /* str */)
{
    // if (str.find("/update") != std::string::npos) {
    //     return MDBServer::Protocol::RequestType::UPDATE;
    // }
    // if (str.find("/cancel") != std::string::npos) {
    //     return MDBServer::Protocol::RequestType::CANCEL;
    // }
    // if (str.find("/auth") != std::string::npos) {
    //     return MDBServer::Protocol::RequestType::AUTH;
    // }
    return MDBServer::Protocol::RequestType::QUERY;
}

inline std::pair<std::string, ReturnType>
    parse_query(boost::beast::http::request<boost::beast::http::string_body>& req)
{
    ReturnType response_type = ReturnType::CSV;
    std::string content_type;
    for (auto& header : req) {
        std::string header_name = header.name_string();
        if (header_name == "Accept") {
            std::string accept_value = header.value();
            if (accept_value.find("text/tab-separated-values") != std::string::npos) {
                response_type = ReturnType::TSV;
            }
        } else if (header_name == "Content-Type") {
            content_type = header.value();
        }
    }

    std::string url_encoded;
    if (req.method() == boost::beast::http::verb::post) {
        if (content_type == "application/x-www-form-urlencoded") {
            url_encoded = req.body();
        }
    } else if (req.method() == boost::beast::http::verb::get) {
        url_encoded = req.target();
    }

    std::string query;
    if (!url_encoded.empty()) {
        const char* ptr = url_encoded.c_str();
        if (*ptr == '/') {
            while (*ptr != '?' && *ptr != '\0') {
                ptr++;
            }
            if (*ptr != '\0')
                ptr++;
        }
        while (*ptr != '\0') {
            auto beg_key = ptr;
            size_t len_key = 0;
            while (*ptr != '\0' && *ptr != '=') {
                ptr++;
                len_key++;
            }
            if (*ptr != '\0')
                ptr++;
            auto beg_value = ptr;
            size_t len_value = 0;
            while (*ptr != '\0' && *ptr != '&') {
                ptr++;
                len_value++;
            }
            std::string key(beg_key, len_key);
            std::string val(beg_value, len_value);
            if (*ptr != '\0')
                ptr++;
            if (key == "query") {
                query = UrlHelper::decode(val);
            }
        }
    } else {
        query = req.body();
    }

    return std::make_pair(query, response_type);
}
}} // namespace GQL::RequestParser
