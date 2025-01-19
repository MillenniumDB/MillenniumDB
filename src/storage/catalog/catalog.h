#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include <boost/unordered_map.hpp>

class Catalog {
protected:
    Catalog(const std::string& filename);
    ~Catalog();

    bool is_empty();

    // should be called before start reading/writing the catalog
    void start_io();

    uint8_t read_uint8();
    uint64_t read_uint64();
    uint_fast32_t read_uint32();
    std::string read_string();
    std::vector<std::string> read_strvec();
    boost::unordered_map<uint64_t, uint64_t> read_map();

    void write_uint8(const uint8_t);
    void write_uint64(const uint64_t);
    void write_uint32(const uint_fast32_t);
    void write_string(const std::string&);
    void write_strvec(const std::vector<std::string>& strvec);
    void write_map(const boost::unordered_map<uint64_t, uint64_t>&);

    boost::unordered_map<std::string, uint64_t> convert_strvec_to_map(const std::vector<std::string>& strvec);

private:
    std::fstream file;
};
