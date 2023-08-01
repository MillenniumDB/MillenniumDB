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

    std::unordered_map<std::string, uint64_t> dtt_to_id;
    std::vector<std::string> id_to_dtt;
    uint64_t next_dtt_id = 0;

    std::unordered_map<std::string, uint64_t> lan_to_id;
    std::vector<std::string> id_to_lan;
    uint64_t next_lan_id = 0;

    void reset() {
        next_str_id = 0;
        next_dtt_id = 0;
        next_lan_id = 0;
        str_to_id.clear();
        dtt_to_id.clear();
        lan_to_id.clear();

        id_to_str.clear();
        id_to_dtt.clear();
        id_to_lan.clear();
    }
};

class TmpManager {
public:
    // necessary to be called before first usage
    static void init(int num_workers);

    ~TmpManager();

    std::unique_ptr<CharIter> get_str_char_iter(uint64_t id) const;
    std::unique_ptr<CharIter> get_language_char_iter(uint64_t id) const;
    std::unique_ptr<CharIter> get_datatype_char_iter(uint64_t id) const;

    void print_str(std::ostream& os, uint64_t id) const;
    void print_dtt(std::ostream& os, uint64_t id) const;
    void print_lan(std::ostream& os, uint64_t id) const;

    uint64_t get_str_id(const std::string&);
    uint64_t get_dtt_id(const std::string&);
    uint64_t get_lan_id(const std::string&);

    uint64_t get_bytes_id(const char* bytes, uint64_t size) {
        std::string str_cpy(bytes, bytes + size);
        return get_str_id(str_cpy);
    }

    // should be called at the start of every query
    void reset();

private:
    TmpManager(int num_workers);

    std::vector<ThreadTempInfo> info;
};

extern TmpManager& tmp_manager; // global object
