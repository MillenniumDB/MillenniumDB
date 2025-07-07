#include <exception>
#include <iostream>

#include <boost/stacktrace.hpp>

#include "bin/mdb-server.h"
#include "graph_models/exceptions.h"
#include "graph_models/quad_model/quad_model.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "misc/fatal_error.h"
#include "misc/logger.h"
#include "query/parser/mql_query_parser.h"
#include "query/parser/sparql_update_parser.h"
#include "system/system.h"
#include "update/mql/update_executor.h"
#include "update/sparql/update_executor.h"

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

int main(int argc, const char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::set_terminate(&my_terminate_handler);

    if (argc < 2) {
        FATAL_ERROR("usage: profile-update <database-folder> <query-file> [OPTIONS]");
    }

    std::vector<std::string> args;

    for (int i = 3; i < argc; i++) {
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
                auto version_scope = buffer_manager.init_version_editable();
                get_query_ctx().prepare(*version_scope, config.query_timeout);

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

                logger(Category::Info) << "Parser duration:    " << parser_duration.count()
                                       << " ms\n"
                                          "Execution duration: "
                                       << execution_duration.count() << " ms";
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
            break;
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
                auto version_scope = buffer_manager.init_version_editable();
                get_query_ctx().prepare(*version_scope, config.query_timeout);

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
            break;
        }
        case Catalog::ModelID::GQL: {
            FATAL_ERROR("GQL update profile not implemented yet");
        }
        } // end switch
    } catch (const WrongModelException& e) {
        FATAL_ERROR(e.what());
    } catch (const WrongCatalogVersionException& e) {
        FATAL_ERROR(e.what());
    }

    return EXIT_SUCCESS;
}
