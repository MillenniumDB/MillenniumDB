#pragma once

#include <cstdint>
#include <string>
#include <thread>
#include <vector>

#include "bin/common.h"
#include "import/gql/default_config.h"
#include "import/gql/import.h"
#include "import/quad_model/default_config.h"
#include "import/quad_model/import.h"
#include "import/rdf_model/default_config.h"
#include "import/rdf_model/import.h"
#include "import/rdf_model/xml/import.h"
#include "misc/fatal_error.h"
#include "storage/filesystem.h"
#include "system/file_manager.h"

namespace MdbBin {
enum class ImportFileFormat {
    UNASSIGNED,
    QUAD_MODEL,
    TTL,
    RDF_XML,
    GQL,
};

struct ImportConfig {
    std::vector<std::string> paths;
    std::string prefixes_file;
    ImportFileFormat format = ImportFileFormat::UNASSIGNED;
    uint64_t strings_buffer_size = 2ULL * 1024 * 1024 * 1024;
    uint64_t tensors_buffer_size = 2ULL * 1024 * 1024 * 1024;
    size_t btree_permutations = 4;
};

inline ImportConfig parse_import_config(const std::vector<std::string>& args)
{
    ImportConfig config;

    std::map<std::string, std::function<std::string(ImportConfig&, const std::string&)>> opt;

    opt.insert({ "--buffer-strings", [](ImportConfig& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for buffer-strings";
                    }
                    config.strings_buffer_size = ((static_cast<uint64_t>(bytes) / MDB_ALIGNMENT) + 1)
                                               * MDB_ALIGNMENT;
                    return "";
                } });

    opt.insert({ "--buffer-tensors", [](ImportConfig& config, const std::string& value) {
                    auto bytes = parse_bytes(value);
                    if (bytes < 0) {
                        return "invalid value for buffer-tensors";
                    }
                    config.tensors_buffer_size = ((static_cast<uint64_t>(bytes) / MDB_ALIGNMENT) + 1)
                                               * MDB_ALIGNMENT;
                    return "";
                } });

    opt.insert({ "--format", [](ImportConfig& config, const std::string& value) {
                    auto lc_value = to_lower(value);
                    if (lc_value == "gql") {
                        config.format = ImportFileFormat::GQL;
                    } else if (lc_value == "ttl" || lc_value == "n3" || lc_value == "nt") {
                        config.format = ImportFileFormat::TTL;
                    } else if (lc_value == "rdf") {
                        config.format = ImportFileFormat::RDF_XML;
                    } else if (lc_value == "qm") {
                        config.format = ImportFileFormat::QUAD_MODEL;
                    } else {
                        return "Valid formats are: [ttl, nt, n3, rdf] for RDF, [gql] for GQL and [qm] "
                               "for Quad Model";
                    }
                    return "";
                } });

    opt.insert({ "--btree-permutations", [](ImportConfig& config, const std::string& value) {
                    try {
                        auto perms = std::stoi(value);
                        if (perms == 3 || perms == 4 || perms == 6) {
                            config.btree_permutations = perms;
                            return "";
                        }
                    } catch (...) {
                    }
                    return "Invalid value for option \"btree-permutations\". Expected 3, 4 or 6";
                } });

    opt.insert({ "--prefixes", [](ImportConfig& config, const std::string& value) {
                    config.prefixes_file = value;
                    return "";
                } });

    size_t i = 0;

