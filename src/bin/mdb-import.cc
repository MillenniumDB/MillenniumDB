#include <iostream>

#include "import/quad_model/import.h"
#include "import/rdf_model/import.h"
#include "storage/filesystem.h"
#include "system/file_manager.h"
#include "third_party/cli11/CLI11.hpp"


enum class Model { Quad, RDF };

static const std::string QUAD_MODEL_EXTENSIONS[] = {".qm"};
static const std::string RDF_MODEL_EXTENSIONS[] = {".ttl", ".nt", ".n3"};


template<size_t size>
static bool in_array(const std::string (&array)[size], const std::string& value) {
    for (const auto& arr_val : array) {
        if (arr_val == value) {
            return true;
        }
    }
    return false;
}


template<size_t size>
static std::ostream& operator<<(std::ostream& os, const std::string (&array)[size]) {
    auto first = true;
    for (const auto& value : array) {
        if (first) first = false; else os << ", ";
        os << value;
    }
    return os;
}


static std::ostream& operator<<(std::ostream& os, Model model) {
    switch (model) {
    case Model::RDF:  os << "RDF";  break;
    case Model::Quad: os << "Quad"; break;
    }
    return os;
}


struct ModelNameValidator : public CLI::Validator {
    ModelNameValidator() {
        name_ = "model_name_validator";
        func_ = [](std::string& str) -> std::string {
            str = CLI::detail::to_lower(str);
            if (str == "rdf" || str== "quad")
                return "";
            else
                return std::string(" unrecognized, expecting rdf|quad");
        };
    }
};


int main(int argc, char* argv[]) {
    std::string data_file;
    std::string db_directory;
    std::string prefixes_file;
    std::string model_name;
    uint64_t buffer_size = 2ULL * 1024 * 1024 * 1024;
    size_t btree_permutations = 4;

    CLI::App app{"MillenniumDB Import"};
    app.get_formatter()->column_width(35);
    app.option_defaults()->always_capture_default();

    app.add_option("data-file", data_file)
        ->description("data file to be imported")
        ->type_name("<path>")
        ->check(CLI::ExistingFile.description(""))
        ->required();

    app.add_option("db-directory", db_directory)
        ->description("path to the database directory to be created")
        ->type_name("<path>")
        ->check(CLI::NonexistentPath.description(""))
        ->required();

    app.add_option("--prefixes", prefixes_file)
        ->description("file containing prefixes to be imported (only for rdf model)")
        ->type_name("<path>")
        ->check(CLI::ExistingFile.description(""));

    app.add_option("--btree-permutations", btree_permutations)
        ->type_name("")
        ->description("How many B+trees permutations will be created (only for rdf model, valid values: 3,4,6)");

    app.add_option("--buffer", buffer_size)
        ->description("size of buffer used during import")
        ->option_text("<bytes> [2GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1ULL * 1024 * 1024 * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("-m,--model", model_name)
        ->description("graph model")
        ->option_text("quad|rdf")
        ->transform(ModelNameValidator());

    CLI11_PARSE(app, argc, argv);

    Model model;
    if (model_name.empty()) {
        auto extension = Filesystem::get_extension(data_file);
        if (in_array(QUAD_MODEL_EXTENSIONS, extension)) {
            model = Model::Quad;
        } else if (in_array(RDF_MODEL_EXTENSIONS, extension)) {
            model = Model::RDF;
        } else {
            std::cerr << "Invalid input file extension: \"" << extension << "\"\n"
                      << "Valid extensions are: "
                      << QUAD_MODEL_EXTENSIONS << ", " << RDF_MODEL_EXTENSIONS << "\n";
            return EXIT_FAILURE;
        }
    } else {
        if (model_name == "rdf")
            model = Model::RDF;
        else // if (model_name == "quad")
            model = Model::Quad;
    }


    if (prefixes_file.empty() && model == Model::RDF) {
        std::cout << "WARNING: no prefixes file specified" << std::endl;
    }

    if (!prefixes_file.empty() && model == Model::Quad) {
        std::cout << "WARNING: QuadModel does not use prefixes, ignoring prefix file" << std::endl;
        prefixes_file = "";
    }


    std::cout << "Creating new " << model << " model database\n";
    std::cout << "  data file:     " << data_file << "\n";
    std::cout << "  db directory:  " << db_directory << "\n";
    if (!prefixes_file.empty()) {
        std::cout << "  prefixes file: " << prefixes_file << "\n";
    }


    FileManager::init(db_directory);

    switch (model) {
    case Model::Quad: {
        Import::QuadModel::OnDiskImport importer(db_directory, buffer_size);
        importer.start_import(data_file);
        break;
    }
    case Model::RDF: {
        if (btree_permutations != 3 && btree_permutations != 4 && btree_permutations != 6) {
            std::cerr << "Invalid value for option \"btree-permutations\". Expected 3, 4 or 6\n";
        }
        Import::Rdf::OnDiskImport importer(db_directory, buffer_size, btree_permutations);
        importer.start_import(data_file, prefixes_file);
        break;
    }
    }
    return EXIT_SUCCESS;
}
