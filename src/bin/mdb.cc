#include "bin/mdb-csv-import.h"
#include "bin/mdb-dump.h"
#include "bin/mdb-import.h"
#include "bin/mdb-server.h"
#include "cli/cli.h"

using namespace MdbBin;

void print_mdb_version()
{
    std::cout << "MillenniumDB v1.0.0\n";
}

void print_server_help()
{
    std::cout << R"(
Server usage: mdb server <db_folder> [options]

  Server Options:
    -j,--threads,--workers <N>         Number of worker threads (default: auto)
    -p,--port <port>                   Server port (default: 1234)
    -t,--timeout <seconds>             Query timeout (default: 60)

  Browser Interface:
    --browser <bool>                   Enable or disable web browser
    --browser-port <port>              Browser port (default: 4321)

  Security:
    --admin-user <username>            Admin username
    --admin-password <password>        Admin password
    --ssl-cert <path>                  Path to SSL certificate file
    --ssl-key <path>                   Path to SSL key file

  Memory Configuration:
    (Values support suffixes: KB, MB, GB, TB)
    --strings-dynamic <N>              Size of dynamic string buffer
    --strings-static <N>               Size of static static string buffer
    --tensors-dynamic <N>              Size of dynamic tensor buffer
    --tensors-static <N>               Size of static static tensor buffer
    --private-buffer <N>               Size of private buffer
    --versioned-buffer <N>             Size of versioned buffer

  Logging:
    --log-path <path>                  Redirect logs to file instead of stdout
    --log-timestamp <bool>             Include timestamps in log output
    --log-category <bool>              Include category names in log output
    --log-debug <bool>                 Include debug category in log
    --log-error <bool>                 Include error category in log
    --log-info <bool>                  Include info category in log
)";
}

void print_import_help()
{
    std::cout << R"(
Import Usage: mdb import <files...> <db_folder> [options]

  Alternative usage with stdin (useful for importing compressed files):
    Example:
    bzcat file.ttl.bz2 | mdb import <db_folder> --format ttl

  Options:
    --buffer-strings <N>               Size of strings buffer (default: 2GB)
    --buffer-tensors <N>               Size of tensors buffer (default: 2GB)
    --format <ext>                     Specify the file format:
                                       * RDF: [ttl, nt, n3, rdf]
                                       * GQL: [gql]
                                       * Quad Model: [qm]

  Options for RDF:
    --prefixes <path>                  Prefixes file path (for IRI compression)
    --btree-permutations <N>           3, 4, or 6 (default: 4)
)";
}

void print_csv_import_help()
{
    std::cout << R"(
CSV Import Usage:
  mdb csv-import <gql|quad> <db_folder> --nodes <node_files...> --edges <edge_files> [options]

  Options:
    --buffer-strings                   Size of strings buffer (default: 2GB)
    --buffer-tensors                   Size of tensors buffer (default: 2GB)
    --list-separator                   Character to separate lists (default: ';')
)";
}

void print_dump_help()
{
    std::cout << R"(
Dump Usage:
  mdb dump <db_folder> <output_file_prefix> <format>

  Valid formats:
    * RDF: [nt ttl]
    * GQL: NOT SUPPORTED YET
    * Quad Model: [qm json]
)";
}

void print_cli_help()
{
    std::cout << R"(
CLI Usage:
  mdb cli <db_folder> [options]

  Options:
    --strings-dynamic <bytes>          Size of dynamic string buffer
    --strings-static <bytes>           Size of static string buffer
    --private-buffer <bytes>           Size of private buffer
    --versioned-buffer <bytes>         Size of versioned buffer
)";
}

void print_help()
{
    print_mdb_version();
    std::cout << R"(
Usage: mdb <command> [<args>] [options]
Commands:
  server                               Start the database server
  import                               Create a new database from a supported file format
                                         * RDF: [.ttl .nt .rdf]
                                         * GQL: [.gql]
                                         * Quad Model: [.qm]
  csv-import                           create a new database from csv files
  dump                                 Export a database to a file
  cli                                  Launch the CLI for querying a database
  help,--help                          Print this message or help for a specific command

)";

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
