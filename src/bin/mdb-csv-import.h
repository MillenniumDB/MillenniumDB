#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "bin/common.h"
#include "import/gql/csv/import.h"
#include "import/gql/default_config.h"
#include "import/quad_model/csv/import.h"
#include "import/quad_model/default_config.h"
#include "misc/fatal_error.h"
#include "storage/filesystem.h"
#include "system/file_manager.h"

namespace MdbBin {
enum class CSVImportFormat {
    QUAD_MODEL,
    GQL,
};

struct CSVImportConfig {
    std::string db_folder;
    std::vector<std::string> node_paths;
    std::vector<std::string> edge_paths;
    CSVImportFormat format;
    uint64_t strings_buffer_size = 2ULL * 1024 * 1024 * 1024;
    uint64_t tensors_buffer_size = 2ULL * 1024 * 1024 * 1024;
    char list_separator = ';';
};

inline CSVImportConfig parse_csv_import_config(const std::vector<std::string>& args)
{
    CSVImportConfig config;

    std::map<std::string, std::function<std::string(CSVImportConfig&, const std::string&)>> opt;

    opt.insert({ "--buffer-strings", [](CSVImportConfig& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for buffer-strings";
                    }
                    config.strings_buffer_size = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "--buffer-tensors", [](CSVImportConfig& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for buffer-tensors";
                    }
                    config.tensors_buffer_size = static_cast<uint64_t>(bytes);
                    return "";
                } });

    opt.insert({ "--list-separator", [](CSVImportConfig& config, const std::string& value) {
                    if (value.size() != 1) {
                        return "list-separator expects a single character";
                    }
                    config.list_separator = value[0];
                    return "";
                } });

    if (args.size() <= 3) {
        FATAL_ERROR("Too few arguments for mdb csv-import");
    }

    auto model = to_lower(args[0]);
    if (model == "gql") {
        config.format = CSVImportFormat::GQL;
    } else if (model == "quad") {
        config.format = CSVImportFormat::QUAD_MODEL;
    } else {
        FATAL_ERROR("Unrecognized model, expected 'gql' or 'quad', received '", model, '\'');
    }
    config.db_folder = args[1];

    enum class FileTypeState {
        NODE,
        EDGE,
        UNSET,
    };
    FileTypeState state = FileTypeState::UNSET;

    size_t i = 2;
    while (i < args.size()) {
        const std::string& arg = args[i];

        // search in flags
        if (auto opt_found = opt.find(arg); opt_found != opt.end()) {
            i++;
            if (i >= args.size()) {
                FATAL_ERROR("expected value for ", args[i]);
            }
            auto error = opt_found->second(config, args[i]);
            if (!error.empty()) {
                FATAL_ERROR(error);
            }
            state = FileTypeState::UNSET;
        } else if (arg == "--nodes") {
            state = FileTypeState::NODE;
        } else if (arg == "--edges") {
            state = FileTypeState::EDGE;
        } else {
            switch (state) {
            case FileTypeState::NODE: {
                config.node_paths.push_back(arg);
                break;
            }
            case FileTypeState::EDGE: {
                config.edge_paths.push_back(arg);
                break;
            }
            case FileTypeState::UNSET: {
                FATAL_ERROR("Unexpected argument ", arg);
                break;
            }
            }
        }
        i++;
    }

    if (config.strings_buffer_size < Import::ExternalData::MIN_IMPORT_BUFFER_SIZE) {
        const auto res = "--buffer-strings must be at least "
                       + std::to_string(Import::ExternalData::MIN_IMPORT_BUFFER_SIZE) + " bytes";
        FATAL_ERROR(res);
    }

    if (config.tensors_buffer_size < Import::ExternalData::MIN_IMPORT_BUFFER_SIZE) {
        const auto res = "--buffer-tensors must be at least "
                       + std::to_string(Import::ExternalData::MIN_IMPORT_BUFFER_SIZE) + " bytes";
        FATAL_ERROR(res);
    }

    return config;
}

inline int mdb_csv_import(CSVImportConfig& config)
{
    const std::string& db_dir = config.db_folder;
    std::cout << "Database directory: " << db_dir << "\n";

    if (Filesystem::is_regular_file(db_dir)) {
        FATAL_ERROR("Cannot use \"", db_dir, "\" as database directory, file already exists.");
    } else if (Filesystem::is_directory(db_dir) && !Filesystem::is_empty(db_dir)) {
        FATAL_ERROR("Database folder \"", db_dir, "\" already exists and it's not empty");
    }

    std::cout << "Creating new database\n";
    std::cout << "  db directory:  " << db_dir << std::endl;

    FileManager::init(db_dir);

    switch (config.format) {
    case CSVImportFormat::QUAD_MODEL: {
        {
            Import::QuadModel::CSV::OnDiskImport importer(
                db_dir,
                config.strings_buffer_size,
                config.tensors_buffer_size,
                config.list_separator
            );

            std::vector<std::unique_ptr<MDBIstreamFile>> streams_nodes;
            std::vector<std::unique_ptr<MDBIstreamFile>> streams_edges;

            for (auto& path : config.node_paths) {
                streams_nodes.push_back(std::make_unique<MDBIstreamFile>(path));
            }

            for (auto& path : config.edge_paths) {
                streams_edges.push_back(std::make_unique<MDBIstreamFile>(path));
            }

            importer.start_import(streams_nodes, streams_edges);
        }
        Import::QuadModel::create_default_params(db_dir);
        break;
    }
    case CSVImportFormat::GQL: {
        {
            Import::GQL::CSV::OnDiskImport importer(
                db_dir,
                config.strings_buffer_size,
                config.tensors_buffer_size,
                config.list_separator
            );

            std::vector<std::unique_ptr<MDBIstreamFile>> streams_nodes;
            std::vector<std::unique_ptr<MDBIstreamFile>> streams_edges;

            for (auto& path : config.node_paths) {
                streams_nodes.push_back(std::make_unique<MDBIstreamFile>(path));
            }

            for (auto& path : config.edge_paths) {
                streams_edges.push_back(std::make_unique<MDBIstreamFile>(path));
            }

            importer.start_import(streams_nodes, streams_edges);
        }
        Import::GQL::create_default_params(db_dir);
        break;
    }
    default: {
        FATAL_ERROR("UNEXPECTED CSVImportFormat.");
    }
    }
    return EXIT_SUCCESS;
}
} // namespace MdbBin
