#include "ordered_file.h"

#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <queue>
#include <thread>

#include "storage/index/record.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/index/record.h"

template class OrderedFile<1>;
template class OrderedFile<2>;
template class OrderedFile<3>;
template class OrderedFile<4>;

using namespace std;

template <std::size_t N>
OrderedFile<N>::OrderedFile(const string& filename) :
    file_id_a   (file_manager.get_file_id(filename + ".a.tmp")),
    file_id_b   (file_manager.get_file_id(filename + ".b.tmp"))
{
    current_page = 0;
    current_pos_in_current_page = 0;

    last_page = 0;
    last_pos_in_last_page = 0;

    file_id        = &file_id_a;
    helper_file_id = &file_id_b;
    io_buffer = make_unique<OrderedFilePage<N>>(buffer_manager.get_page(*file_id, current_page));
}


template <std::size_t N>
OrderedFile<N>::~OrderedFile() {
    io_buffer.reset();
    file_manager.remove(file_id_a);
    file_manager.remove(file_id_b);
}

template <std::size_t N>
void OrderedFile<N>::begin_read() noexcept {
    current_page = 0;
    current_pos_in_current_page = 0;
    io_buffer = make_unique<OrderedFilePage<N>>(buffer_manager.get_page(*file_id, current_page));
}


template <std::size_t N>
void OrderedFile<N>::copy_to_bpt_leaf(BPlusTreeLeaf<N>& leaf, uint_fast32_t page_number) const noexcept {
    assert(page_number <= last_page);

    OrderedFilePage<N> ordered_file_page(buffer_manager.get_page(*file_id, page_number));
    // copy the entire page, but `leaf.next_leaf` needs to be setted outside
    std::memcpy(
        leaf.get_page().get_bytes(),
        ordered_file_page.get_page().get_bytes(),
        Page::MDB_PAGE_SIZE
    );
}


template <std::size_t N>
std::unique_ptr<Record<N>> OrderedFile<N>::next_record() noexcept {
    if (current_page <= last_page) {
        auto arr = io_buffer->get(current_pos_in_current_page);
        ++current_pos_in_current_page;
        if (current_pos_in_current_page >= io_buffer->get_size()) {
            ++current_page;
            current_pos_in_current_page = 0;
            // not necesary to make io_buffer dirty
            if (current_page <= last_page) {
                io_buffer = make_unique<OrderedFilePage<N>>(buffer_manager.get_page(*file_id, current_page));
            }
        }
        return make_unique<Record<N>>(arr);
    } else {
        return nullptr;
    }
}


template <std::size_t N>
void OrderedFile<N>::append_record(const std::array<uint64_t, N>& record) noexcept {
    assert(io_buffer != nullptr);
    if (io_buffer->is_full()) {
        ++last_page;
        io_buffer = make_unique<OrderedFilePage<N>>(buffer_manager.get_page(*file_id, last_page));
        io_buffer->append_record(record);
        last_pos_in_last_page = 1;
    } else {
        io_buffer->append_record(record);
        ++last_pos_in_last_page;
    }
}


template <std::size_t N>
struct RunRecordComparator {
    constexpr bool operator()(const pair<std::array<uint64_t, N>, uint_fast32_t>& lhs,
                              const pair<std::array<uint64_t, N>, uint_fast32_t>& rhs) const
    {
        return lhs.first > rhs.first;
    }
};


constexpr uint_fast32_t division_round_up(uint_fast32_t a, uint_fast32_t b) {
    return (a / b) + (a % b != 0);
}


