#include <iostream>

#include <thread>

#include "graph_models/exceptions.h"
#include "graph_models/gql/gql_model.h"
#include "graph_models/quad_model/quad_model.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "misc/fatal_error.h"
#include "misc/logger.h"
#include "network/server/protocol.h"
#include "network/server/server.h"
#include "storage/index/tensor_store/tensor_store_manager.h"
#include "storage/filesystem.h"
#include "system/buffer_manager.h"
#include "system/string_manager.h"
#include "system/system.h"

#include "third_party/cli11/CLI11.hpp"

static uint64_t read_uint64(std::fstream& fs)
{
    uint64_t res = 0;
    uint8_t buf[8];

    fs.read((char*) buf, sizeof(buf));

    for (int i = 0; i < 8; i++) {
        res |= static_cast<uint64_t>(buf[i]) << (i * 8);
    }

    if (!fs.good()) {
        throw std::runtime_error("Error reading uint64");
    }

    return res;
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);

    bool no_browser = false;

    auto query_timeout_seconds = MDBServer::Protocol::DEFAULT_QUERY_TIMEOUT_SECONDS;
    auto port = MDBServer::Protocol::DEFAULT_PORT;
    auto browser_port = MDBServer::Protocol::DEFAULT_BROWSER_PORT;
    auto max_threads = std::thread::hardware_concurrency();

    uint64_t limit = 0;
    uint64_t strings_static_buffer = StringManager::DEFAULT_STATIC_BUFFER;
    uint64_t strings_dynamic_buffer = StringManager::DEFAULT_DYNAMIC_BUFFER;
    uint64_t versioned_pages_buffer = BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE;
    uint64_t private_pages_buffer = BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE;
    uint64_t unversioned_pages_buffer = BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE;
    uint64_t tensor_store_buffer      = TensorStoreManager::DEFAULT_TENSOR_BUFFER_SIZE;

    std::string db_directory;
    std::string config_path;
    std::string path_mode;

    std::string admin_user;
    std::string admin_password;

    CLI::App app { "MillenniumDB Server" };
    app.get_formatter()->column_width(35);
    app.option_defaults()->always_capture_default();

    app.add_option("db-directory", db_directory)
        ->description("Database directory")
        ->type_name("<path>")
        ->check(CLI::ExistingDirectory.description(""))
        ->required();

    app.add_option("--admin-user", admin_user)->description("if set updates need authentication");

    app.add_option("--admin-password", admin_password)->description("sets the admin user password");

    app.add_option("-p,--port", port)
        ->description("Server port")
        ->type_name("<port>")
        ->check(CLI::Range(1024, 65'535).description(""));

    app.add_option("--browser-port", browser_port)
        ->description("Browser interface port")
        ->type_name("<port>")
        ->check(CLI::Range(1024, 65'535).description(""));

    app.add_option("-t,--timeout", query_timeout_seconds)
        ->description("Timeout for query executions in seconds")
        ->type_name("<seconds>")
        ->check(CLI::Range(1, 36'000).description(""));

    app.add_option("-l,limit", limit)
        ->description("Maximum number of results to return\nPass 0 to set no limit")
        ->type_name("<num_records>")
        ->check(CLI::Range(static_cast<uint64_t>(0ULL), static_cast<uint64_t>(UINT64_MAX)).description(""));

    app.add_option("-j,--threads", max_threads)
        ->description("Number of worker threads")
        ->type_name("<number>")
        ->check(CLI::Range(1, 128).description(""));

    app.add_option("--strings-static", strings_static_buffer)
        ->description("Total amount of strings to pre-load\nAllows units such as MB and GB")
        ->option_text("<bytes> [1GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--strings-dynamic", strings_dynamic_buffer)
        ->description("")
        ->option_text("<bytes> [1GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--versioned-buffer", versioned_pages_buffer)
        ->description(
            "Size of buffer for versioned pages shared between threads\nAllows units such as MB and GB"
        )
        ->option_text("<bytes> [1GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--private-buffer", private_pages_buffer)
        ->description("Size of private per-thread buffers,\nAllows units such as MB and GB")
        ->option_text("<bytes> [64MB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--unversioned-buffer", unversioned_pages_buffer)
        ->description(
            "Size of buffer for unversioned pages shared between threads,\nAllows units such as MB and GB"
        )
        ->option_text("<bytes> [128MB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--tensor-buffer", tensor_store_buffer)
      ->description("Size of buffer for tensors shared between threads,\nAllows units such as MB and GB")
      ->option_text("<bytes> [1GB]")
      ->transform(CLI::AsSizeValue(false))
      ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--path-mode", path_mode)
        ->description("Path mode")
        ->type_name("<mode>")
        ->option_text("BFS|DFS")
        ->transform(CLI::Validator(
            [](std::string& input) -> std::string {
                input = CLI::detail::to_lower(input);
                if (input == "bfs" || input == "dfs")
                    return "";
                else
                    return std::string("Expecting BFS|DFS");
            },
            "Validates the path mode",
            "path_mode_validator"
        ));

    app.add_flag("--no-browser", no_browser)->description("Disable browser interface");

    CLI11_PARSE(app, argc, argv);

    if (!config_path.empty()) {
        if (!Filesystem::exists(config_path)) {
            std::cerr << "Configuration file does not exist: " << config_path << "\n";
            return EXIT_FAILURE;
        } else if (!Filesystem::is_regular_file(config_path)) {
            std::cerr << "Configuration file is not a file: " << config_path << "\n";
            return EXIT_FAILURE;
        }
        if (logger.read_config(config_path)) {
            return EXIT_FAILURE;
        }
    }

    auto catalog_path = db_directory + "/catalog.dat";
    auto catalog_fs = std::fstream(catalog_path, std::ios::in | std::ios::binary);
    if (!catalog_fs.is_open()) {
        std::cerr << "Could not open catalog: " << catalog_path << "\n";
        return 1;
    }

    System system(
        db_directory,
        strings_static_buffer,
        strings_dynamic_buffer,
        versioned_pages_buffer,
        private_pages_buffer,
        unversioned_pages_buffer,
        tensor_store_buffer,
        max_threads
    );

    auto model_identifier = read_uint64(catalog_fs);
    MDBServer::Server server;
    if (!admin_user.empty()) {
        server.set_admin_user(admin_user, admin_password);
    }
    try {
        switch (model_identifier) {
        case QuadCatalog::MODEL_ID: {
            std::cout << "Initializing Quad Model...\n";

            auto model_destroyer = QuadModel::init();

            if (limit != 0) {
                quad_model.MAX_LIMIT = limit;
            }

            if (!path_mode.empty()) {
                quad_model.path_mode = path_mode == "dfs" ? PathSearchMode::DFS : PathSearchMode::BFS;
            }

            quad_model.catalog.print(std::cout);

            server.model_id = MDBServer::Protocol::QUAD_MODEL_ID;
            server.run(
                port,
                browser_port,
                !no_browser,
                max_threads,
                std::chrono::seconds(query_timeout_seconds)
            );
            return EXIT_SUCCESS;
        }
        case RdfCatalog::MODEL_ID: {
            std::cout << "Initializing RDF Model...\n";

            auto model_destroyer = RdfModel::init();

            if (limit != 0) {
                rdf_model.MAX_LIMIT = limit;
            }

            if (!path_mode.empty()) {
                rdf_model.path_mode = path_mode == "dfs" ? PathSearchMode::DFS : PathSearchMode::BFS;
            }

            rdf_model.catalog.print(std::cout);

            server.model_id = MDBServer::Protocol::RDF_MODEL_ID;
            server.run(
                port,
                browser_port,
                !no_browser,
                max_threads,
                std::chrono::seconds(query_timeout_seconds)
            );
            return EXIT_SUCCESS;
        }
        case GQLCatalog::MODEL_ID: {
            std::cout << "Initializing GQL Model...\n";

            auto model_destroyer = GQLModel::init();

            // if (limit != 0) {
            //     gql_model.MAX_LIMIT = limit;
            // }

            // if (!path_mode.empty()) {
            //     gql_model.path_mode = path_mode == "dfs" ? PathSearchMode::DFS : PathSearchMode::BFS;
            // }

            gql_model.catalog.print(std::cout);

            server.model_id = MDBServer::Protocol::GQL_MODEL_ID;
            server.run(
                port,
                browser_port,
                !no_browser,
                max_threads,
                std::chrono::seconds(query_timeout_seconds)
            );
            return EXIT_SUCCESS;
        }
        default:
            throw WrongModelException("Unknown model identifier: " + std::to_string(model_identifier));
        }
    } catch (const WrongModelException& e) {
        FATAL_ERROR(e.what(), ". Catalog may be corrupted");
    } catch (WrongCatalogVersionException& e) {
        FATAL_ERROR(e.what(), ". The database must be created again to work with this version");
    }

    return EXIT_SUCCESS;
}
