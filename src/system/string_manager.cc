#include "string_manager.h"

#include <cassert>
#include <mutex>
#include <sys/mman.h>

#include "graph_models/object_id.h"
#include "query/exceptions.h"
#include "query/query_context.h"
#include "system/file_manager.h"

#if __linux__
#include <linux/version.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,22)
#define _MAP_POPULATE_AVAILABLE
#endif
#endif

#ifdef _MAP_POPULATE_AVAILABLE
#define MAP_POPULATE_ | MAP_POPULATE
#else
#define MAP_POPULATE_
#endif


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

    auto max_populate_blocks = max_initial_populate_size / STRING_BLOCK_SIZE;

    auto populate_blocks = (max_populate_blocks < number_of_blocks)
                         ? max_populate_blocks
                         : number_of_blocks;

    string_blocks.reserve(64*1024); // for 8TB max string file size (with block size 64MB)

    for (uint64_t i = 0; i < populate_blocks; i++) {
        // mmap will return correct alignment
        auto bytes = reinterpret_cast<char*>(mmap(NULL,
                                                  STRING_BLOCK_SIZE,
                                                  PROT_READ|PROT_WRITE,
                                                  MAP_SHARED MAP_POPULATE_,
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

    // We suppose that every string is smaller than STRING_BLOCK_SIZE
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
}


uint64_t StringManager::print_to_buffer(char* buffer, uint64_t id) {
    uint64_t current_block_number = id/STRING_BLOCK_SIZE;
    char* current_block = string_blocks[current_block_number];
    char* ptr = current_block + (id % STRING_BLOCK_SIZE);

    uint64_t bytes_for_len;
    const uint64_t len = get_string_len(ptr, &bytes_for_len);
    ptr += bytes_for_len;

    auto current_offset = (id + bytes_for_len) % STRING_BLOCK_SIZE;
    auto remaining_string = len;

    // We suppose that every string is smaller than STRING_BLOCK_SIZE
    assert(remaining_string < STRING_BLOCK_SIZE);
    auto remaining_in_page = STRING_BLOCK_SIZE - current_offset;
    if (remaining_string < remaining_in_page) {
        memcpy(buffer, ptr, remaining_string);
    } else {
        memcpy(buffer, ptr, remaining_in_page);

        current_block_number++;
        ptr = string_blocks[current_block_number];
        remaining_string -= remaining_in_page;
        assert(remaining_string < STRING_BLOCK_SIZE);

        memcpy(buffer + remaining_in_page, ptr, remaining_string);
    }
    return len;
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


uint64_t StringManager::get_bytes_id(const char* bytes, uint64_t size) const {
    std::shared_lock lock(mutex);
    return str_hash.get_str_id(bytes, size);
}


std::unique_ptr<CharIter> StringManager::get_char_iter(uint64_t id) const {
    char* current_block = string_blocks[id/STRING_BLOCK_SIZE];
    char* ptr = current_block + (id % STRING_BLOCK_SIZE);

    uint64_t bytes_for_len;
    uint64_t len = get_string_len(ptr, &bytes_for_len);

    return std::make_unique<StringExternalIter>(id + bytes_for_len, len);
}


uint64_t StringManager::get_or_create(const char* bytes, uint64_t size) {
    {
        std::shared_lock lock(mutex);
        auto existing_id = str_hash.get_str_id(bytes, size);
        if (existing_id != ObjectId::MASK_NOT_FOUND) {
            return existing_id;
        }
    }

    // need to create
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
    // We suppose that every string is smaller than STRING_BLOCK_SIZE
    // so we don't use the while at most write 2 blocks
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

    update_last_block_offset();
    // TODO: should have some kind of WAL for recovery of the string file

    {
        std::unique_lock lock(mutex);
        str_hash.create_str_id(bytes, size, res);
    }
    return res;
}


void StringManager::append_new_block() {
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
                                                  MAP_PRIVATE MAP_POPULATE_,
                                                  file_descriptor,
                                                  string_blocks.size()*STRING_BLOCK_SIZE));
    string_blocks.push_back(new_block);
    last_block_offset = 0;
}
