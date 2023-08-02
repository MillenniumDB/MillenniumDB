#include "server.h"

#include <array>
#include <cstddef>
#include <iostream>

#include <boost/asio.hpp>

#include "network/mql/listener.h"

using namespace boost;
using namespace MQL;

void Server::execute_timeouts() {
    while (!shutdown_server) {
        auto now = std::chrono::system_clock::now();
        {
            std::lock_guard<std::mutex> lock(thread_info_vec_mutex);
            for (auto& query_ctx : query_contexts) {
                if (query_ctx.thread_info.timeout <= now) {
                    query_ctx.thread_info.interruption_requested = true;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}


void Server::run(
    unsigned short port,
    int number_of_workers,
    std::chrono::seconds timeout
) {
    shutdown_server = false;

    // The io_context is required for all I/O
    asio::io_context io_context(number_of_workers);

    // Create and launch a listening port
    auto listener = std::make_shared<MQL::Listener>(
        *this,
        io_context,
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port),
        timeout
    );

    // Capture SIGINT and SIGTERM to perform a clean shutdown
    asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait(
        [&](const beast::error_code& /*ec*/, int /*signal_number*/) {
            shutdown_server = true;
            std::cout << "\nShutting down server..." << std::endl;

            // Stop the `io_context`. This will cause `run()`
            // to return immediately, eventually destroying the
            // `io_context` and all of the sockets in it.
            io_context.stop();
        }
    );

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> threads;
    threads.reserve(number_of_workers);
    query_contexts.resize(number_of_workers);
    for (auto i = 0; i < number_of_workers; ++i) {
        threads.emplace_back([&, i] {
            auto& qc = query_contexts[i];
            QueryContext::set_query_ctx(&qc);
            get_query_ctx().thread_info.worker_index = i;
            io_context.run();
        });
    }

    listener->run();

    std::cout << "MillenniumDB Server running on port " << port << std::endl;
    std::cout << "To terminate press CTRL-C" << std::endl;

    execute_timeouts();

    // If we get here, it means we got a SIGINT or SIGTERM

    // Block until all the threads exit
    for (auto& thread : threads) {
        thread.join();
    }
}
