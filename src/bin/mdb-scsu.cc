#include "graph_models/rdf_model/rdf_model.h"
#include "storage/scsu.h"
#include "system/buffer_manager.h"
#include "system/string_manager.h"
#include "system/system.h"
#include "third_party/cli11/CLI11.hpp"


bool equal(const unsigned char* a, size_t a_count, const unsigned char* b, size_t b_count) {
    if (a_count != b_count) {
        return false;
    }
    auto end = a + a_count;
    while (a < end) {
        if (*a++ != *b++) {
            return false;
        }
    }
    return true;
}


int main(int argc, char* argv[]) {
    std::string db_directory;

    CLI::App app{"MillenniumDB SCSU"};
    app.get_formatter()->column_width(34);
    app.option_defaults()->always_capture_default();

    app.add_option("db-directory", db_directory)
        ->description("Database directory")
        ->type_name("<path>")
        ->check(CLI::ExistingDirectory.description(""))
        ->required();

    CLI11_PARSE(app, argc, argv);


    System system(
        db_directory,
        StringManager::DEFAULT_LOAD_STR,
        BufferManager::DEFAULT_VERSIONED_PAGES_BUFFER_SIZE,
        BufferManager::DEFAULT_PRIVATE_PAGES_BUFFER_SIZE,
        BufferManager::DEFAULT_UNVERSIONED_PAGES_BUFFER_SIZE,
        1
    );
    auto model_destroyer = RdfModel::init();

    uint64_t current_block = 0;
    uint64_t current_block_offset = StringManager::METADATA_SIZE;
    uint64_t total_blocks = string_manager.get_total_blocks();
    uint64_t last_block_max_offset = string_manager.get_last_block_offset();

    uint64_t total_strings = 0;
    uint64_t total_original_size = 0;
    uint64_t total_compressed_size = 0;

    std::stringstream buffer;

    while (current_block < total_blocks) {
        std::stringstream ss;
        uint64_t str_id = current_block_offset + current_block * StringManager::STRING_BLOCK_SIZE;
        string_manager.print(ss, str_id);
        auto original = ss.str();

        if (original.size() != 8) {
            total_strings++;
            total_original_size += original.size();

            static char compressed[64 * 1024 * 1024];
            // static unsigned char decompressed[64 * 1024 * 1024];

            auto compressed_size = SCSU::compress(original.data(), original.size(), compressed);
            // auto decompressed_size = SCSU::decompress(compressed, compressed_size, decompressed);

            // if (!equal(original, original_size, decompressed, decompressed_size)) {
            //     auto original_string = std::string(original, original + original_size);
            //     auto decompressed_string = std::string(decompressed, decompressed + decompressed_size);
            //     std::cerr << "original_string: \"" << original_string << "\", decompressed_string: \"" << decompressed_string << "\"\n";
            // }

            total_compressed_size += compressed_size;

            if (total_strings % 1'000'000 == 0) {
                std::cout << "total_strings: " << total_strings
                          << ", total_original_size: " << total_original_size
                          << ", total_compressed_size: " << total_compressed_size
                          << ", ratio: " << static_cast<double>(total_compressed_size) / static_cast<double>(total_original_size)
                          << "\n";
            }
        }

        uint64_t bytes_for_len = StringManager::get_bytes_for_len(original.size());
        current_block_offset += bytes_for_len + original.size();

        if (current_block_offset >= StringManager::STRING_BLOCK_SIZE) {
            current_block_offset = current_block_offset % StringManager::STRING_BLOCK_SIZE;
            current_block++;
        }
        // Go to next block if there are no enough space
        if (StringManager::STRING_BLOCK_SIZE - current_block_offset < StringManager::MIN_PAGE_REMAINING_BYTES) {
            current_block++;
            current_block_offset = 0;
        }

        // Border case for last block
        if (current_block + 1 == total_blocks) {
            if (current_block_offset >= last_block_max_offset) {
                break;
            }
        }
    }

    std::cout << "total_strings: " << total_strings
        << ", total_original_size: " << total_original_size
        << ", total_compressed_size: " << total_compressed_size
        << ", ratio: " << static_cast<double>(total_compressed_size) / static_cast<double>(total_original_size)
        << "\n";
}
