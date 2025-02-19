#include <exception>
#include <iostream>

#include <boost/stacktrace.hpp>

#include "graph_models/exceptions.h"
#include "graph_models/quad_model/quad_model.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "misc/fatal_error.h"
#include "misc/logger.h"
#include "query/parser/mql_query_parser.h"
#include "query/parser/sparql_update_parser.h"
#include "storage/index/tensor_store/tensor_store_manager.h"
#include "storage/filesystem.h"
#include "system/buffer_manager.h"
#include "system/string_manager.h"
#include "system/system.h"
#include "update/mql/update_executor.h"
#include "update/sparql/update_executor.h"

#include "third_party/cli11/CLI11.hpp"

using DurationMS = std::chrono::duration<float, std::milli>;
using std::chrono::system_clock;

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

struct PathModeValidator : public CLI::Validator {
    PathModeValidator()
    {
        name_ = "path_mode_validator";
        func_ = [](std::string& str) -> std::string {
            str = CLI::detail::to_lower(str);
            if (str == "dfs" || str == "bfs")
                return "";
            else
                return std::string("expecting bfs|dfs");
        };
    }
};

void my_terminate_handler()
{
    try {
        std::cerr << boost::stacktrace::stacktrace();
    } catch (...) {
    }
    std::abort();
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::set_terminate(&my_terminate_handler);

    uint_fast32_t seconds_timeout = 60;

    uint64_t limit = 0;
    uint64_t strings_static_buffer = StringManager::DEFAULT_STATIC_BUFFER;
    uint64_t strings_dynamic_buffer = StringManager::DEFAULT_DYNAMIC_BUFFER;
    uint64_t private_pages_buffer = BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE;
    uint64_t versioned_pages_buffer = BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE;
    uint64_t unversioned_pages_buffer = BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE;
    uint64_t tensor_store_buffer = TensorStoreManager::DEFAULT_TENSOR_BUFFER_SIZE;

    std::string path_mode;
    std::string config_path;
    std::string query_input;
    std::string db_directory;

    CLI::App app { "MillenniumDB Server" };
    app.get_formatter()->column_width(35);
    app.option_defaults()->always_capture_default();

    app.add_option("database", db_directory)
        ->description("Database directory")
        ->type_name("<path>")
        ->check(CLI::ExistingDirectory.description(""))
        ->required();

    app.add_option("query", query_input)
        ->description("Text file with the query")
        ->type_name("<path>")
        ->check(CLI::ExistingFile.description(""))
        ->required();

    app.add_option("-t,--timeout", seconds_timeout)
        ->description("Query timeout in seconds")
        ->type_name("<seconds>")
        ->check(CLI::Range(1, 100'000).description(""));

    app.add_option("-l,--limit", limit)
        ->description("Maximum number of results returned\nPass 0 to set no limit")
        ->type_name("<rows>")
        ->check(CLI::Range(static_cast<uint64_t>(0), static_cast<uint64_t>(UINT64_MAX)).description(""));

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
        ->description("Size of buffer for versioned pages shared between threads\nAllows units "
                      "such as MB and GB")
        ->option_text("<bytes> [1GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--private-buffer", private_pages_buffer)
        ->description("Size of private per-thread buffers,\nAllows units such as MB and GB")
        ->option_text("<bytes> [64MB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--unversioned-buffer", unversioned_pages_buffer)
        ->description("Size of buffer for unversioned pages shared between threads,\nAllows units "
                      "such as MB and GB")
        ->option_text("<bytes> [128MB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--tensor-buffer", tensor_store_buffer)
        ->description("Size of buffer for tensor store\nAllows units such as MB and GB")
        ->option_text("<bytes> [1GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--path-mode", path_mode)
        ->description("Path Mode")
        ->option_text("bfs|dfs")
        ->transform(PathModeValidator());

    CLI11_PARSE(app, argc, argv);

    if (!config_path.empty()) {
        if (!Filesystem::exists(config_path)) {
            std::cerr << "Config file does not exist: " << config_path << "\n";
            return 1;
        } else if (!Filesystem::is_regular_file(config_path)) {
            std::cerr << "Config file is not a file: " << config_path << "\n";
            return 1;
        }
        if (logger.read_config(config_path)) {
            return 1;
        }
    }

    auto catalog_path = db_directory + "/catalog.dat";
    auto catalog_fs = std::fstream(catalog_path, std::ios::in | std::ios::binary);
    if (!catalog_fs.is_open()) {
        std::cerr << "Could not open catalog: " << catalog_path << "\n";
        return 1;
    }
    auto model_identifier = read_uint64(catalog_fs);

    QueryContext qc;
    QueryContext::set_query_ctx(&qc);

    std::ifstream query_file(query_input);
    std::string query((std::istreambuf_iterator<char>(query_file)), (std::istreambuf_iterator<char>()));

    std::chrono::seconds timeout(seconds_timeout);

    System system(
        db_directory,
        strings_static_buffer,
        strings_dynamic_buffer,
        versioned_pages_buffer,
        private_pages_buffer,
        unversioned_pages_buffer,
        tensor_store_buffer,
        1
    );
    try {
        switch (model_identifier) {
        case QuadCatalog::MODEL_ID: {
            std::cout << "Initializing Quad Model server...\n";

            auto model_destroyer = QuadModel::init();

            if (limit != 0) {
                quad_model.MAX_LIMIT = limit;
            }

            if (!path_mode.empty()) {
                if (path_mode == "bfs")
                    quad_model.path_mode = PathSearchMode::BFS;
                else if (path_mode == "dfs")
                    quad_model.path_mode = PathSearchMode::DFS;
            }

            quad_model.catalog.print(std::cout);

            try {
                auto version_scope = buffer_manager.init_version_editable();
                get_query_ctx().prepare(*version_scope, timeout);

                auto start_parser = system_clock::now();
                auto logical_plan = MQL::QueryParser::get_query_plan(query);
                DurationMS parser_duration = system_clock::now() - start_parser;

                auto execution_start = std::chrono::system_clock::now();
                MQL::UpdateExecutor update_executor;
                update_executor.execute(*logical_plan);
                auto execution_duration = std::chrono::system_clock::now() - execution_start;

                logger.log(Category::ExecutionStats, [&update_executor](std::ostream& os) {
                    update_executor.print_stats(os);
                });

                logger(Category::Info) << "Parser duration:    " << parser_duration.count() << " ms\n"
                                          "Execution duration: " << execution_duration.count() << " ms";
            } catch (const QueryParsingException& e) {
                logger(Category::Error) << "Query Parsing Exception. Line " << e.line << ", col: " << e.column
                                        << ": " << e.what();

                std::cout << std::string(e.what());
            } catch (const QueryException& e) {
                logger(Category::Error) << "Query Exception: " << e.what();

                std::cout << std::string(e.what());
            } catch (const LogicException& e) {
                logger(Category::Error) << "Logic Exception: " << e.what();

                std::cout << std::string(e.what());
            }

            return EXIT_SUCCESS;
        }
        case RdfCatalog::MODEL_ID: {
            std::cout << "Initializing RDF Model server..." << std::endl;

            auto model_destroyer = RdfModel::init();

            if (limit != 0) {
                rdf_model.MAX_LIMIT = limit;
            }

            if (!path_mode.empty()) {
                if (path_mode == "bfs")
                    rdf_model.path_mode = PathSearchMode::BFS;
                else if (path_mode == "dfs")
                    rdf_model.path_mode = PathSearchMode::DFS;
            }

            rdf_model.catalog.print(std::cout);

            try {
                // auto version_scope = buffer_manager.init_version_readonly();
                auto version_scope = buffer_manager.init_version_editable();
                get_query_ctx().prepare(*version_scope, timeout);

                auto logical_plan = SPARQL::UpdateParser::get_query_plan(query);

                SPARQL::UpdateExecutor update_executor;
                for (auto& update : logical_plan->updates) {
                    update->accept_visitor(update_executor);
                }

                logger.log(Category::ExecutionStats, [&update_executor](std::ostream& os) {
                    os << "Update Stats\n";
                    update_executor.print_stats(os);
                });
            } catch (const QueryParsingException& e) {
                logger(Category::Error) << "Query Parsing Exception. Line " << e.line << ", col: " << e.column
                                        << ": " << e.what();

                std::cout << std::string(e.what());
            } catch (const QueryException& e) {
                logger(Category::Error) << "Query Exception: " << e.what();

                std::cout << std::string(e.what());
            } catch (const LogicException& e) {
                logger(Category::Error) << "Logic Exception: " << e.what();

                std::cout << std::string(e.what());
            }

            return EXIT_SUCCESS;
        }
        default: {
            FATAL_ERROR("Unknow model identifier: ", model_identifier, ". Catalog may be corrupted");
        }
        }
    } catch (WrongModelException& e) {
        FATAL_ERROR(e.what(), ". Catalog may be corrupted");
    } catch (WrongCatalogVersionException& e) {
        FATAL_ERROR(e.what(), ". The database must be created again to work with this version");
    }
}
