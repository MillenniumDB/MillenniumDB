#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <stdexcept>
#include <vector>
#include <thread>

#include "stats_processor.h"
#include "storage/page.h"
#include "storage/index/bplus_tree/bpt_mem_import.h"

namespace Import {
template <std::size_t N>
struct RunRecordComparator {
    constexpr bool operator()(const std::pair<std::array<uint64_t, N>, size_t>& lhs,
                              const std::pair<std::array<uint64_t, N>, size_t>& rhs) const
    {
        return lhs.first > rhs.first;
    }
};

template <std::size_t N>
class DiskVector {
public:
    size_t total_tuples; // will be updated only after calling finish_appends()

    DiskVector(const std::string& filename) :
        filename (filename),
        buffer_size  (Page::MDB_PAGE_SIZE*N*sizeof(uint64_t)),
        buffer_count (0),
        buffer (reinterpret_cast<char*>(std::aligned_alloc(Page::MDB_PAGE_SIZE, buffer_size)))
    {
        // TODO: throw exception if we can't order with 1 merge
        file.open(filename, std::ios::out|std::ios::app);
        if (file.fail()) {
            throw std::runtime_error("Could not open file " + filename);
        }
        file.close();
        file.open(filename, std::ios::in|std::ios::out|std::ios::binary);
    }

    // returns the tuple count
    void create_bpt(const std::string& base_name,
                    std::array<size_t, N>&& new_permutation,
                    StatsProcessor<N>& stat_processor)
    {
        reorder_cols(move(new_permutation));
        merge_sort(base_name, stat_processor);
    }

    void finish_appends() {
        file.write(buffer, buffer_count*N*sizeof(uint64_t));
        buffer_count = 0;
        size_t file_length = file.tellg();
        total_tuples = file_length / (N*sizeof(uint64_t));
    }

    void start_indexing(char* new_buffer,
                        size_t max_buffer_size,
                        std::array<size_t, N> original_permutation)
    {
        // free append buffer
        free(buffer);
        current_permutation = original_permutation;
        // buffer_size must be divisible by (Page::MDB_PAGE_SIZE*N*sizeof(uint64_t)
        auto max_blocks_per_run = max_buffer_size / (Page::MDB_PAGE_SIZE*N*sizeof(uint64_t));
        auto new_buffer_size = max_blocks_per_run * (Page::MDB_PAGE_SIZE*N*sizeof(uint64_t));
        this->buffer_size = new_buffer_size;
        buffer = new_buffer;
    }

    void finish_indexing() {
        file.close();
        remove(filename.c_str());
    }

    void push_back(const std::array<uint64_t, N>& record) {
        std::memcpy(&buffer[buffer_count*N*sizeof(uint64_t)],
                    record.data(),
                    N * sizeof(uint64_t));
        buffer_count++;
        if (buffer_count == Page::MDB_PAGE_SIZE) {
            file.write(buffer, buffer_size);
            buffer_count = 0;
        }
    }

    void begin_tuple_iter() {
        file.seekg(0, file.beg);
        current_tuple_iter = 0;
        // set iter_buffer_offset so its 0 when next_tuple() is called the first time
        iter_buffer_offset = Page::MDB_PAGE_SIZE - 1;
    }

    bool has_next_tuple() {
        return current_tuple_iter < total_tuples;
    }

    std::array<size_t, N>& next_tuple() {
        // max tuples in buffer is Page::MDB_PAGE_SIZE
        iter_buffer_offset = (iter_buffer_offset+1) % Page::MDB_PAGE_SIZE;
        current_tuple_iter++;

        if (iter_buffer_offset == 0) {
            // leer del disco a buffer
            file.read(buffer, Page::MDB_PAGE_SIZE*N*sizeof(uint64_t));
            file.clear();
        }
        auto ptr = reinterpret_cast<std::array<size_t, N>*>(buffer) + iter_buffer_offset;
        return *ptr;
    }

private:
    constexpr size_t division_round_up(size_t a, size_t b) {
        return (a / b) + (a % b != 0);
    }

