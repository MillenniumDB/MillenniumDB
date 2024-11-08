#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <stdexcept>
#include <vector>

#include "import/stats_processor.h"
#include "macros/aligned_alloc.h"
#include "storage/page/versioned_page.h"
#include "storage/index/bplus_tree/bpt_mem_import.h"

namespace Import {
template <std::size_t N>
struct RunRecordComparator {
    constexpr bool operator()(const std::pair<std::array<uint64_t, N>, uint64_t>& lhs,
                              const std::pair<std::array<uint64_t, N>, uint64_t>& rhs) const
    {
        return lhs.first > rhs.first;
    }
};

template <std::size_t N>
class DiskVector {
public:
    DiskVector(const std::string& filename) :
        filename (filename),
        buffer_size (VPage::SIZE*N*sizeof(uint64_t)),
        buffer (reinterpret_cast<char*>(MDB_ALIGNED_ALLOC(VPage::SIZE, buffer_size)))
    {
        file.open(filename, std::ios::out|std::ios::app);
        if (file.fail()) {
            throw std::runtime_error("Could not open file " + filename);
        }
        file.close();
        file.open(filename, std::ios::in|std::ios::out|std::ios::binary);

        compression_buffer = new char[VPage::SIZE*2];
    }

    ~DiskVector() {
        delete[] compression_buffer;
    }

    // returns the tuple count
    void create_bpt(const std::string& base_name,
                    std::array<uint64_t, N>&& new_permutation,
                    StatsProcessor<N>& stat_processor)
    {
        reorder_cols(std::move(new_permutation));
        merge_sort(base_name, stat_processor);
    }

    void finish_appends() {
        file.write(buffer, buffer_append_current_pos*N*sizeof(uint64_t));
        uint64_t file_length = file.tellg();
        total_tuples = file_length / (N*sizeof(uint64_t));
    }

    void start_indexing(char* new_buffer,
                        uint64_t max_buffer_size,
                        std::array<uint64_t, N> original_permutation)
    {
        // free append buffer
        MDB_ALIGNED_FREE(buffer);
        current_permutation = original_permutation;
        // buffer_size must be divisible by (VPage::SIZE*N*sizeof(uint64_t)
        auto max_blocks_per_run = max_buffer_size / (VPage::SIZE*N*sizeof(uint64_t));
        auto new_buffer_size = max_blocks_per_run * (VPage::SIZE*N*sizeof(uint64_t));
        this->buffer_size = new_buffer_size;
        buffer = new_buffer;
    }

    void finish_indexing() {
        file.close();
        std::remove(filename.c_str());
    }

    // If start_indexing() was not called, need to call this at the end
    void skip_indexing() {
        file.close();
        std::remove(filename.c_str());
        free(buffer);
    }

    void push_back(const std::array<uint64_t, N>&& record) {
        std::memcpy(&buffer[buffer_append_current_pos*N*sizeof(uint64_t)],
                    &record[0],
                    N * sizeof(uint64_t));
        buffer_append_current_pos++;
        if (buffer_append_current_pos == VPage::SIZE) {
            file.write(buffer, buffer_size);
            buffer_append_current_pos = 0;
        }
    }

    // This number may have duplicates
    inline uint64_t get_total_tuples() const {
        return total_tuples;
    }
    void begin_tuple_iter() {
        file.seekg(0, file.beg);
        iter_current_tuple = 0;
        // set iter_buffer_offset so its 0 when next_tuple() is called the first time
        iter_buffer_offset = VPage::SIZE - 1;
    }

    bool has_next_tuple() {
        return iter_current_tuple < total_tuples;
    }

    std::array<uint64_t, N>& next_tuple() {
        // max tuples in buffer is VPage::SIZE
        iter_buffer_offset = (iter_buffer_offset+1) % VPage::SIZE;
        iter_current_tuple++;

        if (iter_buffer_offset == 0) {
            // leer del disco a buffer
            file.read(buffer, VPage::SIZE*N*sizeof(uint64_t));
            file.clear();
        }
        auto ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer) + iter_buffer_offset;
        return *ptr;
    }

private:
    constexpr uint64_t division_round_up(uint64_t a, uint64_t b) {
        return (a / b) + (a % b != 0);
    }

    uint32_t get_page_size(std::bitset<N * 8> bitset, uint64_t leaf_records) {
        return 2 * sizeof(uint32_t) + N + bitset.count() + leaf_records * (sizeof(uint64_t) * N - bitset.count());
    }

