#include "string_manager.h"

#include <cassert>
#include <sys/mman.h>

#include "base/exceptions.h"
#include "execution/graph_object/char_iter.h"
#include "storage/file_manager.h"

// memory for the object
static typename std::aligned_storage<sizeof(StringManager), alignof(StringManager)>::type string_manager_buf;
// global object
StringManager& string_manager = reinterpret_cast<StringManager&>(string_manager_buf);


void StringManager::init() {
    new (&string_manager) StringManager(); // placement new
}


StringManager::StringManager() :
    str_file_id (file_manager.get_file_id("strings.dat")),
    str_hash    ("str_hash")
{
    size_t string_file_size = lseek(str_file_id.id, 0, SEEK_END);
    assert(string_file_size % STRING_BLOCK_SIZE == 0);

    auto number_of_blocks = string_file_size / STRING_BLOCK_SIZE;

    for (size_t i = 0; i < number_of_blocks; i++) {
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
    size_t current_block_number = id/STRING_BLOCK_SIZE;
    char* current_block = string_blocks[current_block_number];
    char* ptr = current_block + (id % STRING_BLOCK_SIZE);

    size_t bytes_for_len;
    size_t len = get_string_len(ptr, &bytes_for_len);
    ptr += bytes_for_len;

    auto current_offset = (id + bytes_for_len) % STRING_BLOCK_SIZE;
    auto remaining_string = len;

    while (true) {
        auto remaining_in_page = STRING_BLOCK_SIZE - current_offset;
        if (remaining_string < remaining_in_page) {
            os.write(reinterpret_cast<const char*>(ptr), remaining_string);
            break;
        } else {
            os.write(reinterpret_cast<const char*>(ptr), remaining_in_page);

            current_block_number++;
            ptr = string_blocks[current_block_number];
            remaining_string -= remaining_in_page;
            current_offset = 0;
        }
    }
}


bool StringManager::str_eq(const std::string& str, uint64_t id) const {
    char* current_block = string_blocks[id/STRING_BLOCK_SIZE];
    char* ptr = current_block + (id % STRING_BLOCK_SIZE);

    size_t bytes_for_len;
    size_t len = get_string_len(ptr, &bytes_for_len);

    if (len != str.size()) {
        return false;
    }

    StringExternalIter char_iter1(id + bytes_for_len, len);
    StringTmpIter      char_iter2(str);

    return compare(char_iter1, char_iter2) == 0;
}


uint64_t StringManager::get_str_id(const std::string& str, bool create_if_not_exists) {
    if (create_if_not_exists) {
        return str_hash.get_or_create_str_id(str);
    } else {
        return str_hash.get_str_id(str);
    }
}


std::unique_ptr<CharIter> StringManager::get_char_iter(uint64_t id) const {
    char* current_block = string_blocks[id/STRING_BLOCK_SIZE];
    char* ptr = current_block + (id % STRING_BLOCK_SIZE);

    size_t bytes_for_len;
    size_t len = get_string_len(ptr, &bytes_for_len);

    return std::make_unique<StringExternalIter>(id + bytes_for_len, len);
}


uint64_t StringManager::create_new(const std::string& str) {
    // TODO: put mutex if multiple inserts at the same time are suported

    size_t bytes_for_len = (__builtin_ctz(str.size()) - 1) / 7 + 1;

    // create new block if len can't be encoded in current last block
    if (last_block_offset + bytes_for_len >= STRING_BLOCK_SIZE) {
        append_new_block();
    }

    // encode length
    auto res = (string_blocks.size() - 1) * STRING_BLOCK_SIZE + last_block_offset;
    auto* ptr = string_blocks.back() + last_block_offset;

    size_t remaining_len = str.size();
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
    size_t remaining_write = str.size();
    while (remaining_write > 0) {
        size_t remaining_in_block = STRING_BLOCK_SIZE - last_block_offset;

        if (remaining_in_block >= remaining_write) {
            std::memcpy(ptr, &str[0], remaining_write);
            last_block_offset += remaining_write;
            remaining_write = 0;
        } else {
            std::memcpy(ptr, &str[0], remaining_in_block);
            remaining_write -= remaining_in_block;
            append_new_block();
            ptr = string_blocks.back() + last_block_offset;
        }
    }

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