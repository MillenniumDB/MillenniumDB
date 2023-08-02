#include "string_manager.h"

#include <cassert>
#include <sys/mman.h>

#include "query/exceptions.h"
#include "storage/file_manager.h"

// memory for the object
static typename std::aligned_storage<sizeof(StringManager), alignof(StringManager)>::type string_manager_buf;
// global object
StringManager& string_manager = reinterpret_cast<StringManager&>(string_manager_buf);


void StringManager::init(uint64_t max_initial_populate_size) {
    new (&string_manager) StringManager(max_initial_populate_size); // placement new
}


StringManager::StringManager(uint64_t max_initial_populate_size) :
    str_file_id (file_manager.get_file_id("strings.dat")),
    str_hash    ("str_hash")
{
    uint64_t string_file_size = lseek(str_file_id.id, 0, SEEK_END);
    assert(string_file_size % STRING_BLOCK_SIZE == 0);

    auto number_of_blocks = string_file_size / STRING_BLOCK_SIZE;

    auto max_initial_populate_size_in_bytes = max_initial_populate_size*1024*1024*1024;
    auto max_populate_blocks = max_initial_populate_size_in_bytes / STRING_BLOCK_SIZE;

    auto populate_blocks = (max_populate_blocks < number_of_blocks)
                         ? max_populate_blocks
                         : number_of_blocks;

    for (uint64_t i = 0; i < populate_blocks; i++) {
        // mmap will return correct alignment
        auto bytes = reinterpret_cast<char*>(mmap(NULL,
                                                  STRING_BLOCK_SIZE,
                                                  PROT_READ|PROT_WRITE,
                                                  MAP_SHARED|MAP_POPULATE,
                                                  str_file_id.id,
                                                  i*STRING_BLOCK_SIZE));
        // TODO: try madvise random and measure for performance changes
        string_blocks.push_back(bytes);
    }

    for (uint64_t i = populate_blocks; i < number_of_blocks; i++) {
        // mmap will return correct alignment
        auto bytes = reinterpret_cast<char*>(mmap(NULL,
                                                  STRING_BLOCK_SIZE,
                                                  PROT_READ|PROT_WRITE,
                                                  MAP_SHARED,
                                                  str_file_id.id,
                                                  i*STRING_BLOCK_SIZE));
        // TODO: try madvise random and measure for performance changes
        string_blocks.push_back(bytes);
    }

    last_block_offset = *reinterpret_cast<uint64_t*>(string_blocks[0]);
}


StringManager::~StringManager() {
    // munmap(bytes, lseek(str_file_id.id, 0, SEEK_END));
    // TODO: do we really need to unmap block by block?
    for (auto block : string_blocks) {
        munmap(block, STRING_BLOCK_SIZE);
    }
}


void StringManager::print(std::ostream& os, uint64_t id) const {
    uint64_t current_block_number = id/STRING_BLOCK_SIZE;
    char* current_block = string_blocks[current_block_number];
    char* ptr = current_block + (id % STRING_BLOCK_SIZE);

    uint64_t bytes_for_len;
    uint64_t len = get_string_len(ptr, &bytes_for_len);
    ptr += bytes_for_len;

    auto current_offset = (id + bytes_for_len) % STRING_BLOCK_SIZE;
    auto remaining_string = len;

    // TODO: We suppose that every string is smaller than STRING_BLOCK_SIZE
    // so we don't use the while (true)
    assert(remaining_string < STRING_BLOCK_SIZE);
    auto remaining_in_page = STRING_BLOCK_SIZE - current_offset;
    if (remaining_string < remaining_in_page) {
        os.write(reinterpret_cast<const char*>(ptr), remaining_string);
    } else {
        os.write(reinterpret_cast<const char*>(ptr), remaining_in_page);

        current_block_number++;
        ptr = string_blocks[current_block_number];
        remaining_string -= remaining_in_page;
        assert(remaining_string < STRING_BLOCK_SIZE);
        os.write(reinterpret_cast<const char*>(ptr), remaining_string);
    }
    // while (true) {
    //     auto remaining_in_page = STRING_BLOCK_SIZE - current_offset;
    //     if (remaining_string < remaining_in_page) {
    //         os.write(reinterpret_cast<const char*>(ptr), remaining_string);
    //         break;
    //     } else {
    //         os.write(reinterpret_cast<const char*>(ptr), remaining_in_page);

    //         current_block_number++;
    //         ptr = string_blocks[current_block_number];
    //         remaining_string -= remaining_in_page;
    //         current_offset = 0;
    //     }
    // }
}


