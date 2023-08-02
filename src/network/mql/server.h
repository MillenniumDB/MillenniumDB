#pragma once

#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <vector>

#include "query/query_context.h"


namespace MQL {

class Server {
public:
    static constexpr int DEFAULT_PORT = 8080;

    std::vector<QueryContext> query_contexts;

    std::mutex thread_info_vec_mutex;

    std::shared_mutex execution_mutex;

    bool shutdown_server;

    void execute_timeouts();

    void run(unsigned short port,
             int worker_threads,
             std::chrono::seconds timeout);
};

} // namespace MQL