    void merge_sort(const std::string& base_name, StatsProcessor<N>& stat_processor) {
        BPTLeafWriter<N> leaf_writer(base_name + ".leaf");
        BPTDirWriter<N> dir_writer(base_name + ".dir");

        if (total_tuples == 0) {
            leaf_writer.make_empty();
            return;
        }

        file.seekg(0, file.end);
        uint64_t file_length = file.tellg();
        file.seekg(0, file.beg);

        if (buffer_size >= file_length) {
            // we already have everything in buffer from reorder_cols
            auto ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer);
            uint32_t leaf_current_block = 0;
            uint64_t current_tuple = 0;
            uint64_t valid_tuples = total_tuples - repeated_tuples;

            std::bitset<N * 8> bitset_tmp;
            std::bitset<N * 8> bitset;
            uint32_t leaf_tuples = 0;

            while (current_tuple < valid_tuples) {
                // skip first leaf from going into B+tree directory
                if (current_tuple != 0) {
                    dir_writer.bulk_insert(ptr, 0, leaf_current_block);
                }

                char* first_record_ptr = (char*) ptr;
                bitset_tmp.set();
                bitset.set();

                while (current_tuple + leaf_tuples < valid_tuples) {
                    char* record_ptr = (char*) (ptr + leaf_tuples);

                    for (size_t i = 0; i < sizeof(uint64_t) * N; ++i) {
                        if (bitset_tmp[i] && record_ptr[i] != first_record_ptr[i]) {
                            bitset_tmp.set(i, 0);
                        }
                    }

                    if (get_page_size(bitset_tmp, leaf_tuples + 1) > VPage::SIZE) {
                        break;
                    }

                    bitset = bitset_tmp;
                    leaf_tuples++;
                }

                write_to_buffer(ptr, bitset, leaf_tuples);

                uint64_t tuples_written;
                if (current_tuple + leaf_tuples < valid_tuples) {
                    // this leaf is full and there are more leaves
                    tuples_written = leaf_tuples;
                    leaf_writer.process_block(compression_buffer,
                                              tuples_written,
                                              bitset,
                                              ++leaf_current_block);
                } else {
                    // last leaf
                    tuples_written = leaf_tuples;
                    leaf_writer.process_block(compression_buffer,
                                              tuples_written,
                                              bitset,
                                              0);
                }


                for (uint64_t i = 0; i < tuples_written; i++) {
                    stat_processor.process_tuple(*(ptr + i));
                }
                // Its better to sum a compile-time constant than a variable (tuples_written)
                // doesn't matter when we overflow
                leaf_tuples = 0;
                current_tuple += tuples_written;
                ptr += tuples_written;
            }

            return;
        }

        // A run is a set of ordered tuples (ordered when method reorder_cols was called)
        // A run is divided in blocks. Tuples from disk are read 1 block at a time
        // buffer_size was chosen to be a multiple of block_size
        constexpr uint64_t block_size = VPage::SIZE*N*sizeof(uint64_t);

        const uint64_t total_runs = division_round_up(file_length, buffer_size);
        const uint64_t total_blocks = division_round_up(file_length, block_size);

        const uint64_t max_tuples_per_block = VPage::SIZE;
        const uint64_t max_blocks_per_run = buffer_size / block_size;

        const uint64_t tuples_in_last_block = (total_tuples % max_tuples_per_block == 0)
                                            ? max_tuples_per_block
                                            : (total_tuples % max_tuples_per_block);

        const uint64_t blocks_in_last_run = (total_blocks % max_blocks_per_run == 0)
                                            ? max_blocks_per_run
                                            : (total_blocks % max_blocks_per_run);


        // throw exception if we can't order with 1 merge
        if (total_runs*block_size + (BPTLeafWriter<N>::max_records*N*sizeof(uint64_t)) > buffer_size) {
            throw std::logic_error("Can't order tuples with one merge, need a bigger buffer size.");
        }

        // When we have only one run we skip the merge
        std::priority_queue<std::pair<std::array<uint64_t, N>, uint64_t>,               // tuple: (record, run_number)
                            std::vector< std::pair<std::array<uint64_t, N>, uint64_t>>, // container
                            RunRecordComparator<N>                                      // comparator
                            > queue;

        // Fill buffers and put the first tuple of the run in the heap
        std::array<uint64_t, N>** start_pos   = new std::array<uint64_t, N>*[total_runs];
        std::array<uint64_t, N>** end_pos     = new std::array<uint64_t, N>*[total_runs];
        std::array<uint64_t, N>** current_pos = new std::array<uint64_t, N>*[total_runs];