    while (i < args.size()) {
        const std::string* arg = &args[i];

        // search in flags
        if (auto opt_found = opt.find(*arg); opt_found != opt.end()) {
            i++;
            if (i >= args.size()) {
                FATAL_ERROR("expected value for ", args[i]);
            }
            auto error = opt_found->second(config, args[i]);
            if (!error.empty()) {
                FATAL_ERROR(error);
            }
        } else {
            if (arg->size() > 0 && arg->data()[0] == '-') {
                FATAL_ERROR("unrecognized option: ", args[i]);
            }

            config.paths.push_back(*arg);
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

inline int mdb_import(ImportConfig& config)
{
    if (config.paths.empty()) {
        FATAL_ERROR("No database folder specified");
    }

    const std::string& db_dir = config.paths.back();
    std::cout << "Database directory: " << db_dir << "\n";

    if (Filesystem::is_regular_file(db_dir)) {
        FATAL_ERROR("Cannot use \"", db_dir, "\" as database directory, file already exists.");
    } else if (Filesystem::is_directory(db_dir) && !Filesystem::is_empty(db_dir)) {
        FATAL_ERROR("Database folder \"", db_dir, "\" already exists and it's not empty");
    }

    std::vector<std::string> input_files;
    for (size_t i = 0; i < config.paths.size() - 1; i++) {
        if (!Filesystem::is_regular_file(config.paths[i])) {
            FATAL_ERROR("Input file \"", config.paths[i], "\" does not exist");
        }
        input_files.push_back(config.paths[i]);
    }

    if (config.format == ImportFileFormat::UNASSIGNED) {
        if (input_files.empty()) {
            FATAL_ERROR(
                "Must specify the model when not working with input files or when importing CSV files"
            );
        }

        for (auto& input_file : input_files) {
            std::string extension = Filesystem::get_extension(input_file);

            ImportFileFormat file_format = ImportFileFormat::UNASSIGNED;
            if (extension == ".qm") {
                file_format = ImportFileFormat::QUAD_MODEL;
            } else if (extension == ".ttl" || extension == ".n3" || extension == ".nt") {
                file_format = ImportFileFormat::TTL;
            } else if (extension == ".rdf") {
                file_format = ImportFileFormat::RDF_XML;
            } else if (extension == ".gql") {
                file_format = ImportFileFormat::GQL;
            } else if (extension.empty()) {
                FATAL_ERROR(
                    "No input file extension for file \"",
                    input_file,
                    "\", could not guess what graph model are you using.\nValid extensions are: ",
                    "[.ttl, .nt, .n3 .rdf] for RDF, [.gql] for GQL and [.qm] for Quad Model",
                    "\n. Rename your file or specify the graph model with the option --format"
                );
            } else {
                FATAL_ERROR(
                    "Unrecognized input file extension: \"",
                    extension,
                    "\".\nValid extensions are: ",
                    "[.ttl, .nt, .n3 .rdf] for RDF, [.gql] for GQL and [.qm] for Quad Model"
                );
            }

            if (config.format == ImportFileFormat::UNASSIGNED) {
                config.format = file_format;
            } else if (config.format != file_format) {
                FATAL_ERROR("Cannot import multiple files with different file formats.");
            }
        }
    }

    std::cout << "Creating new database\n";
    std::cout << "  db directory:  " << db_dir << "\n";
    if (input_files.empty()) {
        auto in_avail = std::cin.rdbuf()->in_avail();
        if (in_avail == 0) {
            // wait just in case
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            in_avail = std::cin.rdbuf()->in_avail();
            if (in_avail == 0) {
                FATAL_ERROR("Nothing received as standard input.");
            }
        }
    }
    if (!config.prefixes_file.empty()) {
        std::cout << "  prefixes file: " << config.prefixes_file << "\n";
    }
    std::cout.flush();

    FileManager::init(db_dir);

    switch (config.format) {
    case ImportFileFormat::QUAD_MODEL: {
        Import::QuadModel::OnDiskImport importer(
            db_dir,
            config.strings_buffer_size,
            config.tensors_buffer_size
        );
        if (input_files.empty()) {
            MDBIstreamWrapper in(std::cin);
            importer.start_import(in);
        } else {
            MDBIstreamFiles files(input_files);
            importer.start_import(files);
        }
        Import::QuadModel::create_default_params(db_dir);
        break;
    }
    case ImportFileFormat::TTL: {
        if (config.prefixes_file.empty()) {
            WARN("no prefixes file specified");
        }

        Import::Rdf::OnDiskImport importer(
            db_dir,
            config.strings_buffer_size,
            config.tensors_buffer_size,
            config.btree_permutations
        );
        if (input_files.empty()) {
            MDBIstreamWrapper in(std::cin);
            importer.start_import(in, config.prefixes_file, input_files);
        } else {
            MDBIstreamFiles files(input_files);
            importer.start_import(files, config.prefixes_file, input_files);
        }
        Import::Rdf::create_default_params(db_dir);
        break;
    }
    case ImportFileFormat::RDF_XML: {
        Import::Rdf::XML::OnDiskImport importer(
            db_dir,
            config.strings_buffer_size,
            config.tensors_buffer_size,
            config.btree_permutations
        );
        if (input_files.empty()) {
            MDBIstreamWrapper in(std::cin);
            importer.start_import(in, config.prefixes_file);
        } else {
            MDBIstreamFiles files(input_files);
            importer.start_import(files, config.prefixes_file);
        }
        Import::Rdf::create_default_params(db_dir);
        break;
    }
    case ImportFileFormat::GQL: {
        Import::GQL::OnDiskImport importer(db_dir, config.strings_buffer_size, config.tensors_buffer_size);
        if (input_files.empty()) {
            MDBIstreamWrapper in(std::cin);
            importer.start_import(in);
        } else {
            MDBIstreamFiles files(input_files);
            importer.start_import(files);
        }
        Import::GQL::create_default_params(db_dir);
        break;
    }
    case ImportFileFormat::UNASSIGNED: {
        FATAL_ERROR("Graph model not assigned.");
    }
    }
    return EXIT_SUCCESS;
}
} // namespace MdbBin
