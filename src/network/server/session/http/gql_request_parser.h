#pragma once

#include <boost/beast.hpp>

#include "network/server/protocol.h"
#include "network/sparql/response_type.h"
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

    for (auto& header : req) {
        std::string accept_value = header.value();
        if (header.name_string() == "Accept") {
            if (accept_value.find("text/tab-separated-values") != std::string::npos) {
                response_type = ReturnType::TSV;
            }
        }
        // else CSV by default
    }

    // We assume that the post body contains the query, otherwise ignore contents
    if (req.method() != boost::beast::http::verb::post) {
        return std::make_pair("", response_type);
    }

    return std::make_pair(req.body(), response_type);
}
}} // namespace GQL::RequestParser
