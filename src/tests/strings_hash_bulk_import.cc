#include "macros/aligned_alloc.h"
#include "storage/index/hash/strings_hash/strings_hash_bulk_ondisk_import.h"
#include "system/string_manager.h"

// returns the encoded string length, and writes the string in the buffer.
// assumes the string fits in the buffer and file read position is correct
size_t read_pending_str(std::fstream& file, char* buffer) {
    size_t decoded_len = 0;
    // size_t bytes_for_len = 0;
    size_t shift_size = 0;

    while (true) {
        char c;
        file.read(&c, 1);
        auto decode_ptr = reinterpret_cast<unsigned char*>(&c);
        uint64_t b = *decode_ptr;
        // bytes_for_len++;

        if (b <= 127) {
            decoded_len |= b << shift_size;
            break;
        } else {
            decoded_len |= (b & 0x7FUL) << shift_size;
        }
        shift_size += 7;
    }
    file.read(buffer, decoded_len);
    return decoded_len;
}


int main() {
    std::fstream strings_file;
    strings_file.open("test_str_hash/strings.dat", std::ios::in|std::ios::binary);
    strings_file.seekg(0, strings_file.end);
    uint64_t string_file_size = strings_file.tellg();

    strings_file.seekg(0, strings_file.beg);
    uint64_t last_block_offset;
    strings_file.read(reinterpret_cast<char*>(&last_block_offset), 8);

    auto buffer_size = 2ULL * 1024 * 1024 * 1024;
    char* buffer = reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(buffer_size));

    char* pending_string_buffer = reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(StringManager::MAX_STRING_SIZE));

    {
        StringsHashBulkOnDiskImport strings_hash("test_str_hash/str_hash",
                                                buffer,
                                                buffer_size);
        uint64_t current_pos = 0;

        assert(string_file_size % StringManager::BLOCK_SIZE == 0);

        auto string_blocks = string_file_size / StringManager::BLOCK_SIZE;
        auto last_str_pos = (string_blocks-1) * StringManager::BLOCK_SIZE + last_block_offset;

        // read all strings one by one and add them to the StringsHash
        while (current_pos < last_str_pos) {
            size_t remaining_in_block = StringManager::BLOCK_SIZE
                                            - (current_pos % StringManager::BLOCK_SIZE);

            if (remaining_in_block < StringManager::MIN_PAGE_REMAINING_BYTES) {
                current_pos += remaining_in_block;
                strings_file.read(pending_string_buffer, remaining_in_block);
            }

            auto str_len = read_pending_str(strings_file, pending_string_buffer);
            strings_hash.create_id(pending_string_buffer, current_pos, str_len);
            current_pos = strings_file.tellg();
        }
    }
    strings_file.close();
    MDB_ALIGNED_FREE(buffer);
    MDB_ALIGNED_FREE(pending_string_buffer);
}
