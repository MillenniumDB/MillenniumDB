#include "catalog.h"

#include <cassert>

#include "storage/file_manager.h"

using namespace std;

Catalog::Catalog(const string& filename) :
    file (file_manager.get_file(file_manager.get_file_id(filename))) { }


bool Catalog::is_empty() {
    file.seekg(0, file.end);
    return file.tellg() == 0;
}


void Catalog::start_io() {
    file.seekg(0, file.beg);
}

bool Catalog::check_no_error_flags() {
    return file.good();
}


uint64_t Catalog::read_uint64() {
    uint64_t res = 0;
    uint8_t buf[8];
    file.read((char*)buf, sizeof(buf));

    for (int i = 0, shift = 0; i < 8; ++i, shift += 8) {
        res |= static_cast<uint64_t>(buf[i]) << shift;
    }
    return res;
}


uint_fast32_t Catalog::read_uint32() {
    uint_fast32_t res = 0;
    uint8_t buf[4];
    file.read((char*)buf, sizeof(buf));

    for (int i = 0, shift = 0; i < 4; ++i, shift += 8) {
        res |= static_cast<uint_fast32_t>(buf[i]) << shift;
    }
    return res;
}


void Catalog::write_uint64(const uint64_t n) {
    uint8_t buf[8];
    for (unsigned int i = 0, shift = 0; i < sizeof(buf); ++i, shift += 8) {
        buf[i] = (n >> shift) & 0xFF;
    }
    file.write(reinterpret_cast<const char*>(buf), sizeof(buf));
}


void Catalog::write_uint32(const uint_fast32_t n) {
    uint8_t buf[4];
    for (unsigned int i = 0, shift = 0; i < sizeof(buf); ++i, shift += 8) {
        buf[i] = (n >> shift) & 0xFF;
    }
    file.write(reinterpret_cast<const char*>(buf), sizeof(buf));
}
