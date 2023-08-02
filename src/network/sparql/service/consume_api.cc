// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Official repository: https://github.com/boostorg/beast
#include "consume_api.h"

#include <iostream>
#include <regex>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/beast/version.hpp>
#include <boost/lexical_cast.hpp>


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http  = boost::beast::http;   // from <boost/beast/http.hpp>
namespace ssl   = boost::asio::ssl;     // from <boost/asio/ssl.hpp>
using tcp       = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

// POST to API, overrides the response and format params
int consume(bool https,
            const std::string& host,
            const std::string& port,
            const std::string& target,
            const std::string& body,
            std::string& response,
            Format& format){
    try {
        int version = 11;
        http::verb method = http::verb::post;

        // Set up an HTTP request message
        http::request<http::string_body> request{method, target, version};
        request.method(method);
        request.target(target);
        request.keep_alive(false);
        request.set(http::field::host, host);
        request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        request.set(http::field::content_type, "application/sparql-query");
        switch (format) {
            case Format::tsv: {
                request.set(http::field::accept, "text/tab-separated-values");
                break;
            }
            case Format::json: {
                request.set(http::field::accept, "application/sparql-results+json");
                break;
            }
            case Format::xml: {
                request.set(http::field::accept, "application/sparql-results+xml");
                break;
            }
            case Format::csv: {
                request.set(http::field::accept, "application/text/csv");
                break;
            }
        }
        request.content_length(body.length());
        request.body() = body;

        beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
        http::response_parser<http::string_body> parser; // Declare a container to hold the response
        parser.body_limit((std::numeric_limits<std::uint64_t>::max)());
        beast::error_code ec;
        boost::asio::io_context ctx; // The io_context is required for all I/O

        if(https){
            // The SSL context is required, and certificates are not validated
            ssl::context ssl_ctx(ssl::context::tlsv12_client);
            ssl_ctx.set_verify_mode(ssl::context::verify_none); // set no validation
            ssl_ctx.set_default_verify_paths();

            boost::asio::ip::tcp::socket socket{ctx};
            tcp::resolver resolver{ctx};
            boost::asio::connect(socket, resolver.resolve(host, port));
            auto stream = boost::make_unique<ssl::stream<boost::asio::ip::tcp::socket>>(std::move(socket), ssl_ctx);
            stream->handshake(ssl::stream_base::handshake_type::client);

            http::write(*stream, request); // Send the HTTP request to the remote host
            http::read(*stream, buffer, parser); // Receive the HTTP response

            stream->shutdown(ec);
            stream->next_layer().close(ec);
        } else {
            tcp::resolver resolver{ctx}; // Performs I/O
            tcp::resolver::results_type const results = resolver.resolve(host, port); // Look up the domain name

            beast::tcp_stream stream(ctx);
            stream.connect(results); // Make the connection on the IP address we get from a lookup

            http::write(stream, request); // Send the HTTP request to the remote host
            http::read(stream, buffer, parser); // Receive the HTTP response

            stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        }

        if(ec && ec != beast::errc::not_connected) throw beast::system_error{ec};

        auto header = parser.get().base();
        std::string content_type = header.at(http::field::content_type);

        // the header can have more than one Content-Type field so we use regex to find any
        std::regex json(R"(application/sparql-results\+json|application/json)");
        std::regex xml(R"(application/sparql-results\+xml)");
        std::regex tsv(R"(text/tab-separated-values)");
        std::regex csv(R"(text/csv)");
        // override format with response format, not always is the same as requested
        if (std::regex_search(content_type, tsv)) format = Format::tsv;
        else if (std::regex_search(content_type, json)) format = Format::json;
        else if (std::regex_search(content_type, xml)) format = Format::xml;
        else if (std::regex_search(content_type, csv)) format = Format::csv;

        response = boost::lexical_cast<std::string>(parser.get().body()); // set response
        return parser.get().result_int(); // response status

    } catch(std::exception& e) {
        throw std::runtime_error("Consume API bad request to " + host + ": " + std::string{e.what()});
    }
}
