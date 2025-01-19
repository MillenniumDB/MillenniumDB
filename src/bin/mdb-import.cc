#include <cstdio>
#include <iostream>
#include <thread>

#include "import/gql/import.h"
#include "import/quad_model/import.h"
#include "import/rdf_model/import.h"
#include "misc/fatal_error.h"
#include "misc/istream.h"
#include "storage/filesystem.h"
#include "system/file_manager.h"
#include "third_party/cli11/CLI11.hpp"

enum class Model {
    Quad,
    RDF,
    GQL,
    NOT_ASSIGNED
};

static const std::string QUAD_MODEL_EXTENSIONS[] = { ".qm" };
static const std::string RDF_MODEL_EXTENSIONS[] = { ".ttl", ".nt", ".n3" };
static const std::string GQL_MODEL_EXTENSIONS[] = { ".gql" };

static const std::string ALL_EXTENSIONS = "[.ttl, .nt, .n3] for RDF, [.gql] for GQL and [.qm] for Quad Model";

template<size_t size>
static bool in_array(const std::string (&array)[size], const std::string& value)
{
    for (const auto& arr_val : array) {
        if (arr_val == value) {
            return true;
        }
    }
    return false;
}

static std::ostream& operator<<(std::ostream& os, Model model)
{
    switch (model) {
    case Model::RDF:
        os << "RDF";
        break;
    case Model::Quad:
        os << "Quad";
        break;
    case Model::GQL:
        os << "GQL";
        break;
    case Model::NOT_ASSIGNED:
        os << "";
        break;
    }
    return os;
}

