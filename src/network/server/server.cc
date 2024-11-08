#include "server.h"

#include <chrono>
#include <csignal>
#include <thread>
#include <vector>

#include <boost/beast.hpp>

#include "misc/logger.h"
#include "network/server/protocol.h"
#include "network/server/listener.h"
#include "query/query_context.h"

using namespace MDBServer;
using namespace boost;
using tcp      = asio::ip::tcp;
namespace http = boost::beast::http;


// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
inline std::string path_cat(std::string_view base, std::string_view path) {
    if (base.empty())
        return std::string(path);
    std::string result(base);
#ifdef BOOST_MSVC
    char constexpr path_separator = '\\';
    if (result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for (auto& c : result)
        if (c == '/')
            c = path_separator;
#else
    char constexpr path_separator = '/';
    if (result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
#endif
    return result;
}


std::string get_mime_type(const std::string& path) {
    using boost::beast::iequals;

    const auto  last_dot_pos = path.rfind('.');
    std::string extension    = "";
    if (last_dot_pos != std::string::npos) {
        extension = path.substr(last_dot_pos + 1);
    }

    if (iequals(extension, "htm"))
        return "text/html";
    if (iequals(extension, "html"))
        return "text/html";
    if (iequals(extension, "php"))
        return "text/html";
    if (iequals(extension, "css"))
        return "text/css";
    if (iequals(extension, "txt"))
        return "text/plain";
    if (iequals(extension, "js"))
        return "application/javascript";
    if (iequals(extension, "json"))
        return "application/json";
    if (iequals(extension, "xml"))
        return "application/xml";
    if (iequals(extension, "swf"))
        return "application/x-shockwave-flash";
    if (iequals(extension, "flv"))
        return "video/x-flv";
    if (iequals(extension, "png"))
        return "image/png";
    if (iequals(extension, "jpe"))
        return "image/jpeg";
    if (iequals(extension, "jpeg"))
        return "image/jpeg";
    if (iequals(extension, "jpg"))
        return "image/jpeg";
    if (iequals(extension, "gif"))
        return "image/gif";
    if (iequals(extension, "bmp"))
        return "image/bmp";
    if (iequals(extension, "ico"))
        return "image/vnd.microsoft.icon";
    if (iequals(extension, "tiff"))
        return "image/tiff";
    if (iequals(extension, "tif"))
        return "image/tiff";
    if (iequals(extension, "svg"))
        return "image/svg+xml";
    if (iequals(extension, "svgz"))
        return "image/svg+xml";
    return "application/text";
}

template <bool isRequest, class Body, class Fields>
void write(beast::tcp_stream& stream, http::message<isRequest, Body, Fields>&& msg, beast::error_code& ec) {
    (void) beast::http::write(stream, msg, ec);
    if (ec) {
        logger(Category::Error) << "Browser write error: " << ec.message();
    }
}


void MDBServer::Server::browser_session(tcp::socket&& socket) {
    http::request<http::string_body> req = {};

    beast::tcp_stream stream { std::move(socket) };

    beast::flat_buffer buffer;

    boost::beast::error_code ec;

    while (true) {
        (void) beast::http::read(stream, buffer, req, ec);
        if (ec) {
            if (ec == boost::beast::http::error::end_of_stream) {
                boost::beast::error_code ec;
                (void) stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
            }
            return;
        }

        // Returns a bad request response
        auto const bad_request = [&req](std::string_view why) {
            boost::beast::http::response<boost::beast::http::string_body> res { boost::beast::http::status::bad_request,
                                                                                req.version() };
            res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(boost::beast::http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = std::string(why);
            res.prepare_payload();
            return res;
        };

        // Returns a not found response
        auto const not_found = [&req](std::string_view target) {
            boost::beast::http::response<boost::beast::http::string_body> res { boost::beast::http::status::not_found,
                                                                                req.version() };
            res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(boost::beast::http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = "The resource '" + std::string(target) + "' was not found.";
            res.prepare_payload();
            return res;
        };

        // Returns a server error response
        auto const server_error = [&req](std::string_view what) {
            boost::beast::http::response<boost::beast::http::string_body> res {
                boost::beast::http::status::internal_server_error,
                req.version()
            };
            res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(boost::beast::http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = "An error occurred: '" + std::string(what) + "'";
            res.prepare_payload();
            return res;
        };

        // Make sure we can handle the method
        if (req.method() != boost::beast::http::verb::get && req.method() != boost::beast::http::verb::head) {
            write(stream, bad_request("Unknown HTTP-method"), ec);
            return;
        }

        // Request path must be absolute and not contain "..".
        if (req.target().empty() || req.target()[0] != '/' || req.target().find("..") != std::string_view::npos) {
            write(stream, bad_request("Illegal request-target"), ec);
            return;
        }

        // if ENV MDB_BROWSER is set use that
        char* env_browser = std::getenv("MDB_BROWSER");

        std::string path = env_browser == nullptr ? MDBServer::Protocol::DEFAULT_BROWSER_PATH : env_browser;
        path = path_cat(path, req.target());
        if (req.target().back() == '/')
            path.append("index.html");

        // Attempt to open the file
        boost::beast::error_code                  ec;
        boost::beast::http::file_body::value_type body;
        body.open(path.c_str(), boost::beast::file_mode::scan, ec);

        // Handle the case where the file doesn't exist
        if (ec == boost::beast::errc::no_such_file_or_directory) {
            write(stream, not_found(req.target()), ec);
            return;
        }

        // Handle an unknown error
        if (ec) {
            write(stream, server_error(ec.message()), ec);
            return;
        }

        // Cache the size since we need it after the move
        const auto size = body.size();

        // Respond to HEAD request
        if (req.method() == boost::beast::http::verb::head) {
            boost::beast::http::response<boost::beast::http::empty_body> res { boost::beast::http::status::ok,
                                                                               req.version() };
            res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(boost::beast::http::field::content_type, get_mime_type(path));
            res.content_length(size);
            res.keep_alive(req.keep_alive());
            write(stream, std::move(res), ec);
            return;
        }

        // Respond to GET request
        boost::beast::http::response<boost::beast::http::file_body> res {
            std::piecewise_construct,
            std::make_tuple(std::move(body)),
            std::make_tuple(boost::beast::http::status::ok, req.version())
        };
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, get_mime_type(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());
        write(stream, std::move(res), ec);
        return;
    }
}


void Server::browser_listener(asio::io_context* browser_io_context, int port) {
    // Start the acceptor and listen for connections, dispatching them to the session
    asio::ip::tcp::acceptor acceptor(*browser_io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));

    while (true) {
        asio::ip::tcp::socket socket(*browser_io_context);
        acceptor.accept(socket);

        std::thread(browser_session, std::move(socket)).detach();
    }
}



void Server::run(unsigned short       port,
                 unsigned short       browser_port,
                 bool                 launch_browser,
                 int                  num_workers,
                 std::chrono::seconds query_timeout)
{
    asio::io_context io_context(num_workers);

    Listener listener(*this, io_context, tcp::endpoint(tcp::v4(), port), query_timeout);

    std::signal(SIGTERM, &signal_shutdown_server);
    std::signal(SIGINT, &signal_shutdown_server);

    // Prevent io_context from finishing immediately while creating threads
    auto work_guard = asio::make_work_guard(io_context);

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> threads;
    threads.reserve(num_workers);
    query_contexts.resize(num_workers);
    for (auto i = 0; i < num_workers; ++i) {
        threads.emplace_back([&, i] {
            auto& qc = query_contexts[i];
            QueryContext::set_query_ctx(&qc);
            get_query_ctx().thread_info.worker_index = i;
            io_context.run();
        });
    }

    listener.run();
    work_guard.reset();

    std::cout << "MillenniumDB HTTP/WebSocket server listening on http://localhost:" << port << "\n";

    std::unique_ptr<asio::io_context> browser_io_context;
    if (launch_browser) {
        browser_io_context = std::make_unique<asio::io_context>(1);
        std::thread browser_listener_thread(browser_listener, browser_io_context.get(), browser_port);
        browser_listener_thread.detach();
        std::cout << "MillenniumDB browser interface is available at http://localhost:" << browser_port << "\n";
    }

    std::cout << "\nTo terminate the server, press Ctrl+C" << std::endl;

    execute_timeouts();

    std::cout << "Shutting down server..." << std::endl;
    for (auto& query_ctx : query_contexts) {
        query_ctx.thread_info.interruption_requested = true;
    }

    io_context.stop();
    if (browser_io_context != nullptr) {
        browser_io_context->stop();
    }

    // Wait for all threads in the thread pool to exit
    for (auto& thread : threads)
        thread.join();
}


void Server::signal_shutdown_server(int) {
    shutdown_server = true;
}


void Server::execute_timeouts() {
    while (!shutdown_server) {
        const auto now = std::chrono::system_clock::now();
        {
            const std::lock_guard<std::mutex> lock(thread_info_vec_mutex);
            for (auto& query_ctx : query_contexts) {
                if (query_ctx.thread_info.timeout <= now) {
                    query_ctx.thread_info.interruption_requested = true;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1'000));
    }
}


bool Server::try_cancel(uint_fast32_t worker_idx, const std::string& cancel_token) {
    if (worker_idx >= query_contexts.size())
        return false;

    if (query_contexts[worker_idx].cancellation_token != cancel_token)
        return false;

    query_contexts[worker_idx].thread_info.interruption_requested = true;
    return true;
}


// For now we only have an optional superuser
bool Server::authorize(Protocol::RequestType request_type, const std::string& auth_token) {
    if (users.empty() || request_type != Protocol::RequestType::UPDATE) {
        return true;
    }
    auto now = std::chrono::system_clock::now();
    for (auto& user : users) {
        if (user.token == auth_token && user.valid_until > now) {
            return true;
        }
    }
    return false;
}

std::pair<std::string, std::chrono::system_clock::time_point>
Server::create_auth_token(const std::string& user, const std::string& pass) {
    for (auto& u : users) {
        if (u.user == user && u.password == pass) {
            auto new_token = get_query_ctx().get_rand();
            u.token = new_token;
            u.valid_until = std::chrono::system_clock::now() + std::chrono::hours(1);
            return make_pair(u.token, u.valid_until);
        }
    }
    return make_pair("", std::chrono::system_clock::now());
}


void Server::set_admin_user(const std::string& user, const std::string& password) {
    users.emplace_back(user, password);
}
