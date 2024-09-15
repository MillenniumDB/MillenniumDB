#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

class CharIter;

struct ThreadTempInfo {
    ThreadTempInfo() = default;

    std::unordered_map<std::string, uint64_t> str_to_id;
    std::vector<std::string> id_to_str;
    uint64_t next_str_id = 0;

    void reset() {
        next_str_id = 0;
        str_to_id.clear();
        id_to_str.clear();
    }
};

class TmpManager {
public:
    // necessary to be called before first usage
    static void init(int num_workers);

    ~TmpManager();

    std::unique_ptr<CharIter> get_str_char_iter(uint64_t id) const;

    const std::string& get_str(uint64_t id) const;

    void print_str(std::ostream& os, uint64_t id) const;

    uint64_t get_str_id(const std::string&);

    uint64_t get_bytes_id(const char* bytes, uint64_t size) {
        std::string str_cpy(bytes, bytes + size);
        return get_str_id(str_cpy);
    }

    // should be called at the start of every query
    void reset(uint_fast32_t worker_idx);

private:
    TmpManager(int num_workers);

    std::vector<ThreadTempInfo> info;
};

extern TmpManager& tmp_manager; // global object