        uint64_t* current_block = new uint64_t[total_runs];
        uint64_t* end_block     = new uint64_t[total_runs];

        for (uint64_t run = 0; run < total_runs-1; ++run) {
            start_pos[run] = reinterpret_cast<std::array<uint64_t, N>*>(buffer + (run*block_size));
            end_pos[run]   = reinterpret_cast<std::array<uint64_t, N>*>(buffer + ((run+1)*block_size));
            current_pos[run] = start_pos[run];
            current_block[run] = 0;
            end_block[run] = max_blocks_per_run;

            file.seekg(buffer_size * run, file.beg);
            file.read(reinterpret_cast<char*>(start_pos[run]), block_size);
            queue.push(std::make_pair(*current_pos[run], run));
        }
        const auto last_run = total_runs - 1;
        { // last run case
            start_pos[last_run] = reinterpret_cast<std::array<uint64_t, N>*>(buffer + (last_run*block_size));
            if (blocks_in_last_run == 1) {
                // the last block of the last run may be not full
                end_pos[last_run] = start_pos[last_run] + tuples_in_last_block;
            } else {
                end_pos[last_run] = reinterpret_cast<std::array<uint64_t, N>*>(buffer + ((last_run+1)*block_size));
            }
            current_pos[last_run] = start_pos[last_run];
            current_block[last_run] = 0;
            end_block[last_run] = blocks_in_last_run;

            file.seekg(buffer_size * last_run, file.beg);
            file.read(reinterpret_cast<char*>(start_pos[last_run]), block_size);
            queue.push(std::make_pair(*current_pos[last_run], last_run));
        }
        if (!file.good()) {
            std::cout << "WARNING: file not good\n";
            file.clear();
        }

        // the output block is the last block
        auto output_block = reinterpret_cast<std::array<uint64_t, N>*>(buffer + (total_runs*block_size));
        auto output_block_curr = 0;

        uint32_t leaf_current_block = 0;
        const uint32_t leaf_last_block = division_round_up(total_tuples, BPTLeafWriter<N>::max_records);
        // set last_seen_record as an impossible record
        std::array<uint64_t, N> last_seen_record; // used to remove duplicates
        for (uint64_t i = 0; i < N; i++) { last_seen_record[i] = UINT64_MAX; }

        std::bitset<N * 8> bitset_tmp;
        std::bitset<N * 8> bitset;
        bitset_tmp.set();
        bitset.set();

        std::array<uint64_t, N> first_record = queue.top().first;
        char* first_record_ptr = (char*) &first_record;

