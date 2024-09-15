#include <exception>
#include <iostream>

#include <boost/stacktrace.hpp>

#include "graph_models/exceptions.h"
#include "graph_models/quad_model/quad_model.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "misc/fatal_error.h"
#include "misc/logger.h"
#include "query/executor/query_executor/mql/return_executor.h"
#include "query/optimizer/quad_model/executor_constructor.h"
#include "query/optimizer/rdf_model/executor_constructor.h"
#include "query/parser/mql_query_parser.h"
#include "query/parser/sparql_query_parser.h"
#include "storage/filesystem.h"
#include "storage/index/tensor_store/lsh/forest_index.h"
#include "storage/index/tensor_store/tensor_buffer_manager.h"
#include "storage/index/tensor_store/tensor_store.h"
#include "system/buffer_manager.h"
#include "system/string_manager.h"
#include "system/system.h"

#include "third_party/cli11/CLI11.hpp"


using DurationMS = std::chrono::duration<float, std::milli>;
using std::chrono::system_clock;

static uint64_t read_uint64(std::fstream& fs) {
    uint64_t res = 0;
    uint8_t buf[8];

    fs.read((char*)buf, sizeof(buf));

    for (int i = 0; i < 8; i++) {
        res |= static_cast<uint64_t>(buf[i]) << (i * 8);
    }

    if (!fs.good()) {
        throw std::runtime_error("Error reading uint64");
    }

    return res;
}


