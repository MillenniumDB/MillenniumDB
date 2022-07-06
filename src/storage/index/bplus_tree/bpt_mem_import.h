#pragma once

#include <array>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <tuple>
#include <vector>

#include "storage/page.h"

template <std::size_t N>
class BPTLeafWriter {
public:
    static constexpr auto max_records = (Page::MDB_PAGE_SIZE - 2*sizeof(int32_t)) / (sizeof(uint64_t)*N);

    BPTLeafWriter(const std::string& filename) {
        file.open(filename, std::ios::out|std::ios::binary);
        buffer = new char[Page::MDB_PAGE_SIZE];
    }

    ~BPTLeafWriter() {
        file.close();
        delete[] buffer;
    }

    void process_block(char* bytes, uint32_t size, uint32_t next_block) {
        auto value_count = reinterpret_cast<uint32_t*>(buffer);
        auto next_leaf   = reinterpret_cast<uint32_t*>(buffer + sizeof(uint32_t));

        memset(buffer, 0, Page::MDB_PAGE_SIZE);
        *value_count = size;
        *next_leaf = next_block;
        std::memcpy(buffer + 2*sizeof(uint32_t), bytes, size*(N*sizeof(uint64_t)));
        file.write(buffer, Page::MDB_PAGE_SIZE);
    }

    void make_empty() {
        memset(buffer, 0, Page::MDB_PAGE_SIZE);
        file.write(buffer, Page::MDB_PAGE_SIZE);
    }

private:
    std::fstream file;

    char* buffer;
};

template <std::size_t N>
struct SplitData {
    const std::array<uint64_t, N>* record;
    int32_t encoded_page_number;
    bool need_split;

    SplitData(const std::array<uint64_t, N>* record,
              int32_t encoded_page_number,
              bool need_split) :
        record              (record),
        encoded_page_number (encoded_page_number),
        need_split          (need_split) { }

    SplitData() = default;
};


template <std::size_t N>
class BPTDirWriter {
private:
    std::fstream file;
    std::vector<char*> pages;

public:
    static constexpr auto max_records = (Page::MDB_PAGE_SIZE - 2*sizeof(int32_t) )
                                         / (sizeof(uint64_t)*N + sizeof(int32_t));

    BPTDirWriter(const std::string& filename) {
        file.open(filename, std::ios::out|std::ios::binary);
        if (file.fail()) {
            std::cout << "Error opening file " << filename << std::endl;
        }
        auto root = new char[Page::MDB_PAGE_SIZE];
        memset(root, 0, Page::MDB_PAGE_SIZE);
        pages.push_back(root);
    }

    ~BPTDirWriter() {
        for (auto page : pages) {
            file.write(page, Page::MDB_PAGE_SIZE);
            delete[] page;
        }
        file.close();
    }

    uint64_t* get_keys(int32_t dir_page_number) {
        return reinterpret_cast<uint64_t*>(pages[dir_page_number]);
    }

    uint32_t* get_key_count(int32_t dir_page_number) {
        return reinterpret_cast<uint32_t*>(pages[dir_page_number]
                                           + (sizeof(uint64_t) * max_records * N));
    }

    int32_t* get_children(int32_t dir_page_number) {
        return reinterpret_cast<int32_t*>(pages[dir_page_number]
                                          + (sizeof(uint64_t) * max_records * N)
                                          + sizeof(uint32_t));
    }

    SplitData<N> bulk_insert(const std::array<uint64_t, N>* record,
                             int32_t dir_page_number,
                             int32_t leaf_page_number)
    {
        uint64_t* keys      = get_keys(dir_page_number);
        uint32_t* key_count = get_key_count(dir_page_number);
        int32_t* children   = get_children(dir_page_number);

        SplitData<N> split_data;

        if (children[*key_count] < 0) {
            // negative number: pointer to dir
            split_data = bulk_insert(record, children[*key_count]*-1, leaf_page_number);
        } else {
            // positive number: pointer to leaf
            split_data = SplitData(record, leaf_page_number, true);
        }

        if (split_data.need_split) {
            // Case 1: no need to split this node
            if (*key_count < max_records) {
                // update key
                std::memcpy(&keys[(*key_count)*N],
                            split_data.record->data(),
                            N * sizeof(uint64_t));
                ++(*key_count);
                // update child
                children[*key_count] = split_data.encoded_page_number;
                return SplitData<N>(nullptr, 0, false);
            }
            // Case 2: non-root split
            else if (dir_page_number != 0) {
                // create new dir page
                int32_t new_page_number = pages.size();
                auto new_page = new char[Page::MDB_PAGE_SIZE];
                memset(new_page, 0, Page::MDB_PAGE_SIZE);
                pages.push_back(new_page);

                auto new_dir_children = get_children(new_page_number);
                auto new_dir_key_count = get_key_count(new_page_number);

                new_dir_children[0] = split_data.encoded_page_number;
                *new_dir_key_count = 0;
                return SplitData<N>(split_data.record, new_page_number*-1, true);
            }
            // Case 3: root split
            else {
                // create 2 new pages (new_lhs, new_rhs)
                int32_t lhs_page_number = pages.size();
                auto lhs_page = new char[Page::MDB_PAGE_SIZE];
                memset(lhs_page, 0, Page::MDB_PAGE_SIZE);
                pages.push_back(lhs_page);

                int32_t rhs_page_number = pages.size();
                auto rhs_page = new char[Page::MDB_PAGE_SIZE];
                memset(rhs_page, 0, Page::MDB_PAGE_SIZE);
                pages.push_back(rhs_page);

                // new_lhs has everything previous
                std::memcpy(lhs_page, pages[dir_page_number], Page::MDB_PAGE_SIZE);

                // new_rhs has 0 keys and 1 record (the splitted record)
                auto rhs_key_count = get_key_count(rhs_page_number);
                auto rhs_children  = get_children(rhs_page_number);
                *rhs_key_count = 0;
                rhs_children[0] = split_data.encoded_page_number;

                // new root will have the new pages as children
                std::memcpy(keys,
                            split_data.record->data(),
                            N * sizeof(uint64_t) );
                *key_count = 1;
                children[0] = lhs_page_number * -1;
                children[1] = rhs_page_number * -1;

                return SplitData<N>(nullptr, 0, false);
            }
        }
        else {
            return SplitData<N>(nullptr, 0, false);
        }
    }
};