bool StringManager::bytes_eq(const char* bytes, uint64_t size, uint64_t id) const {
    char* current_block = string_blocks[id/STRING_BLOCK_SIZE];
    char* ptr = current_block + (id % STRING_BLOCK_SIZE);

    uint64_t bytes_for_len;
    uint64_t len = get_string_len(ptr, &bytes_for_len);

    if (len != size) {
        return false;
    }

    StringExternalIter char_iter1(id + bytes_for_len, len);
    BytesIter          char_iter2(bytes, size);

    return compare(char_iter1, char_iter2) == 0;
}


uint64_t StringManager::get_bytes_id(const char* bytes, uint64_t size, bool create_if_not_exists) {
    if (create_if_not_exists) {
        return str_hash.get_or_create_str_id(bytes, size);
    } else {
        return str_hash.get_str_id(bytes, size);
    }
}


std::unique_ptr<CharIter> StringManager::get_char_iter(uint64_t id) const {
    char* current_block = string_blocks[id/STRING_BLOCK_SIZE];
    char* ptr = current_block + (id % STRING_BLOCK_SIZE);

    uint64_t bytes_for_len;
    uint64_t len = get_string_len(ptr, &bytes_for_len);

    return std::make_unique<StringExternalIter>(id + bytes_for_len, len);
}


uint64_t StringManager::create_new(const char* bytes, uint64_t size) {
    // TODO: put mutex if multiple inserts at the same time are suported

    uint64_t bytes_for_len = get_bytes_for_len(size);

    // create new block if len can't be encoded in current last block
    if (last_block_offset + bytes_for_len >= STRING_BLOCK_SIZE) {
        append_new_block();
    }

    // encode length
    auto res = (string_blocks.size() - 1) * STRING_BLOCK_SIZE + last_block_offset;
    auto* ptr = string_blocks.back() + last_block_offset;

    uint64_t remaining_len = size;
    while (remaining_len != 0) {
        if (remaining_len <= 127) {
            *ptr = static_cast<char>(remaining_len);
        } else {
            *ptr = static_cast<char>(remaining_len & 0x7FUL) | 0x80;
        }
        remaining_len = remaining_len >> 7;
        ptr++;
        last_block_offset++;
    }


    // copy string
    uint64_t remaining_write = size;
    // TODO: We suppose that every string is smaller than STRING_BLOCK_SIZE
    // so we don't use the while (true)
    assert(remaining_write < STRING_BLOCK_SIZE);
    auto str_ptr = bytes;
    uint64_t remaining_in_block = STRING_BLOCK_SIZE - last_block_offset;

    if (remaining_in_block >= remaining_write) {
        std::memcpy(ptr, str_ptr, remaining_write);
        last_block_offset += remaining_write;
    } else {
        std::memcpy(ptr, str_ptr, remaining_in_block);
        remaining_write -= remaining_in_block;
        str_ptr += remaining_in_block;
        append_new_block(); // sets last_block_offset = 0
        ptr = string_blocks.back() /* + last_block_offset*/;
        std::memcpy(ptr, str_ptr, remaining_write);
        last_block_offset = remaining_write /* + last_block_offset*/;
    }
    // auto str_ptr = &str[0];
    // while (remaining_write > 0) {
    //     uint64_t remaining_in_block = STRING_BLOCK_SIZE - last_block_offset;

    //     if (remaining_in_block >= remaining_write) {
    //         std::memcpy(ptr, str_ptr, remaining_write);
    //         last_block_offset += remaining_write;
    //         remaining_write = 0;
    //     } else {
    //         std::memcpy(ptr, str_ptr, remaining_in_block);
    //         remaining_write -= remaining_in_block;
    //         append_new_block();
    //         str_ptr += remaining_in_block;
    //         ptr = string_blocks.back() + last_block_offset;
    //     }
    // }

    update_last_block_offset();
    return res;
}


void StringManager::append_new_block() {
    // TODO: move resize to file_manager?
    // resize file on disk
    auto file_descriptor = str_file_id.id;
    lseek(file_descriptor, 0, SEEK_END);

    // the second parameter does not matter, anything can be written
    auto write_res = write(file_descriptor, string_blocks[0], STRING_BLOCK_SIZE);

    if (write_res != STRING_BLOCK_SIZE) {
        throw std::runtime_error("Error appending new block to string manager");
    }

    auto new_block = reinterpret_cast<char*>(mmap(NULL,
                                                  STRING_BLOCK_SIZE,
                                                  PROT_READ|PROT_WRITE,
                                                  MAP_PRIVATE|MAP_POPULATE,
                                                  file_descriptor,
                                                  string_blocks.size()*STRING_BLOCK_SIZE));
    string_blocks.push_back(new_block);
    last_block_offset = 0;
}