template <std::size_t N>
void OrderedFile<N>::order(const std::array<uint_fast8_t, N>& column_order) noexcept {
    bool need_column_reorder = false;
    for (size_t n = 0; n < N; n++) {
        if (column_order[n] != n) {
            need_column_reorder = true;
        }
    }

    const uint32_t MAX_THREADS = 8; // TODO: extract to config file
    const auto total_pages = last_page + 1;

    // First step: order each page
    // TODO: bulk loading pages could improve performance
    {
        const auto number_of_threads = MAX_THREADS < total_pages ? MAX_THREADS : total_pages;
        std::vector<thread> threads;

        const auto pages_per_thread = division_round_up(total_pages, number_of_threads);

        if (need_column_reorder) {
            for (uint_fast32_t start_page = 0; start_page <= last_page; start_page += pages_per_thread) {
                auto end_page = (start_page + pages_per_thread - 1) > last_page
                                ? last_page
                                : (start_page + pages_per_thread - 1);

                threads.push_back( thread(
                    [this, start_page, end_page, &column_order] {
                        for (auto p = start_page; p <= end_page; p++) {
                            OrderedFilePage<N> ordered_file_page(buffer_manager.get_page(*file_id, p));
                            ordered_file_page.reorder_columns(column_order);
                            ordered_file_page.order();
                        }
                    }
                ) );
            }
        } else { // this is the same but without calling reorder_columns
            for (uint_fast32_t start_page = 0; start_page <= last_page; start_page += pages_per_thread) {
                auto end_page = (start_page + pages_per_thread - 1) > last_page
                                ? last_page
                                : (start_page + pages_per_thread - 1);

                threads.push_back( thread(
                    [this, start_page, end_page] {
                        for (auto p = start_page; p <= end_page; p++) {
                            OrderedFilePage<N> ordered_file_page(buffer_manager.get_page(*file_id, p));
                            ordered_file_page.order();
                        }
                    }
                ) );
            }
        }
        for (auto& thread : threads) {
            thread.join();
        }
    }

    auto pool_size = buffer_manager.get_shared_buffer_pool_size() / (2*MAX_THREADS);
    unsigned int max_runs;

    for (unsigned r = 2; true; r++) {
        double root = ceil( pow(last_page+1, 1.0/r) );
        if (root < pool_size) {
            max_runs = root;
            break;
        }
    }

    // Second step: iterative merge sort
    // cout << "total pages: " << total_pages << "\n";
    for (uint_fast32_t run_length = 1; run_length < total_pages; run_length *= max_runs) {
        std::vector<thread> threads;
        uint_fast32_t start_page = 0;

        const auto merges_needed         = division_round_up(total_pages, run_length*max_runs);
        const auto max_merges_per_thread = division_round_up(merges_needed, MAX_THREADS);
        const auto number_of_threads     = division_round_up(merges_needed, max_merges_per_thread);

        // cout << "run_length " << run_length << "\n";
        // cout << "max_runs " << max_runs << "\n";
        // cout << "merges_needed " << merges_needed << "\n";
        // cout << "number_of_threads " << number_of_threads << "\n";
        // cout << "max_merges_per_thread " << max_merges_per_thread << "\n";

        for (uint_fast32_t t = 0; t < number_of_threads; ++t) {
            auto end_page = start_page + (run_length*max_runs)*max_merges_per_thread - 1;
            if (end_page > last_page) {
                end_page = last_page;
            }

            threads.push_back( thread(
                [this, start_page, end_page, run_length, max_runs] {
                    // cout << "sorting from " << start_page << " to " << end_page << "\n";
                    auto current_output_page = start_page;
                    auto output_file_page = make_unique<OrderedFilePage<N>>(buffer_manager.get_page(*helper_file_id, current_output_page));
                    output_file_page->clear();

                    auto merge_buffer = new std::unique_ptr<OrderedFilePage<N>>[max_runs];
                    uint_fast32_t* buffer_current_pos  = new uint_fast32_t[max_runs];
                    uint_fast32_t* buffer_current_page = new uint_fast32_t[max_runs];

                    for (auto p = start_page; p <= end_page; p += run_length*max_runs) {
                        uint_fast32_t runs =   (p + (run_length*max_runs) <= end_page)
                                             ? max_runs
                                             : ((end_page+1-p)/run_length + ((end_page+1-p)%run_length != 0));

                        std::priority_queue<
                            pair<std::array<uint64_t, N>, uint_fast32_t>,          // tuple: (record, run_number)
                            vector< pair<std::array<uint64_t, N>, uint_fast32_t>>, // container
                            RunRecordComparator<N>                                 // comparator
                        > queue;


                        // Fill buffers
                        for (uint_fast32_t run = 0; run < runs; ++run) {
                            buffer_current_pos[run] = 0;
                            buffer_current_page[run] = 0;
                            uint_fast32_t page_number = p + (run*run_length);
                            merge_buffer[run] = make_unique<OrderedFilePage<N>>(buffer_manager.get_page(*file_id, page_number));
                            queue.push(make_pair(merge_buffer[run]->get(0), run));
                        }
                        // Merge runs
                        while (!queue.empty()) {
                            if (output_file_page->is_full()) {
                                ++current_output_page;
                                output_file_page = make_unique<OrderedFilePage<N>>(buffer_manager.get_page(*helper_file_id,
                                                                                   current_output_page));
                                output_file_page->clear();
                            }
                            const auto pair = queue.top();
                            queue.pop();
                            const auto min_run = pair.second;
                            output_file_page->append_record( pair.first );

                            buffer_current_pos[min_run]++;
                            if (buffer_current_pos[min_run] >= merge_buffer[min_run]->get_size()) {
                                buffer_current_pos[min_run] = 0;
                                buffer_current_page[min_run]++;
                                if (buffer_current_page[min_run] >= run_length
                                    || (p + (min_run*run_length) + buffer_current_page[min_run]) > last_page)
                                {
                                    continue; // to skip the queue.push
                                } else {
                                    merge_buffer[min_run] = make_unique<OrderedFilePage<N>>(
                                        buffer_manager.get_page(*file_id,
                                                                p + (min_run*run_length) + buffer_current_page[min_run])
                                    );
                                }
                            }

                            queue.push( make_pair(
                                merge_buffer[min_run]->get(buffer_current_pos[min_run]),
                                min_run
                            ) );

                        }
                    }
                    delete[](merge_buffer);
                    delete[](buffer_current_pos);
                    delete[](buffer_current_page);
                }
            ) );
            start_page = end_page + 1;
        }
        for (auto& thread : threads) {
            thread.join();
        }
        // Ended one merge iteration, swap files
        std::swap(file_id, helper_file_id);
    }
}


template <std::size_t N>
bool OrderedFile<N>::check_order() {
    begin_read();

    auto a = next_record();
    if (a == nullptr)  {
        return true;
    }
    auto b = next_record();

    int errors = 0;
    for (int i = 0; b != nullptr; i++, b=next_record() ) {
        if (!(*a < *b)) {
            cerr << "Bad ordering at tuples " << i << " and " << i+1 << "\n";
            for (size_t n = 0; n < N; n++) {
                cerr << "\t" << a->ids[n];
            }
            cerr << "\n";
            for (size_t n = 0; n < N; n++) {
                cerr << "\t" << b->ids[n];
            }
            cerr << "\n";
            cerr << "\tdiff: ";
            for (size_t n = 0; n < N; n++) {
                cerr << "\t" << a->ids[n] - b->ids[n];
            }
            cerr << "\n";

            errors++;
            if (errors >= 260) {
                cerr << "too much errors, not showing them all\n";
                return false;
            }
        }
        a = move(b);
    }
    return errors == 0;
}
