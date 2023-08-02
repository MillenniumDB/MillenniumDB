#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include "network/sparql/url_helper.h"
#include "network/sparql/server.h"

namespace SPARQL {

class RequestHandler {
public:
    static std::pair<std::string, ResponseType>
      parse_request(boost::beast::http::request<boost::beast::http::string_body>& req)
    {
        // Returns a bad request response
        // auto const request_error = [&req](const std::string why) {
        //     boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::bad_request, req.version()};
        //     res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        //     res.set(boost::beast::http::field::content_type, "application/json");
        //     res.keep_alive(req.keep_alive());
        //     res.body() = "{\"error\": \"" + why + "\"}";
        //     res.prepare_payload();
        //     return res;
        // };

        std::string content_type;
        std::string sparql_query;
        std::string url_encoded;
        std::string output_format;

        ResponseType response_type = ResponseType::JSON;

        for (auto& header : req) {
            if (to_string(header.name()) == "Content-Type") {
                content_type = header.value();
            }
            else if (to_string(header.name()) == "Accept") {
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
                // TODO: maybe check if Accept header specifies JSON, and if not return error?
                // else JSON by default
            }
        }

        bool query_url_encoded = true;
        if (req.method() == boost::beast::http::verb::post) {
            if (content_type == "application/sparql-query") {
                sparql_query = req.body();
                query_url_encoded = false;
            } else if (content_type == "application/x-www-form-urlencoded") {
                url_encoded = req.body();
                query_url_encoded = true;
            } else {
                // return request_error("Unknown Content-Type"); // TODO: throw exception?
            }
        } else if (req.method() == boost::beast::http::verb::get) {
            url_encoded = req.target();
            query_url_encoded = true;
        }

        if (query_url_encoded) {
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
                if (key == "query") {
                    sparql_query = UrlHelper::decode(val);
                }
                // TODO: params can also include 'default-graph-uri' and 'named-graph-uri'. For now we ignore it
            }
        }

        return std::make_pair(sparql_query, response_type);
    }
};
} // namespace SPARQL
