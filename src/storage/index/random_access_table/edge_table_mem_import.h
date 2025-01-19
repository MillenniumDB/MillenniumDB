#pragma once

#include <array>
#include <cstring>
#include <fstream>
#include <ios>

#include "storage/index/random_access_table/random_access_table_block.h"
#include "storage/page/versioned_page.h"

// N is the number of elements (not counting the edge). For quad_model N=4
// Creates a RandomAccessTable<N-1>
template<std::size_t N>
class EdgeTableMemImport {
public:
    static constexpr auto max_records = RandomAccessTableBlock<N-1>::max_records;

    EdgeTableMemImport(const std::string& filename)
    {
        file.open(filename, std::ios::out | std::ios::binary);
        buffer = new char[VPage::SIZE];
        record_count = reinterpret_cast<uint32_t*>(buffer + ((N - 1) * sizeof(uint64_t) * max_records));
        *record_count = 0;
        current_pos = 0;
    }

    ~EdgeTableMemImport()
    {
        file.write(buffer, VPage::SIZE);
        file.close();
        delete[] buffer;
    }

    void insert_tuple(const std::array<uint64_t, N>& edge)
    {
        auto records = reinterpret_cast<uint64_t*>(buffer);
        if (*record_count < max_records) {
            for (std::size_t i = 0; i < N; i++) {
                records[((*record_count) * (N - 1)) + i] = edge[i];
            }
            (*record_count)++;
        } else {
            file.write(buffer, VPage::SIZE);
            for (std::size_t i = 0; i < N; i++) {
                records[i] = edge[i];
            }
            *record_count = 1;
        }
    }

private:
    std::fstream file;

    uint32_t* record_count;

    size_t current_pos;

    char* buffer;
};