struct ModelNameValidator : public CLI::Validator {
    ModelNameValidator()
    {
        name_ = "model_name_validator";
        func_ = [](std::string& str) -> std::string {
            str = CLI::detail::to_lower(str);
            if (str == "rdf" || str == "quad" || str == "gql")
                return "";
            else
                return std::string(" unrecognized, expecting rdf|quad|gql");
        };
    }
};

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);

    std::vector<std::string> paths;

    std::string prefixes_file;
    std::string model_name;
    uint64_t buffer_size = 2ULL * 1024 * 1024 * 1024;
    size_t btree_permutations = 4;

    CLI::App app { "MillenniumDB Import" };
    app.get_formatter()->column_width(35);
    app.option_defaults()->always_capture_default();

    app.add_option("paths", paths)
        ->type_name("")
        ->description("list (size >= 1) of paths. The last path is the folder that will be created.")
        ->required();

    app.add_option("--prefixes", prefixes_file)
        ->description("file containing prefixes to be imported (only for rdf model)")
        ->type_name("<path>")
        ->check(CLI::ExistingFile.description(""));

    app.add_option("--btree-permutations", btree_permutations)
        ->type_name("")
        ->description(
            "How many B+trees permutations will be created (only for rdf model, valid values: 3,4,6)"
        );

    app.add_option("--buffer", buffer_size)
        ->description("size of buffer used during import")
        ->option_text("<bytes> [2GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1ULL * 1024 * 1024 * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("-m,--model", model_name)
        ->description("graph model")
        ->option_text("quad|rdf|gql")
        ->transform(ModelNameValidator());

    // app.set_help_flag("");
    // app.add_flag_callback("--help",[](){ std::cout << help_message;  throw CLI::Success(); })->trigger_on_parse();

    CLI11_PARSE(app, argc, argv);

    std::string db_directory = paths.back();
    std::cout << "Database directory: " << db_directory << "\n";

    if (Filesystem::is_regular_file(db_directory)) {
        FATAL_ERROR("Cannot use \"", db_directory, "\" as database directory, file already exists.");
    } else if (Filesystem::is_directory(db_directory) && !Filesystem::is_empty(db_directory)) {
        FATAL_ERROR("Database folder \"", db_directory, "\" already exists and it's not empty");
    }

    std::vector<std::string> input_files;
    for (size_t i = 0; i < paths.size() - 1; i++) {
        if (!Filesystem::is_regular_file(paths[i])) {
            FATAL_ERROR("Input file \"", paths[i], "\" does not exist");
        }
        input_files.push_back(paths[i]);
    }

    Model model = Model::NOT_ASSIGNED;
    if (model_name.empty()) {
        if (input_files.empty()) {
            FATAL_ERROR("Must specify the model when not working with input files");
        }

        for (auto& input_file : input_files) {
            std::string extension = Filesystem::get_extension(input_file);

            Model new_model = Model::NOT_ASSIGNED;
            if (in_array(QUAD_MODEL_EXTENSIONS, extension)) {
                new_model = Model::Quad;
            } else if (in_array(RDF_MODEL_EXTENSIONS, extension)) {
                new_model = Model::RDF;
            } else if (in_array(GQL_MODEL_EXTENSIONS, extension)) {
                new_model = Model::GQL;
            } else if (extension.empty()) {
                FATAL_ERROR(
                    "No input file extension for file \"",
                    input_file,
                    "\", could not guess what graph model are you using.\nValid extensions are: ",
                    ALL_EXTENSIONS,
                    "\n. Rename your file or specify the graph model with the option --model"
                );
            } else {
                FATAL_ERROR(
                    "Unrecognized input file extension: \"",
                    extension,
                    "\".\nValid extensions are: ",
                    ALL_EXTENSIONS
                );
            }

            if (model == Model::NOT_ASSIGNED) {
                model = new_model;
            } else if (model != new_model) {
                FATAL_ERROR("Cannot import multiple files with different graph models.");
            }
        }
    } else {
        if (model_name == "rdf")
            model = Model::RDF;
        else if (model_name == "quad")
            model = Model::Quad;
        else if (model_name == "gql")
            model = Model::GQL;
        else {
            FATAL_ERROR("Invalid model name: \"", model_name, "\"");
        }
    }

    if (prefixes_file.empty() && model == Model::RDF) {
        WARN("no prefixes file specified");
    }

    if (!prefixes_file.empty() && model == Model::Quad) {
        WARN("QuadModel does not use prefixes, ignoring prefix file");
        prefixes_file = "";
    }

    std::cout << "Creating new " << model << " model database\n";
    std::cout << "  db directory:  " << db_directory << "\n";
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
    if (!prefixes_file.empty()) {
        std::cout << "  prefixes file: " << prefixes_file << "\n";
    }
    std::cout.flush();

    FileManager::init(db_directory);

    switch (model) {
    case Model::Quad: {
        Import::QuadModel::OnDiskImport importer(db_directory, buffer_size);
        if (input_files.empty()) {
            MDBIstreamWrapper in(std::cin);
            importer.start_import(in);
        } else {
            MDBIstreamFiles files(input_files);
            importer.start_import(files);
        }
        break;
    }
    case Model::RDF: {
        if (btree_permutations != 3 && btree_permutations != 4 && btree_permutations != 6) {
            std::cerr << "Invalid value for option \"btree-permutations\". Expected 3, 4 or 6\n";
        }
        Import::Rdf::OnDiskImport importer(db_directory, buffer_size, btree_permutations);
        if (input_files.empty()) {
            MDBIstreamWrapper in(std::cin);
            importer.start_import(in, prefixes_file);
        } else {
            MDBIstreamFiles files(input_files);
            importer.start_import(files, prefixes_file);
        }
        break;
    }
    case Model::GQL: {
        Import::GQL::OnDiskImport importer(db_directory, buffer_size);
        if (input_files.empty()) {
            MDBIstreamWrapper in(std::cin);
            importer.start_import(in);
        } else {
            MDBIstreamFiles files(input_files);
            importer.start_import(files);
        }
        break;
    }
    case Model::NOT_ASSIGNED: {
        FATAL_ERROR("Graph model not assigned.");
    }
    }
    return EXIT_SUCCESS;
}
