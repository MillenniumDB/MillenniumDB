#include "tuple_collection.h"

#include <cstdlib>
#include <cstring>

#include "macros/likely.h"
#include "query/exceptions.h"
#include "query/query_context.h"
#include "system/buffer_manager.h"

using namespace std;

TupleCollectionPage::TupleCollectionPage(
    PPage& page,
    OrderInfo& info,
    int64_t (*_compare)(ObjectId, ObjectId)
) :
    page(page),
    info(info),
    tuples(reinterpret_cast<ObjectId*>(page.get_bytes())),
    tuple_count(reinterpret_cast<uint64_t*>(page.get_bytes() + PPage::SIZE - sizeof(uint64_t))),
    compare(_compare)
{ }

TupleCollectionPage::~TupleCollectionPage()
{
    page.make_dirty();
    buffer_manager.unpin(page);
}

void TupleCollectionPage::add(const ObjectId* tuple)
{
    // Add a new tuple in the last position of the page
    const size_t offset = (*tuple_count) * info.saved_vars.size();
    for (size_t i = 0; i < info.saved_vars.size(); i++) {
        tuples[offset + i] = tuple[i];
    }
    (*tuple_count)++;
}

const ObjectId* TupleCollectionPage::get(uint64_t n) const
{
    return &tuples[n * info.saved_vars.size()];
}

void TupleCollectionPage::reset()
{
    // Causes all tuples on the page will be ignored
    (*tuple_count) = 0;
}

void TupleCollectionPage::swap(int x, int y)
{
    size_t pos_x = x * info.saved_vars.size();
    size_t pos_y = y * info.saved_vars.size();
    for (size_t i = 0; i < info.saved_vars.size(); i++) {
        std::swap(tuples[pos_x + i], tuples[pos_y + i]);
    }
}

void TupleCollectionPage::sort()
{
    // Sort all the tuples. order_vars the position of the vars in each array that
    // define the order.
    quick_sort(0, (*tuple_count) - 1);
}

bool TupleCollectionPage::less_or_equal(const ObjectId* lhs, const ObjectId* rhs) const
{
    for (size_t i = 0; i < info.ascending.size(); i++) {
        auto cmp = compare(lhs[i], rhs[i]);
        if (cmp < 0) {
            return info.ascending[i];
        } else if (cmp > 0) {
            return !info.ascending[i];
        }
    }
    return true;
}

void TupleCollectionPage::insertion_sort(int lo, int hi)
{
    for (int i = lo + 1; i < hi + 1; i++) {
        for (int j = i; j > lo && !TupleCollectionPage::less_or_equal(get(j - 1), get(j)); j--) {
            this->swap(j, j - 1);
        }
    }
}

void TupleCollectionPage::quick_sort(int lo, int hi)
{
    if (lo + 16 < hi) {
        int p = partition(lo, hi);
        quick_sort(lo, p - 1);
        quick_sort(p + 1, hi);
    } else if (lo < hi) {
        insertion_sort(lo, hi);
    }
}

int TupleCollectionPage::partition(int lo, int hi)
{
    int x = (lo + hi) / 2;
    this->swap(x, hi);

    auto pivot = get(hi);
    int i = lo - 1;

    for (int j = lo; j < hi; j++) {
        if (less_or_equal(get(j), pivot)) {
            i++;
            this->swap(i, j);
        }
    }
    this->swap(i + 1, hi);
    return i + 1;
}

TupleCollectionMerger::TupleCollectionMerger(
    OrderInfo& info,
    int64_t (*_compare)(ObjectId, ObjectId)
) :
    info(info),
    compare(_compare)
{ }

void TupleCollectionMerger::merge(
    uint64_t left_start,
    uint64_t left_end,
    uint64_t right_start,
    uint64_t right_end,
    TmpFileId source_file_id,
    TmpFileId output_file_id
)
{
    // Merge [left_start-left_end] with [right_start-right_end] from source_file_id in output_file_id,
    // in output_file_id is sorted
    auto left_run = get_run(buffer_manager.get_ppage(source_file_id, left_start));
    auto right_run = get_run(buffer_manager.get_ppage(source_file_id, right_start));
    auto out_run = get_run(buffer_manager.get_ppage(output_file_id, left_start));
    out_run->reset();

    // use pointers with care, may be invalid after reassigning left_run/right_run
    auto left_tuple = left_run->get(0);
    auto right_tuple = right_run->get(0);

    uint64_t left_counter = 0;
    uint64_t right_counter = 0;
    uint64_t out_page_counter = left_start;
    bool left_first;
    bool open_left = true;
    bool open_right = true;

    while (open_left || open_right) {
        if (out_run->is_full()) {
            if (MDB_unlikely(get_query_ctx().thread_info.interruption_requested)) {
                throw InterruptedException();
            }
            out_page_counter++;
            out_run = get_run(buffer_manager.get_ppage(output_file_id, out_page_counter));
            out_run->reset();
        }
        left_first = left_run->less_or_equal(left_tuple, right_tuple);
        if (open_left && (left_first || !open_right)) {
            out_run->add(left_tuple);
            left_counter++;
            if (left_counter == left_run->get_tuple_count()) {
                left_start++;
                if (left_start <= left_end) {
                    left_run = get_run(buffer_manager.get_ppage(source_file_id, left_start));
                    left_counter = 0;
                } else {
                    open_left = false;
                    continue;
                }
            }
            left_tuple = left_run->get(left_counter);
        } else if (open_right && (!left_first || !open_left)) {
            out_run->add(right_tuple);
            right_counter++;
            if (right_counter == right_run->get_tuple_count()) {
                right_start++;
                if (right_start <= right_end) {
                    right_run = get_run(buffer_manager.get_ppage(source_file_id, right_start));
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

void TupleCollectionMerger::copy_page(
    uint64_t source_page,
    TmpFileId source_file_id,
    TmpFileId output_file_id
)
{
    auto source_tuples = get_run(buffer_manager.get_ppage(source_file_id, source_page));
    auto output_tuples = get_run(buffer_manager.get_ppage(output_file_id, source_page));
    output_tuples->reset();
    for (size_t i = 0; i < source_tuples->get_tuple_count(); i++) {
        auto tuple = source_tuples->get(i);
        output_tuples->add(tuple);
    }
    source_tuples->reset();
}

std::unique_ptr<TupleCollectionPage> TupleCollectionMerger::get_run(PPage& run_page)
{
    return make_unique<TupleCollectionPage>(run_page, info, compare);
}
