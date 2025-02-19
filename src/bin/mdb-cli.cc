#include "cli/cli.h"
#include "graph_models/exceptions.h"
#include "graph_models/quad_model/quad_model.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "misc/fatal_error.h"
#include "storage/index/tensor_store/tensor_store_manager.h"
#include "system/buffer_manager.h"
#include "system/string_manager.h"
#include "system/system.h"

#include "third_party/cli11/CLI11.hpp"

static const std::string QUAD_MODEL_EXTENSIONS[] = { ".qm" };
static const std::string RDF_MODEL_EXTENSIONS[] = { ".ttl", ".nt", ".n3" };

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

int main(int argc, char* argv[])
{
    std::string db_directory;
    std::chrono::seconds timeout { 60 };

    uint64_t strings_static_buffer = StringManager::DEFAULT_STATIC_BUFFER;
    uint64_t strings_dynamic_buffer = StringManager::DEFAULT_DYNAMIC_BUFFER;
    uint64_t versioned_pages_buffer = BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE;
    uint64_t private_pages_buffer = BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE;
    uint64_t unversioned_pages_buffer = BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE;
    uint64_t tensor_store_buffer = TensorStoreManager::DEFAULT_TENSOR_BUFFER_SIZE;

    CLI::App app { "MillenniumDB TUI" };
    app.get_formatter()->column_width(34);
    app.option_defaults()->always_capture_default();

    app.add_option("database", db_directory)
        ->description("Database directory")
        ->type_name("<path>")
        ->check(CLI::ExistingDirectory.description(""))
        ->required();

    app.add_option("--timeout", timeout)
        ->description("Timeout of query executions")
        ->type_name("<seconds>")
        ->check(CLI::Range(1, 36'000).description(""));

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
        ->description(
            "Size of buffer for versioned pages shared between threads\nAllows units such as MB and GB"
        )
        ->option_text("<bytes> [1GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--private-buffer", private_pages_buffer)
        ->description("Size of private per-thread buffers,\nAllows units such as MB and GB")
        ->option_text("<bytes> [64MB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--unversioned-buffer", unversioned_pages_buffer)
        ->description(
            "Size of buffer for unversioned pages shared between threads,\nAllows units such as MB and GB"
        )
        ->option_text("<bytes> [128MB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--tensor-store-buffer", tensor_store_buffer)
        ->description("Size of tensor store buffer,\nAllows units such as MB and GB")
        ->option_text("<bytes> [1GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    CLI11_PARSE(app, argc, argv);

    auto catalog_path = db_directory + "/catalog.dat";
    auto catalog_fs = std::fstream(catalog_path, std::ios::in | std::ios::binary);

    if (!catalog_fs.is_open()) {
        std::cerr << "Could not open file \"" << catalog_path << "\".\n";
        return EXIT_FAILURE;
    }

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
    auto model_identifier = read_uint64(catalog_fs);

    try {
        switch (model_identifier) {
        case QuadCatalog::MODEL_ID: {
            auto model_destroyer = QuadModel::init();
            return RunCLI(Model::Quad, timeout);
        }
        case RdfCatalog::MODEL_ID: {
            auto model_destroyer = RdfModel::init();
            return RunCLI(Model::RDF, timeout);
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
