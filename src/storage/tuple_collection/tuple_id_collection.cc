#include "tuple_id_collection.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include "query/exceptions.h"
#include "query/query_context.h"
#include "storage/buffer_manager.h"
#include "storage/file_id.h"
#include "storage/file_manager.h"

using namespace std;

TupleIdCollection::TupleIdCollection(
    Page&                            page,
    const map<VarId, uint_fast32_t>& saved_vars,
    const vector<VarId>&             order_vars,
    const vector<bool>&              ascending,
    int64_t(*_compare)(ObjectId, ObjectId)
) :
    page        (page),
    saved_vars  (saved_vars),
    order_vars  (order_vars),
    ascending   (ascending),
    tuples      (reinterpret_cast<ObjectId*>(page.get_bytes())),
    tuple_count (reinterpret_cast<uint64_t*>(page.get_bytes() + Page::MDB_PAGE_SIZE - sizeof(uint64_t))),
    compare     (_compare)
    { }


TupleIdCollection::~TupleIdCollection() {
    page.make_dirty();
    buffer_manager.unpin(page);
}


void TupleIdCollection::add(std::vector<ObjectId> new_tuple) {
    // Add a new tuple in the last position of the page
    const size_t bytes_used = (*tuple_count) * saved_vars.size();
    for (size_t i = 0; i < saved_vars.size(); i++) {
        tuples[bytes_used + i] = new_tuple[i];
    }
  	(*tuple_count)++;
}


std::vector<ObjectId> TupleIdCollection::get(uint64_t id) const {
    // Return the n-th tuple of the page
    std::vector<ObjectId> res(saved_vars.size());
    size_t tuple_position = id * saved_vars.size();
    for (size_t i = 0; i < saved_vars.size(); i++) {
        res[i] = tuples[tuple_position + i];
    }
    return res;
}


void TupleIdCollection::override_tuple(const std::vector<ObjectId>& tuple, int position) {
    // Write a tuple in the specific position. tuple_count don't increase because
    // this method assume that is overriding a tuple
    const auto position_to_override = position * saved_vars.size();
    for (size_t i = 0; i < saved_vars.size(); i++) {
        tuples[position_to_override + i] = tuple[i];
    }
}


void TupleIdCollection::reset() {
    // Causes all tuples on the page will be ignored
    (*tuple_count) = 0;
}


void TupleIdCollection::swap(int x, int y) {
    // Override tuple in position x in position y
    // and tuple in position y in position x
    auto x_tuple = get(x);
    auto y_tuple = get(y);
    override_tuple(x_tuple, y);
    override_tuple(y_tuple, x);
}


void TupleIdCollection::sort() {
    // Sort all the tuples. order_vars the position of the vars in each array that
    // define the order.
    quicksort(0, (*tuple_count) - 1);

    // Insertion sort:
    // for (size_t i = 0; i < *tuple_count - 1; i++) {
    //     for (size_t j = i + 1; j < *tuple_count; j++) {
    //         if (TupleIdCollection::has_priority(get(j), get(i))) {
    //             swap(i,j);
    //         }
    //     }
    // }
}


bool TupleIdCollection::has_priority(
    const std::vector<ObjectId>& lhs,
    const std::vector<ObjectId>& rhs)
{
    assert(order_vars.size() == ascending.size());
    for (size_t i = 0; i < order_vars.size(); i++) {
        uint_fast32_t index;
        auto search = saved_vars.find(order_vars[i]);
        if (search != saved_vars.end()) {
            index = search->second;
        } else {
            throw LogicException("saved_vars must contain VarId(" + std::to_string(order_vars[i].id) + ")");
        }

        auto left_value  = lhs[index];
        auto right_value = rhs[index];

        auto cmp = compare(left_value, right_value);
        if (cmp < 0) {
            return ascending[i];
        } else if (cmp > 0) {
            return !ascending[i];
        }
    }
    return true;
}


void TupleIdCollection::quicksort(int i, int f) {
    if (i < f) {
        int p = partition(i, f);
        quicksort(i, p - 1);
        quicksort(p + 1, f);
    }
}


int TupleIdCollection::partition(int i, int f) {
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


MergeOrderedTupleIdCollection::MergeOrderedTupleIdCollection(
    const map<VarId, uint_fast32_t>& saved_vars,
    const vector<VarId>&             order_vars,
    const vector<bool>&              ascending,
    int64_t(*_compare)(ObjectId, ObjectId)
) :
    saved_vars (saved_vars),
    order_vars (order_vars),
    ascending  (ascending),
    compare    (_compare) { }


void MergeOrderedTupleIdCollection::merge(uint64_t left_start,
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
            if (__builtin_expect(!!(get_query_ctx().thread_info.interruption_requested), 0)) {
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


void MergeOrderedTupleIdCollection::copy_page(
    uint64_t source_page,
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


std::unique_ptr<TupleIdCollection> MergeOrderedTupleIdCollection::get_run(Page& run_page) {
    return make_unique<TupleIdCollection>(run_page, saved_vars, order_vars, ascending, compare);
}
