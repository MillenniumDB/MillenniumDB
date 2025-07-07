#include <exception>
#include <iostream>

#include <boost/stacktrace.hpp>
#include <gperftools/profiler.h>

#include "bin/mdb-server.h"
#include "graph_models/exceptions.h"
#include "graph_models/quad_model/quad_model.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "misc/fatal_error.h"
#include "misc/logger.h"
#include "query/optimizer/quad_model/executor_constructor.h"
#include "query/optimizer/rdf_model/executor_constructor.h"
#include "query/parser/mql_query_parser.h"
#include "query/parser/sparql_query_parser.h"
#include "system/system.h"

using namespace MdbBin;
using DurationMS = std::chrono::duration<float, std::milli>;
using std::chrono::system_clock;

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

    if (argc < 3) {
        FATAL_ERROR("usage: profile-query <database-folder> <query-file> [WARMUPS] [OPTIONS]");
    }

    int start_options = 3;
    int warmups = 0;
    if (argc > 3 && strtol(argv[3], nullptr, 10) != 0) {
        warmups = strtol(argv[3], nullptr, 10);
        start_options = 4;
    }

    std::vector<std::string> args;

    for (int i = start_options; i < argc; i++) {
        // convert things like "--foo=x" to "--foo" and "x"
        std::string input(argv[i]);

        size_t start = 0;
        size_t end = input.find("=");

        while (end != std::string::npos) {
            args.push_back(input.substr(start, end - start));
            start = end + 1;
            end = input.find("=", start);
        }

        // Add the last substring
        args.push_back(input.substr(start));
    }

    auto config = parse_profile_config(argv[1], args);
    auto model_id = Catalog::get_model_id(config.db_directory);

    if (config.query_timeout == MDBServer::Protocol::DEFAULT_QUERY_TIMEOUT_SECONDS) {
        config.query_timeout = 10 * MDBServer::Protocol::DEFAULT_QUERY_TIMEOUT_SECONDS;
    }

    QueryContext qc;
    QueryContext::set_query_ctx(&qc);

    std::ifstream query_file(argv[2]);
    std::string query((std::istreambuf_iterator<char>(query_file)), (std::istreambuf_iterator<char>()));

    System system(
        config.db_directory,
        config.strings_static_buffer,
        config.strings_dynamic_buffer,
        config.versioned_pages_buffer,
        config.private_pages_buffer,
        config.unversioned_pages_buffer,
        config.tensors_static_buffer,
        config.tensors_dynamic_buffer,
        config.workers
    );

    try {
        switch (model_id) {
        case Catalog::ModelID::QUAD: {
            std::cout << "Initializing Quad Model..." << std::endl;
            auto model_destroyer = QuadModel::init();

            quad_model.path_mode = config.path_mode;
            if (config.limit != 0) {
                quad_model.MAX_LIMIT = config.limit;
            }

            quad_model.catalog.print(std::cout);

            try {
                for (int i = 0; i < warmups; i++) {
                    auto version_scope = buffer_manager.init_version_readonly();
                    get_query_ctx().prepare(*version_scope, config.query_timeout);

                    auto logical_plan = MQL::QueryParser::get_query_plan(query);
                    MQL::ExecutorConstructor query_optimizer(MQL::ReturnType::CSV);
                    logical_plan->accept_visitor(query_optimizer);

                    auto physical_plan = std::move(query_optimizer.executor);
                    physical_plan->execute(std::cout);
                }

                ProfilerStart("profile_query");
                auto version_scope = buffer_manager.init_version_readonly();
                get_query_ctx().prepare(*version_scope, config.query_timeout);

                auto start_parser = system_clock::now();
                auto logical_plan = MQL::QueryParser::get_query_plan(query);
                DurationMS parser_duration = system_clock::now() - start_parser;

                auto start_optimizer = system_clock::now();

                MQL::ExecutorConstructor query_optimizer(MQL::ReturnType::CSV);
                logical_plan->accept_visitor(query_optimizer);

                DurationMS optimizer_duration = system_clock::now() - start_optimizer;

                auto physical_plan = std::move(query_optimizer.executor);
                auto execution_start = system_clock::now();

                logger.log(Category::ExecutionStats, [&physical_plan](std::ostream& os) {
                    physical_plan->analyze(os, true);
                    os << '\n';
                });

                auto result_count = physical_plan->execute(std::cout);
                DurationMS execution_duration = system_clock::now() - execution_start;

                logger(Category::Info) << "Results: " << result_count << "\n"
                                       << "Parser duration:    " << parser_duration.count() << " ms\n"
                                       << "Optimizer duration: " << optimizer_duration.count() << " ms\n"
                                       << "Execution duration: " << execution_duration.count() << " ms";
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
        case Catalog::ModelID::RDF: {
            std::cout << "Initializing RDF Model..." << std::endl;
            auto model_destroyer = RdfModel::init();

            rdf_model.path_mode = config.path_mode;
            if (config.limit != 0) {
                rdf_model.MAX_LIMIT = config.limit;
            }

            rdf_model.catalog.print(std::cout);

            try {
                for (int i = 0; i < warmups; i++) {
                    auto version_scope = buffer_manager.init_version_readonly();
                    get_query_ctx().prepare(*version_scope, config.query_timeout);

                    auto logical_plan = SPARQL::QueryParser::get_query_plan(query);
                    SPARQL::ExecutorConstructor executor_constructor(SPARQL::ResponseType::TSV);
                    logical_plan->accept_visitor(executor_constructor);

                    auto physical_plan = std::move(executor_constructor.executor);
                    physical_plan->execute(std::cout);
                }

                ProfilerStart("profile_query");
                auto version_scope = buffer_manager.init_version_readonly();
                get_query_ctx().prepare(*version_scope, config.query_timeout);

                auto start_parser = system_clock::now();
                auto logical_plan = SPARQL::QueryParser::get_query_plan(query);
                DurationMS parser_duration = system_clock::now() - start_parser;

                auto start_optimizer = system_clock::now();

                SPARQL::ExecutorConstructor executor_constructor(SPARQL::ResponseType::TSV);
                logical_plan->accept_visitor(executor_constructor);

                DurationMS optimizer_duration = system_clock::now() - start_optimizer;

                auto physical_plan = std::move(executor_constructor.executor);
                auto execution_start = system_clock::now();

                logger.log(Category::ExecutionStats, [&physical_plan](std::ostream& os) {
                    physical_plan->analyze(os, false);
                    os << '\n';
                });

                auto result_count = physical_plan->execute(std::cout);
                DurationMS execution_duration = system_clock::now() - execution_start;

                logger.log(Category::ExecutionStats, [&physical_plan](std::ostream& os) {
                    physical_plan->analyze(os, true);
                    os << '\n';
                });

                logger(Category::Info) << "Results: " << result_count << "\n"
                                       << "Parser duration:    " << parser_duration.count() << " ms\n"
                                       << "Optimizer duration: " << optimizer_duration.count() << " ms\n"
                                       << "Execution duration: " << execution_duration.count() << " ms";

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

            ProfilerStop();
            return EXIT_SUCCESS;
        }
        case Catalog::ModelID::GQL: {
            FATAL_ERROR("GQL query profile not implemented yet");
        }
        }
    } catch (const WrongModelException& e) {
        FATAL_ERROR(e.what());
    } catch (const WrongCatalogVersionException& e) {
        FATAL_ERROR(e.what());
    }
}
