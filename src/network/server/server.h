#pragma once

#include <chrono>
#include <mutex>
#include <vector>

#include <boost/asio.hpp>

#include "network/server/protocol.h"
#include "query/query_context.h"

namespace MDBServer {

struct LoginInfo {
    std::string user;
    std::string password;
    std::string token;
    std::chrono::system_clock::time_point valid_until;

    LoginInfo(std::string user, std::string password) :
        user(std::move(user)),
        password(std::move(password))
    {
        valid_until = std::chrono::system_clock::now() - std::chrono::hours(1);
    }
};

class Server {
public:
    static inline bool shutdown_server = false;

    // Prevent concurrent updates
    static inline std::mutex update_execution_mutex;

    uint64_t model_id;

    std::vector<QueryContext> query_contexts;

    // Used to prevent synchronization problems between tht timeout thread and the worker thread
    std::mutex thread_info_vec_mutex;

    void run(unsigned short       port,
             unsigned short       browser_port,
             bool                 launch_browser,
             int                  num_threads,
             std::chrono::seconds timeout);

    static void signal_shutdown_server(int signal);

    void execute_timeouts();

    bool try_cancel(uint_fast32_t worker_idx, const std::string& cancel_token);

    bool authorize(Protocol::RequestType, const std::string& auth_token);

    // returns empty string if not authorized
    std::pair<std::string, std::chrono::system_clock::time_point>
        create_auth_token(const std::string& user, const std::string& pass);

    void set_admin_user(const std::string& user, const std::string& password);

private:
    static void browser_session(boost::asio::ip::tcp::socket&& socket);

    static void browser_listener(boost::asio::io_context* browser_io_context, int port);

    std::vector<LoginInfo> users;
};
} // namespace MDBServer