    void merge_sort(const std::string& base_name, StatsProcessor<N>& stat_processor) {
        BPTLeafWriter<N> leaf_writer(base_name + ".leaf");
        BPTDirWriter<N> dir_writer(base_name + ".dir");

        file.seekg(0, file.end);
        size_t file_length = file.tellg();

        if (file_length == 0) {
            leaf_writer.make_empty();
            return;
        }

        // A run is a set of ordered tuples (ordered when method reorder_cols was called)
        // A run is divided in blocks. Tuples from disk are readed 1 block at a time
        // buffer_size was chosen to be a multiple of block_size
        const size_t block_size = Page::MDB_PAGE_SIZE*N*sizeof(uint64_t);

        const size_t total_runs = division_round_up(file_length, buffer_size);
        const size_t total_blocks = division_round_up(file_length, block_size);


        const size_t max_tuples_per_block = Page::MDB_PAGE_SIZE;
        const size_t max_blocks_per_run = buffer_size / block_size;


        const size_t tuples_in_last_block = (total_tuples % max_tuples_per_block == 0)
                                            ? max_tuples_per_block
                                            : (total_tuples % max_tuples_per_block);

        const size_t blocks_in_last_run = (total_blocks % max_blocks_per_run == 0)
                                            ? max_blocks_per_run
                                            : (total_blocks % max_blocks_per_run);

        assert(total_runs*block_size + (BPTLeafWriter<N>::max_records*N*sizeof(uint64_t)) <= buffer_size);
        file.seekg(0, file.beg);

        if (total_runs == 1) {
            uint32_t leaf_current_block = 0;
            size_t current_tuple = 0;
            while (current_tuple < total_tuples) {
                file.read(buffer, N*sizeof(uint64_t)*BPTLeafWriter<N>::max_records);

                // skip first leaf from going into bulk_import
                if (current_tuple > 0) {
                    dir_writer.bulk_insert(reinterpret_cast<std::array<uint64_t, N>*>(buffer),
                                           0,
                                           leaf_current_block);
                }

                if (current_tuple + leaf_writer.max_records < total_tuples) {
                    current_tuple += leaf_writer.max_records;
                    leaf_writer.process_block(buffer,
                                              leaf_writer.max_records,
                                              ++leaf_current_block);
                    for (size_t i = 0; i < leaf_writer.max_records; i++) {
                        stat_processor.process_tuple(*(reinterpret_cast<std::array<uint64_t, N>*>(buffer) + i));
                    }
                } else {
                    auto remaining_tuples = total_tuples - current_tuple;
                    leaf_writer.process_block(buffer,
                                              remaining_tuples,
                                              0);
                    for (size_t i = 0; i < remaining_tuples; i++) {
                        stat_processor.process_tuple(*(reinterpret_cast<std::array<uint64_t, N>*>(buffer) + i));
                    }
                    break;
                }
            }
            file.clear();
            return;
        }

        std::priority_queue<std::pair<std::array<uint64_t, N>, size_t>,               // tuple: (record, run_number)
                            std::vector< std::pair<std::array<uint64_t, N>, size_t>>, // container
                            RunRecordComparator<N>                                    // comparator
                            > queue;

        // Fill buffers and put the first tuple of the run in the heap
        std::array<uint64_t, N>** start_pos   = new std::array<uint64_t, N>*[total_runs];
        std::array<uint64_t, N>** end_pos     = new std::array<uint64_t, N>*[total_runs];
        std::array<uint64_t, N>** current_pos = new std::array<uint64_t, N>*[total_runs];

        size_t* current_block = new size_t[total_runs];
        size_t* end_block     = new size_t[total_runs];

        for (size_t run = 0; run < total_runs; ++run) {
            start_pos[run] = reinterpret_cast<std::array<uint64_t, N>*>(buffer + (run*block_size));
            end_pos[run]   = reinterpret_cast<std::array<uint64_t, N>*>(buffer + ((run+1)*block_size));
            current_pos[run] = start_pos[run];
            current_block[run] = 0;
            end_block[run] = max_blocks_per_run;

            file.seekg(buffer_size * run, file.beg);
            file.read(reinterpret_cast<char*>(start_pos[run]), block_size);
            queue.push(std::make_pair(*current_pos[run], run));
        }
        file.clear();
        // the last run and last block are special cases
        auto last_run = total_runs - 1;
        end_block[last_run] = blocks_in_last_run;
        if (blocks_in_last_run == 1) {
            end_pos[last_run] = start_pos[last_run] + tuples_in_last_block;
        }

        auto output_block = reinterpret_cast<std::array<uint64_t, N>*>(
            buffer + (total_runs*block_size));

        auto output_block_curr = 0;

        uint32_t leaf_current_block = 0;
        uint32_t leaf_last_block = division_round_up(total_tuples, BPTLeafWriter<N>::max_records);
        // Merge runs
        while (!queue.empty()) {
            const auto pair = queue.top();
            queue.pop();
            if (output_block_curr == BPTLeafWriter<N>::max_records) {
                // skip first leaf
                if (leaf_current_block > 0) {
                    dir_writer.bulk_insert(output_block, 0, leaf_current_block);
                }
                ++leaf_current_block;
                uint32_t next_bpt_block = leaf_current_block < leaf_last_block ? leaf_current_block : 0;
                leaf_writer.process_block((char*)output_block,
                                          leaf_writer.max_records,
                                          next_bpt_block);
                output_block_curr = 0;
            }
            stat_processor.process_tuple(pair.first);

            output_block[output_block_curr] = pair.first;
            output_block_curr++;

            const auto min_run = pair.second;
            current_pos[min_run]++;

            if (current_pos[min_run] == end_pos[min_run]) {
                current_pos[min_run] = start_pos[min_run];
                current_block[min_run]++;

                if (current_block[min_run] == end_block[min_run]) {
                    continue; // to skip the queue.push
                } else {
                    // offset of the run: (buffer_size * min_run)
                    // offset of the current block: (current_block[min_run] * block_size)
                    file.seekg((buffer_size * min_run) + (current_block[min_run] * block_size), file.beg);
                    file.read(reinterpret_cast<char*>(start_pos[min_run]), block_size);
                    current_pos[min_run] = start_pos[min_run];

                    // update end_pos of last block ultimo bloque si se pasa
                    if (min_run == last_run && current_block[min_run] == blocks_in_last_run - 1) {
                        end_pos[min_run] = start_pos[min_run] + tuples_in_last_block;
                    }
                    file.clear();
                }
            }

            queue.push(std::make_pair(*current_pos[min_run],
                                      min_run));
        }
        // write remaining output_block
        if (output_block_curr != 0) {
            // skip first leaf
            if (leaf_current_block > 0) {
                dir_writer.bulk_insert(output_block, 0, leaf_current_block);
            }
            leaf_writer.process_block((char*)output_block, output_block_curr, 0);
        }

        // delete aux arrays
        delete[](start_pos);
        delete[](end_pos);
        delete[](current_pos);
        delete[](current_block);
        delete[](end_block);
        return;
    }


