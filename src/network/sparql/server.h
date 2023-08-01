#pragma once

#include <chrono>
#include <mutex>
#include <vector>

#include "query/query_context.h"

namespace SPARQL {

class Server {
public:
    static constexpr int DEFAULT_PORT = 8080;

    std::vector<QueryContext> query_contexts;

    // used to prevent synchronization problems between tht timeout thread
    // and the worker thread
    std::mutex thread_info_vec_mutex;

    bool shutdown_server;

    void execute_timeouts();

    void run(unsigned short port,
             int worker_threads,
             std::chrono::seconds timeout);
};

} // namespace SPARQL