struct PathModeValidator : public CLI::Validator {
    PathModeValidator() {
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



void my_terminate_handler() {
    try {
        std::cerr << boost::stacktrace::stacktrace();
    } catch (...) {}
    std::abort();
}


int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::set_terminate(&my_terminate_handler);

    uint_fast32_t seconds_timeout = 60;

    uint64_t limit = 0;
    uint64_t load_strings = StringManager::DEFAULT_LOAD_STR;
    uint64_t versioned_pages_buffer   = BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE;
    uint64_t private_pages_buffer     = BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE;
    uint64_t unversioned_pages_buffer = BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE;
    uint64_t tensor_pages_buffer      = TensorBufferManager::DEFAULT_TENSOR_PAGES_BUFFER_SIZE;
    bool     preload_tensors          = false;

    std::string db_directory;
    std::string config_path;
    std::string path_mode;
    std::string query_input;

    CLI::App app{"MillenniumDB Server"};
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

    app.add_option("--load-strings", load_strings)
        ->description("Total amount of strings to pre-load\nAllows units such as MB and GB")
        ->option_text("<bytes> [2GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--versioned-buffer", versioned_pages_buffer)
        ->description("Size of buffer for versioned pages shared between threads\nAllows units such as MB and GB")
        ->option_text("<bytes> [1GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--private-buffer", private_pages_buffer)
        ->description("Size of private per-thread buffers,\nAllows units such as MB and GB")
        ->option_text("<bytes> [64MB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--unversioned-buffer", unversioned_pages_buffer)
        ->description("Size of buffer for unversioned pages shared between threads,\nAllows units such as MB and GB")
        ->option_text("<bytes> [128MB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--path-mode", path_mode)
        ->description("Path Mode")
        ->option_text("bfs|dfs")
        ->transform(PathModeValidator());

    app.add_option("--tensor-buffer", tensor_pages_buffer)
      ->description("Size of buffer for tensor pages shared between threads\nAllows units such as MB and GB")
      ->option_text("<bytes> [2GB]")
      ->transform(CLI::AsSizeValue(false))
      ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_flag("--preload-tensors", preload_tensors)
      ->description("Fill the tensor buffer before starting the server");

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
    std::string query(
        (std::istreambuf_iterator<char>(query_file)),
        (std::istreambuf_iterator<char>())
    );

    std::chrono::seconds timeout(seconds_timeout);

    System system(
        db_directory,
        load_strings,
        versioned_pages_buffer,
        private_pages_buffer,
        unversioned_pages_buffer,
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
                        quad_model.path_mode = PathMode::BFS;
                    else if (path_mode == "dfs")
                        quad_model.path_mode = PathMode::DFS;
                }

                quad_model.catalog().print(std::cout);

                TensorStore::load_tensor_stores(tensor_pages_buffer, preload_tensors);

                try {
                    auto version_scope = buffer_manager.init_version_readonly();
                    get_query_ctx().prepare(*version_scope, timeout);

                    auto start_parser = system_clock::now();
                    auto logical_plan = MQL::QueryParser::get_query_plan(query);
                    DurationMS parser_duration = system_clock::now() - start_parser;

                    auto start_optimizer = system_clock::now();

                    MQL::ExecutorConstructor query_optimizer(MQL::ReturnType::CSV);
                    logical_plan->accept_visitor(query_optimizer);

                    DurationMS optimizer_duration = system_clock::now() - start_optimizer;

                    auto physical_plan = std::move(query_optimizer.executor);
                    auto execution_start = system_clock::now();

                    logger.log(Category::ExecutionStats, [&physical_plan] (std::ostream& os) {
                        physical_plan->analyze(os, true);
                        os << '\n';
                    });

                    auto result_count = physical_plan->execute(std::cout);
                    DurationMS execution_duration = system_clock::now() - execution_start;

                    logger(Category::Info)
                        << "Results: " << result_count << '\n'
                        << "Parser duration:    " << parser_duration.count()    << " ms\n"
                        << "Optimizer duration: " << optimizer_duration.count() << " ms\n"
                        << "Execution duration: " << execution_duration.count() << " ms";
                }
                catch (const QueryParsingException& e) {
                    logger(Category::Error) << "Query Parsing Exception. Line " << e.line << ", col: " << e.column << ": " << e.what();

                    std::cout << std::string(e.what());
                }
                catch (const QueryException& e) {
                    logger(Category::Error) << "Query Exception: " << e.what();

                    std::cout << std::string(e.what());
                }
                catch (const LogicException& e) {
                    logger(Category::Error)  << "Logic Exception: " << e.what();

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
                        rdf_model.path_mode = PathMode::BFS;
                    else if (path_mode == "dfs")
                        rdf_model.path_mode = PathMode::DFS;
                }

                rdf_model.catalog().print(std::cout);

                try {
                    auto version_scope = buffer_manager.init_version_readonly();
                    get_query_ctx().prepare(*version_scope, timeout);

                    auto start_parser = system_clock::now();
                    auto logical_plan = SPARQL::QueryParser::get_query_plan(query);
                    DurationMS parser_duration = system_clock::now() - start_parser;

                    auto start_optimizer = system_clock::now();

                    SPARQL::ExecutorConstructor executor_constructor(SPARQL::ResponseType::TSV);
                    logical_plan->accept_visitor(executor_constructor);

                    DurationMS optimizer_duration = system_clock::now() - start_optimizer;

                    auto physical_plan = std::move(executor_constructor.executor);
                    auto execution_start = system_clock::now();

                    logger.log(Category::ExecutionStats, [&physical_plan] (std::ostream& os) {
                        physical_plan->analyze(os, false);
                        os << '\n';
                    });

                    auto result_count = physical_plan->execute(std::cout);
                    DurationMS execution_duration = system_clock::now() - execution_start;

                    logger.log(Category::ExecutionStats, [&physical_plan] (std::ostream& os) {
                        physical_plan->analyze(os, true);
                        os << '\n';
                    });

                    logger(Category::Info)
                        << "Results: " << result_count << '\n'
                        << "Parser duration:    " << parser_duration.count()    << " ms\n"
                        << "Optimizer duration: " << optimizer_duration.count() << " ms\n"
                        << "Execution duration: " << execution_duration.count() << " ms";
                }
                catch (const QueryParsingException& e) {
                    logger(Category::Error) << "Query Parsing Exception. Line " << e.line
                        << ", col: " << e.column << ": " << e.what();

                    std::cout << std::string(e.what());
                }
                catch (const QueryException& e) {
                    logger(Category::Error) << "Query Exception: " << e.what();

                    std::cout << std::string(e.what());
                }
                catch (const LogicException& e) {
                    logger(Category::Error)  << "Logic Exception: " << e.what();

                    std::cout << std::string(e.what());
                }

                return EXIT_SUCCESS;
            }
            default: {
                std::string error = "Unknow model identifier: "
                    + std::to_string(model_identifier)
                    + ". Catalog may be corrupted";
                FATAL_ERROR(error);
            }
        }
    } catch (WrongModelException& e) {
        std::string error = std::string(e.what())
            + ". Catalog may be corrupted";
        FATAL_ERROR(error);
    } catch (WrongCatalogVersionException& e) {
        std::string error = std::string(e.what())
            + ". The database must be created again to work with this version";
        FATAL_ERROR(error);
    }
}
