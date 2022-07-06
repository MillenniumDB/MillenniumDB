#pragma once

#include <array>
#include <fstream>
#include <cstring>
#include <ios>

#include "storage/page.h"

class EdgeTableMemImport {
public:
    static constexpr auto max_records = (Page::MDB_PAGE_SIZE - sizeof(uint32_t)) / (sizeof(uint64_t) * 3);

    EdgeTableMemImport(const std::string& filename) {
        file.open(filename, std::ios::out|std::ios::binary);
        buffer = new char[Page::MDB_PAGE_SIZE];
        record_count = reinterpret_cast<uint32_t*>(buffer + (3*sizeof(uint64_t)*max_records));
        *record_count = 0;
        current_pos = 0;
    }

    ~EdgeTableMemImport() {
        file.write(buffer, Page::MDB_PAGE_SIZE);
        file.close();
        delete[] buffer;
    }

    void insert_tuple(const std::array<uint64_t, 4>& edge) {
        auto records = reinterpret_cast<uint64_t*>(buffer);
        if (*record_count < max_records) {
            records[ (*record_count)*3 ]     = edge[0];
            records[ (*record_count)*3 + 1 ] = edge[1];
            records[ (*record_count)*3 + 2 ] = edge[2];
            (*record_count)++;
        } else {
            file.write(buffer, Page::MDB_PAGE_SIZE);
            records[0] = edge[0];
            records[1] = edge[1];
            records[2] = edge[2];
            (*record_count) = 1;
        }
    }

private:
    std::fstream file;

    uint32_t* record_count;

    size_t current_pos;

    char* buffer;
};
