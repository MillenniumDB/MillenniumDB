#include "tuple_collection.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include "base/exceptions.h"
#include "storage/buffer_manager.h"
#include "storage/file_id.h"
#include "storage/file_manager.h"

using namespace std;

TupleCollection::TupleCollection(Page&                            page,
                                 const map<VarId, uint_fast32_t>& saved_vars,
                                 const vector<VarId>&             order_vars,
                                 const vector<bool>&              ascending) :
    page        (page),
    saved_vars  (saved_vars),
    order_vars  (order_vars),
    ascending   (ascending),
    tuples      (reinterpret_cast<GraphObject*>(page.get_bytes())),
    tuple_count (reinterpret_cast<uint64_t*>(page.get_bytes() + Page::MDB_PAGE_SIZE - sizeof(uint64_t)))
    { }


TupleCollection::~TupleCollection() {
    page.make_dirty();
    buffer_manager.unpin(page);
}


void TupleCollection::add(std::vector<GraphObject> new_tuple) {
    // Add a new tuple in the last position of the page
    const size_t bytes_used = (*tuple_count) * saved_vars.size();
    for (size_t i = 0; i < saved_vars.size(); i++) {
        tuples[bytes_used + i] = new_tuple[i];
    }
  	(*tuple_count)++;
}


std::vector<GraphObject> TupleCollection::get(uint64_t id) const {
    // Return the n-th tuple of the page
    std::vector<GraphObject> res(saved_vars.size());
    size_t tuple_position = id * saved_vars.size();
    for (size_t i = 0; i < saved_vars.size(); i++) {
        res[i] = tuples[tuple_position + i];
    }
    return res;
}


void TupleCollection::override_tuple(const std::vector<GraphObject>& tuple, int position) {
    // Write a tuple in the specific position. tuple_count don't increase because
    // this method assume that is overriding a tuple
    const auto position_to_override = position * saved_vars.size();
    for (size_t i = 0; i < saved_vars.size(); i++) {
        tuples[position_to_override + i] = tuple[i];
    }
}


void TupleCollection::reset() {
    // Causes all tuples on the page will be ignored
    (*tuple_count) = 0;
}


void TupleCollection::swap(int x, int y) {
    // Override tuple in position x in position y
    // and tuple in position y in position x
    auto x_tuple = get(x);
    auto y_tuple = get(y);
    override_tuple(x_tuple, y);
    override_tuple(y_tuple, x);
}


void TupleCollection::sort() {
    // Sort all the tuples. order_vars the position of the vars in each array that
    // define the order.
    quicksort(0, (*tuple_count) - 1);

    // Insertion sort:
    // for (size_t i = 0; i < *tuple_count - 1; i++) {
    //     for (size_t j = i + 1; j < *tuple_count; j++) {
    //         if (TupleCollection::has_priority(get(j), get(i))) {
    //             swap(i,j);
    //         }
    //     }
    // }
}


bool TupleCollection::has_priority(const std::vector<GraphObject>& lhs,
                                   const std::vector<GraphObject>& rhs)
{
    assert(order_vars.size() == ascending.size());
    for (size_t i = 0; i < order_vars.size(); i++) {
        uint_fast32_t index;
        auto search = saved_vars.find(order_vars[i]);
        if (search != saved_vars.end()) {
            index = search->second;
        } else {
            throw std::logic_error("saved_vars must contain VarId(" + std::to_string(order_vars[i].id) + ")");
        }

        auto left_value  = lhs[index];
        auto right_value = rhs[index];

        if (left_value < right_value) {
            return ascending[i];
        } else if (right_value < left_value) {
            return !ascending[i];
        }
    }
    return true;
}


void TupleCollection::quicksort(int i, int f) {
    if (i < f) {
        int p = partition(i, f);
        quicksort(i, p - 1);
        quicksort(p + 1, f);
    }
}


int TupleCollection::partition(int i, int f) {
    int x = i + (rand() % (f - i + 1));
    auto p = get(x);
    swap(x, f);
    int low_el = i - 1;
    for (int j = i; j <= f - 1; j++) {
        if (has_priority(get(j), p)) {
            low_el++;
            swap(low_el, j);
        }
    }
    swap(low_el + 1, f);
    return low_el + 1;
}


MergeOrderedTupleCollection::MergeOrderedTupleCollection(const map<VarId, uint_fast32_t>& saved_vars,
                                                         const vector<VarId>&             order_vars,
                                                         const vector<bool>&              ascending,
                                                         bool*                            interruption_requested) :
    saved_vars             (saved_vars),
    order_vars             (order_vars),
    ascending              (ascending),
    interruption_requested (interruption_requested) { }


void MergeOrderedTupleCollection::merge(uint64_t left_start,
                                        uint64_t left_end,
                                        uint64_t right_start,
                                        uint64_t right_end,
                                        TmpFileId source_file_id,
                                        TmpFileId output_file_id)
{
    // Merge [left_start-left_end] with [right_start-right_end] from source_file_id in output_file_id,
    // in output_file_id is sorted
    auto left_run  = get_run(buffer_manager.get_tmp_page(source_file_id, left_start));
    auto right_run = get_run(buffer_manager.get_tmp_page(source_file_id, right_start));
    auto out_run   = get_run(buffer_manager.get_tmp_page(output_file_id, left_start));
    out_run->reset();

    auto left_tuple  = left_run->get(0);
    auto right_tuple = right_run->get(0);
    uint64_t left_counter = 0;
    uint64_t right_counter = 0;
    uint64_t out_page_counter = left_start;
    bool left_first;
    bool open_left = true;
    bool open_right = true;

    while (open_left || open_right) {
        if (out_run->is_full()) {
            if (__builtin_expect(!!(*interruption_requested), 0)) {
                throw InterruptedException();
            }
            out_page_counter++;
            out_run = get_run(buffer_manager.get_tmp_page(output_file_id, out_page_counter));
            out_run->reset();
        }
        left_first = left_run->has_priority(left_tuple, right_tuple);
        if (open_left && (left_first || !open_right)) {
            out_run->add(left_tuple);
            left_counter++;
            if (left_counter == left_run->get_tuple_count()) {
                left_start++;
                if (left_start <= left_end) {
                    left_run = get_run(buffer_manager.get_tmp_page(source_file_id, left_start));
                    left_counter = 0;
                } else {
                    open_left = false;
                    continue;
                }
            }
            left_tuple = left_run->get(left_counter);
        }
        else if (open_right && (!left_first || !open_left)) {
            out_run->add(right_tuple);
            right_counter++;
            if (right_counter == right_run->get_tuple_count()) {
                right_start++;
                if (right_start <= right_end) {
                    right_run = get_run(buffer_manager.get_tmp_page(source_file_id, right_start));
                    right_counter = 0;
                } else {
                    open_right = false;
                    continue;
                }
            }
            right_tuple = right_run->get(right_counter);
        }
    }
}


void MergeOrderedTupleCollection::copy_page(uint64_t source_page,
                                            TmpFileId source_file_id,
                                            TmpFileId output_file_id)
{
    auto source_tuples = get_run(buffer_manager.get_tmp_page(source_file_id, source_page));
    auto output_tuples = get_run(buffer_manager.get_tmp_page(output_file_id, source_page));
    output_tuples->reset();
    for (size_t i = 0; i < source_tuples->get_tuple_count(); i++) {
        auto t = source_tuples->get(i);
        output_tuples->add(t);
    }
    source_tuples->reset();
}


std::unique_ptr<TupleCollection> MergeOrderedTupleCollection::get_run(Page& run_page) {
    return make_unique<TupleCollection>(run_page, saved_vars, order_vars, ascending);
}
