#pragma once

#include <boost/beast.hpp>

#include "network/server/protocol.h"
#include "network/sparql/response_type.h"
#include "network/sparql/url_helper.h"

namespace SPARQL {

namespace RequestParser {

inline MDBServer::Protocol::RequestType get_request_type(const std::string_view& str) {
    if (str.find("/sparql") != std::string::npos) {
        return MDBServer::Protocol::RequestType::QUERY;
    }
    if (str.find("/update") != std::string::npos) {
        return MDBServer::Protocol::RequestType::UPDATE;
    }
    if (str.find("/cancel") != std::string::npos) {
        return MDBServer::Protocol::RequestType::CANCEL;
    }
    if (str.find("/auth") != std::string::npos) {
        return MDBServer::Protocol::RequestType::AUTH;
    }
    return MDBServer::Protocol::RequestType::INVALID;
}


inline std::pair<std::string, ResponseType>
parse_query(boost::beast::http::request<boost::beast::http::string_body>& req)
{
    std::string sparql_query;
    ResponseType response_type = ResponseType::JSON;

    std::string content_type;
    std::string auth_token;

    // search in header the "Content-Type" and "Accept"
    for (auto& header : req) {
        auto header_name = header.name_string();
        if (header_name == "Content-Type") {
            content_type = header.value();
        }
        else if (header_name == "Accept") {
            std::string accept_value = header.value();
            if (accept_value.find("text/csv") != std::string::npos) {
                response_type = ResponseType::CSV;
            } else if (accept_value.find("text/tab-separated-values") != std::string::npos) {
                response_type = ResponseType::TSV;
            } else if (accept_value.find("application/sparql-results+xml") != std::string::npos) {
                response_type = ResponseType::XML;
            } else if (accept_value.find("application/turtle")    != std::string::npos
                    || accept_value.find("application/n-triples") != std::string::npos
                    || accept_value.find("text/turtle")           != std::string::npos) {
                response_type = ResponseType::TURTLE;
            }
            // else JSON by default
        }
    }

    std::string url_encoded;

    if (req.method() == boost::beast::http::verb::post) {
        if (content_type == "application/x-www-form-urlencoded") {
            url_encoded = req.body();
        } else if (content_type != "application/sparql-query" &&
                    content_type != "application/sparql-update")
        {
            // This case is invalid according to the SPARQL standard. Try to guess
            std::string req_body = req.body();
            if (req_body.find("query=") != std::string::npos ||
                req_body.find("update=") != std::string::npos)
            {
                url_encoded = req.body();
            }
        }
    } else if (req.method() == boost::beast::http::verb::get) {
        url_encoded = req.target();
    }

    if (url_encoded.empty()) {
        sparql_query = req.body();
    } else {
        const char* ptr = url_encoded.c_str();

        if (*ptr == '/') {
            // params_string_encoded is like "/?query=SELECT+%2A+WHERE+%7B+%3Fs+%3Fp+%3Fo+.+%7D+LIMIT+1000&format=json&output=json&results=json"
            // skip until params starts
            while (*ptr != '?' && *ptr != '\0') {
                ptr++;
            }
            if (*ptr != '\0') ptr++;
        }

        // new *ptr is something like "query=SELECT+%2A+WHERE+%7B+%3Fs+%3Fp+%3Fo+.+%7D+LIMIT+1000&format=json&output=json&results=json"
        // key1=value1&key2=value2&...&key_n=value_n
        // split by '&' and then by '='
        while (*ptr != '\0') {
            auto beg_key = ptr;
            size_t len_key = 0;
            while (*ptr != '\0' && *ptr != '=') {
                ptr++;
                len_key++;
            }
            if (*ptr != '\0') ptr++;

            auto beg_value = ptr;
            size_t len_value = 0;
            while (*ptr != '\0' && *ptr != '&') {
                ptr++;
                len_value++;
            }
            std::string key(beg_key, len_key);
            std::string val(beg_value, len_value);

            if (*ptr != '\0') ptr++;

            // process key/value
            if (key == "query" || key == "update") {
                sparql_query = UrlHelper::decode(val);
            }
            // params can also include 'default-graph-uri' and 'named-graph-uri'. For now we ignore it
        }
    }

    return std::make_pair(sparql_query, response_type);
}

}} // namespace SPARQL::RequestParser
