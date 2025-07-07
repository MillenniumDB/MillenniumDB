#include "bin/mdb-csv-import.h"
#include "bin/mdb-dump.h"
#include "bin/mdb-import.h"
#include "bin/mdb-server.h"
#include "cli/cli.h"

using std::cout;
using namespace MdbBin;

void print_mdb_version()
{
    cout << "MillenniumDB v1.0.0\n";
}

void print_server_help()
{
    cout << "\nServer Usage:"
            "\n  mdb server <db_folder> [OPTIONS]"
            "\n"
            "\n  Options:"
            "\n    -j,--threads,--workers <N>         number of worker threads"
            "\n    -p,--port <port>                   server port (default: 1234)"
            "\n    -t,--timeout <seconds>             set query timeout (default: 60)"
            "\n    --browser <true|false>             enable or disable web browser"
            "\n    --browser-port <port>              browser port (default: 4321)"
            "\n    --admin-user <username>            admin username"
            "\n    --admin-password <password>        admin password"
            "\n    --strings-dynamic <bytes>          size for the strings-dynamic-buffer"
            "\n    --strings-static <bytes>           size for static strings-static-buffer"
            "\n    --tensors-dynamic <bytes>          size for the tensors-dynamic-buffer"
            "\n    --tensors-static <bytes>           size for static tensors-static-buffer"
            "\n    --private-buffer <bytes>           size for the private-buffer"
            "\n    --versioned-buffer <bytes>         size for the versioned-buffer"
            "\n    --unversioned-buffer <bytes>       size for the unversioned-buffer"
            "\n";
}

void print_import_help()
{
    cout << "\nImport Usage:"
            "\n  mdb import <files...> <db_folder> [OPTIONS]\n"
            "\n"
            "\n  alternative use with stdinput (useful for importing compressed files)"
            "\n  example:"
            "\n    bzcat file.ttl.bz2 | mdb import <db_folder> --format ttl"
            "\n"
            "\n  Options:"
            "\n    --buffer-strings                   size of buffer for strings used during import (default: 2GB)"
            "\n    --buffer-tensors                   size of buffer for tensors used during import (default: 2GB)"
            "\n    --format                           specify the file format"
            "\n                                         * RDF: [ttl nt n3 rdf]"
            "\n                                         * GQL: [gql]"
            "\n                                         * Quad Model: [qm]"
            "\n  Options for RDF:"
            "\n    --prefixes                         prefixes file path (for IRI compression)"
            "\n    --btree-permutations               3, 4 or 6 (default: 4)"
            "\n";
}

void print_csv_import_help()
{
    cout << "\nCSV Import Usage:"
            "\n  mdb csv-import <gql|quad> <db_folder> --nodes <node_files...> --edges <edge_files> [OPTIONS]"
            "\n"
            "\n  Options:"
            "\n    --buffer-strings                   size of buffer for strings used during import (default: 2GB)"
            "\n    --buffer-tensors                   size of buffer for tensors used during import (default: 2GB)"
            "\n    --list-separator                   character used to separate lists (default: ';')"
            "\n";
}

void print_dump_help()
{
    cout << "\nDump Usage:"
            "\n  mdb dump <db_folder> <output_file_prefix> <format>"
            "\n"
            "\n  Valid formats:"
            "\n   * RDF: [nt ttl]"
            "\n   * GQL: NOT SUPPORTED YET"
            "\n   * Quad Model: [qm json]"
            "\n";
}

void print_cli_help()
{
    cout << "\nCLI Usage:"
            "\n  mdb cli <db_folder> [OPTIONS]"
            "\n"
            "\n  Options:"
            "\n    --strings-dynamic <bytes>          size for the strings-dynamic-buffer"
            "\n    --strings-static <bytes>           size for static strings-static-buffer"
            "\n    --private-buffer <bytes>           size for the private-buffer"
            "\n    --versioned-buffer <bytes>         size for the versioned-buffer"
            "\n    --unversioned-buffer <bytes>       size for the unversioned-buffer"
            "\n";
}

void print_help()
{
    print_mdb_version();
    cout << "\nUsage: mdb <COMMAND> <ARGS> [OPTIONS]"
            "\nCommands:"
            "\n * server                              start the server"
            "\n * import                              create a new database from a supported file format"
            "\n                                        * RDF: [.ttl .nt .rdf]"
            "\n                                        * GQL: [.gql]"
            "\n                                        * Quad Model: [.qm]"
            "\n * csv-import                          create a new database from csv files"
            "\n * dump                                export a database into a file"
            "\n * cli                                 start the CLI for querying a database"
            "\n * help,--help                         print this help message"
            "\n";

    print_server_help();
    print_import_help();
    print_csv_import_help();
    print_dump_help();
    print_cli_help();
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);

    if (argc < 2) {
        print_help();
        return EXIT_FAILURE;
    }

    std::vector<std::string> args;

    for (int i = 2; i < argc; i++) {
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

    std::string subcommand(argv[1]);

    bool is_help = false;
    for (auto& arg : args) {
        if (arg == "--help")
            is_help = true;
    }

    if (is_help) {
        print_mdb_version();
        if (subcommand == "server") {
            print_server_help();
        } else if (subcommand == "import") {
            print_import_help();
        } else if (subcommand == "csv-import") {
            print_csv_import_help();
        } else if (subcommand == "dump") {
            print_dump_help();
        } else if (subcommand == "cli") {
            print_cli_help();
        } else {
            print_help();
        }
        return EXIT_SUCCESS;
    }

    if (subcommand == "help" || subcommand == "--help") {
        print_help();
        return EXIT_SUCCESS;
    }
    if (subcommand == "server") {
        auto config = parse_system_config(args, true);
        return mdb_server(config);
    }
    if (subcommand == "import") {
        ImportConfig config = parse_import_config(args);
        return mdb_import(config);
    }
    if (subcommand == "csv-import") {
        CSVImportConfig config = parse_csv_import_config(args);
        return mdb_csv_import(config);
    }
    if (subcommand == "dump") {
        DumpConfig config = parse_dump_config(args);
        return mdb_dump(config);
    }
#ifndef NO_MDB_CLI
    if (subcommand == "cli") {
        auto config = parse_system_config(args, false);
        auto model_id = Catalog::get_model_id(config.db_directory);

        System system(
            config.db_directory,
            config.strings_static_buffer,
            config.strings_dynamic_buffer,
            config.versioned_pages_buffer,
            config.private_pages_buffer,
            config.unversioned_pages_buffer,
            config.tensors_static_buffer,
            config.tensors_dynamic_buffer,
            1
        );

        try {
            switch (model_id) {
            case Catalog::ModelID::QUAD: {
                auto model_destroyer = QuadModel::init();
                return RunCLI(Model::Quad, config.query_timeout);
            }
            case Catalog::ModelID::RDF: {
                auto model_destroyer = RdfModel::init();
                return RunCLI(Model::RDF, config.query_timeout);
            }
            case Catalog::ModelID::GQL: {
                auto model_destroyer = GQLModel::init();
                return RunCLI(Model::GQL, config.query_timeout);
            }
            }
        } catch (const WrongModelException& e) {
            FATAL_ERROR(e.what());
        } catch (const WrongCatalogVersionException& e) {
            FATAL_ERROR(e.what());
        }
    }
#else
    if (subcommand == "cli") {
        FATAL_ERROR("Compiled without CLI support");
    }
#endif

    WARN("Unknown option \"", subcommand, '"');
    print_help();
    return EXIT_FAILURE;
}