    void reorder_cols(std::array<size_t, N>&& new_permutation) {
        file.seekg(0, file.beg);

        while (true) {
            auto saved_pos = file.tellg();
            file.read(buffer, buffer_size);
            size_t read_size = file.gcount();

            reorder_chunk(read_size, new_permutation);
            auto beg_ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer);
            auto end_ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer + read_size);
            std::sort(beg_ptr, end_ptr);

            if (file.eof() || file.fail()) {
                file.clear();
                file.seekg(saved_pos, file.beg);
                file.write(buffer, read_size);
                break;
            } else {
                file.seekg(saved_pos, file.beg);
                file.write(buffer, read_size);
            }
        }
        current_permutation = std::move(new_permutation);
    }

    void reorder_chunk(size_t read_size,
                       const std::array<size_t, N>& new_permutation)
    {
        static_assert(N == 1 || N == 2 || N == 3 || N == 4, "Unsuported N");
        assert(current_permutation.size() == new_permutation.size());
        auto end_ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer + read_size);

        if (N == 1) return;

        if (N == 2) {
            if (current_permutation[0] == new_permutation[0]) {
                assert(current_permutation[1] == new_permutation[1]);
            }
            else {
                for (auto key_ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer);
                     key_ptr < end_ptr;
                     ++key_ptr)
                {
                    std::swap((*key_ptr)[0], (*key_ptr)[1]);
                }
            }
        }

        if (N == 3) {
            if (current_permutation[0] == new_permutation[0]
             && current_permutation[1] == new_permutation[1]
             && current_permutation[2] == new_permutation[2])
            {
                // do nothing
            }
            else if (new_permutation[0] == current_permutation[1]
                  && new_permutation[1] == current_permutation[2]
                  && new_permutation[2] == current_permutation[0])
            {
                for (auto key_ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer);
                     key_ptr < end_ptr;
                     ++key_ptr)
                {
                    auto aux      = (*key_ptr)[0];
                    (*key_ptr)[0] = (*key_ptr)[1];
                    (*key_ptr)[1] = (*key_ptr)[2];
                    (*key_ptr)[2] = aux;
                }
            }
            else if (new_permutation[0] == current_permutation[1]
                  && new_permutation[1] == current_permutation[0]
                  && new_permutation[2] == current_permutation[2])
            {
                for (auto key_ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer);
                     key_ptr < end_ptr;
                     ++key_ptr)
                {
                    auto aux      = (*key_ptr)[0];
                    (*key_ptr)[0] = (*key_ptr)[1];
                    (*key_ptr)[1] = aux;
                }
            }
            else if (new_permutation[0] == current_permutation[2]
                  && new_permutation[1] == current_permutation[1]
                  && new_permutation[2] == current_permutation[0])
            {
                for (auto key_ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer);
                     key_ptr < end_ptr;
                     ++key_ptr)
                {
                    auto aux      = (*key_ptr)[0];
                    (*key_ptr)[0] = (*key_ptr)[2];
                    (*key_ptr)[2] = aux;
                }
            }
            else {
                throw std::invalid_argument("Unsuported permutation");
            }
        }

        if (N == 4) {
            if (current_permutation[0] == new_permutation[0]
             && current_permutation[1] == new_permutation[1]
             && current_permutation[2] == new_permutation[2]
             && current_permutation[3] == new_permutation[3])
            {
                // do nothing
            }
            else if (new_permutation[0] == current_permutation[1]
                  && new_permutation[1] == current_permutation[2]
                  && new_permutation[2] == current_permutation[0]
                  && new_permutation[3] == current_permutation[3])
            {
                for (auto key_ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer);
                     key_ptr < end_ptr;
                     ++key_ptr)
                {
                    auto aux      = (*key_ptr)[0];
                    (*key_ptr)[0] = (*key_ptr)[1];
                    (*key_ptr)[1] = (*key_ptr)[2];
                    (*key_ptr)[2] = aux;
                }
            }
            else if (new_permutation[0] == current_permutation[0]
                  && new_permutation[1] == current_permutation[2]
                  && new_permutation[2] == current_permutation[1]
                  && new_permutation[3] == current_permutation[3])
            {
                for (auto key_ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer);
                     key_ptr < end_ptr;
                     ++key_ptr)
                {
                    auto aux      = (*key_ptr)[1];
                    (*key_ptr)[1] = (*key_ptr)[2];
                    (*key_ptr)[2] = aux;
                }
            }
            else {
                throw std::invalid_argument("Unsuported permutation");
            }
        }
    }

    std::fstream file;
    std::string filename;

    std::array<size_t, N> current_permutation;

    size_t buffer_size;
    size_t buffer_count;

    size_t current_tuple_iter;
    size_t iter_buffer_offset;

    char* buffer;
};
} // namespace Import