        // Merge runs
        while (!queue.empty()) {
            const auto queue_top_pair = queue.top();
            queue.pop();

            // add tuple to output only if distinct
            if (queue_top_pair.first != last_seen_record) {
                last_seen_record = queue_top_pair.first;
                stat_processor.process_tuple(last_seen_record);
                char* record_ptr = (char*) &last_seen_record;

                // update the bitset
                for (size_t i = 0; i < sizeof(uint64_t) * N; ++i) {
                    if (bitset_tmp[i] && record_ptr[i] != first_record_ptr[i]) {
                        bitset_tmp.set(i, 0);
                    }
                }

                output_block[output_block_curr] = queue_top_pair.first;
                output_block_curr++;
            }

            if (get_page_size(bitset_tmp, output_block_curr) > VPage::SIZE) {
                // skip first leaf
                if (leaf_current_block > 0) {
                    dir_writer.bulk_insert(output_block, 0, leaf_current_block);
                }

                write_to_buffer(output_block, bitset, output_block_curr - 1);

                ++leaf_current_block;
                uint32_t next_bpt_block = leaf_current_block < leaf_last_block ? leaf_current_block : 0;
                leaf_writer.process_block(compression_buffer,
                                          output_block_curr - 1,
                                          bitset,
                                          next_bpt_block);


                output_block[0] = last_seen_record;
                output_block_curr = 1;
                first_record = last_seen_record;
                first_record_ptr = (char*) &first_record;
                bitset_tmp.set();
                bitset.set();
            }

            bitset = bitset_tmp;

            const auto min_run = queue_top_pair.second;
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

                    // update end_pos of the last block of the last run, it may be not full
                    if (min_run == last_run && current_block[min_run] == blocks_in_last_run - 1) {
                        end_pos[min_run] = start_pos[min_run] + tuples_in_last_block;
                    }
                    file.clear();
                }
            }

            queue.push(std::make_pair(*current_pos[min_run], min_run));
        }
        // write remaining output_block
        if (output_block_curr != 0) {
            // skip first leaf
            if (leaf_current_block > 0) {
                dir_writer.bulk_insert(output_block, 0, leaf_current_block);
            }
            write_to_buffer(output_block, bitset, output_block_curr);
            leaf_writer.process_block(compression_buffer, output_block_curr, bitset, 0);
        }

        // delete aux arrays
        delete[](start_pos);
        delete[](end_pos);
        delete[](current_pos);
        delete[](current_block);
        delete[](end_block);

        return;
    }


    void write_to_buffer(std::array<uint64_t, N>* records_buffer, std::bitset<N * 8> bitset, uint32_t n_records) {
        uint64_t buffer_pos = 0;

        // bitset
        unsigned long bits_ul = bitset.to_ulong();
        memcpy(compression_buffer, &bits_ul, N);
        buffer_pos += N;

        char* first_record_ptr = (char*) records_buffer;

        // redundant bytes
        for (size_t i = 0; i < N * 8; ++i) {
            if (bitset[i]) {
                compression_buffer[buffer_pos] = first_record_ptr[i];
                buffer_pos++;
            }
        }

        // non redundant bytes of each record
        for (uint32_t i = 0; i < n_records; ++i) {
            char* record_ptr = (char*) (records_buffer + i);
            for (size_t j = 0; j < N * 8; ++j) {
                if (!bitset[j]) {
                    compression_buffer[buffer_pos] = record_ptr[j];
                    buffer_pos++;
                }
            }
        }
    }


    void reorder_cols(std::array<uint64_t, N>&& new_permutation) {
        file.seekg(0, file.end);
        const uint64_t file_length = file.tellg();
        // repeated_tuples is 0 when buffer_size < file_length even if there are repeated tuples
        uint64_t remaining = (total_tuples - repeated_tuples) * N * sizeof(uint64_t);
        file.seekg(0, file.beg);

        uint64_t read_pos = 0;
        uint64_t write_pos = 0;

        while (remaining > 0) {
            file.seekg(read_pos, file.beg);
            file.read(buffer, std::min(buffer_size, remaining));
            uint64_t read_size = file.gcount();
            auto write_size = read_size;

            reorder_chunk(read_size, new_permutation);
            auto beg_ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer);
            auto end_ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer + read_size);
            std::sort(beg_ptr, end_ptr);

            // remove duplicates now if total_runs == 1
            // if total_runs > 1 duplicates are removed in merge
            // if repeated_tuples != 0 this was already executed in another permutation
            if (buffer_size >= file_length && repeated_tuples == 0) {
                auto write_ptr = reinterpret_cast<std::array<uint64_t, N>*>(buffer);
                auto read_ptr = write_ptr;

                while (read_ptr != end_ptr) {
                    *write_ptr = *read_ptr;
                    read_ptr++;
                    while (read_ptr != end_ptr && *read_ptr == *write_ptr) {
                        read_ptr++;
                        write_size -= N*sizeof(uint64_t);
                        repeated_tuples++;
                    }
                    write_ptr++;
                }
            }

            file.seekg(write_pos, file.beg);
            file.write(buffer, write_size);

            remaining -= read_size;
            read_pos += read_size;
            write_pos += write_size;
        }
        current_permutation = std::move(new_permutation);
    }

    void reorder_chunk(uint64_t read_size,
                       const std::array<uint64_t, N>& new_permutation)
    {
        static_assert(N == 1 || N == 2 || N == 3 || N == 4, "Unsupported N");
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
                throw std::invalid_argument("Unsupported permutation");
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
                throw std::invalid_argument("Unsupported permutation");
            }
        }
    }

    std::fstream file;
    std::string filename;

    std::array<uint64_t, N> current_permutation;

    uint64_t buffer_size;
    uint64_t buffer_append_current_pos = 0;

    // used in begin_tuple_iter(), has_next_tuple() and next_tuple() methods for iterating
    uint64_t iter_current_tuple;

    // used in begin_tuple_iter(), has_next_tuple() and next_tuple() methods for iterating
    // this number consider the duplicates
    uint64_t total_tuples;

    // used in begin_tuple_iter(), has_next_tuple() and next_tuple() methods for iterating
    uint64_t iter_buffer_offset;

    char* buffer;
    char* compression_buffer;

    // will be set and used only when total_runs = 1
    uint64_t repeated_tuples = 0;
};
} // namespace Import
