#include "ordered_group.h"

#include <cstdlib>
#include <cstring>

#include "macros/likely.h"
#include "query/exceptions.h"
#include "query/query_context.h"
#include "system/buffer_manager.h"

using namespace std;

OrderedGroupPage::OrderedGroupPage(PPage& page, OrderedGroupInfo& info) :
    page(page),
    info(info),
    tuples(reinterpret_cast<ObjectId*>(page.get_bytes())),
    tuple_count(reinterpret_cast<uint64_t*>(page.get_bytes() + PPage::SIZE - sizeof(uint64_t)))
{ }

OrderedGroupPage::~OrderedGroupPage()
{
    page.make_dirty();
    buffer_manager.unpin(page);
}

void OrderedGroupPage::add(const ObjectId* tuple)
{
    // Add a new tuple in the last position of the page
    const size_t offset = (*tuple_count) * info.saved_vars.size();
    for (size_t i = 0; i < info.saved_vars.size(); i++) {
        tuples[offset + i] = tuple[i];
    }
    (*tuple_count)++;
}

const ObjectId* OrderedGroupPage::get(uint64_t n) const
{
    return &tuples[n * info.saved_vars.size()];
}

void OrderedGroupPage::reset()
{
    // Causes all tuples on the page will be ignored
    (*tuple_count) = 0;
}

void OrderedGroupPage::swap(int x, int y)
{
    size_t pos_x = x * info.saved_vars.size();
    size_t pos_y = y * info.saved_vars.size();
    for (size_t i = 0; i < info.saved_vars.size(); i++) {
        std::swap(tuples[pos_x + i], tuples[pos_y + i]);
    }
}

void OrderedGroupPage::sort()
{
    // Sort all the tuples. order_vars the position of the vars in each array that
    // define the order.
    quick_sort(0, (*tuple_count) - 1);
}

bool OrderedGroupPage::less_or_equal(const ObjectId* lhs, const ObjectId* rhs) const
{
    for (size_t i = 0; i < info.order_size; i++) {
        if (lhs[i].id < rhs[i].id) {
            return true;
        } else if (lhs[i].id > rhs[i].id) {
            return false;
        }
    }
    return true;
}

void OrderedGroupPage::insertion_sort(int lo, int hi)
{
    for (int i = lo + 1; i < hi + 1; i++) {
        for (int j = i; j > lo && !OrderedGroupPage::less_or_equal(get(j - 1), get(j)); j--) {
            this->swap(j, j - 1);
        }
    }
}

void OrderedGroupPage::quick_sort(int lo, int hi)
{
    if (lo + 16 < hi) {
        int p = partition(lo, hi);
        quick_sort(lo, p - 1);
        quick_sort(p + 1, hi);
    } else if (lo < hi) {
        insertion_sort(lo, hi);
    }
}

int OrderedGroupPage::partition(int lo, int hi)
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

OrderedGroupMerger::OrderedGroupMerger(OrderedGroupInfo& info) :
    info(info)
{ }

void OrderedGroupMerger::merge(
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

void OrderedGroupMerger::copy_page(uint64_t source_page, TmpFileId source_file_id, TmpFileId output_file_id)
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

std::unique_ptr<OrderedGroupPage> OrderedGroupMerger::get_run(PPage& run_page)
{
    return std::make_unique<OrderedGroupPage>(run_page, info);
}

OrderedGroup::OrderedGroup(const std::set<VarId>& group_vars, std::set<VarId>&& order_saved_vars) :
    order_info(group_vars, std::move(order_saved_vars)),
    current_file_id(buffer_manager.get_tmp_file_id()),
    aux_file_id(buffer_manager.get_tmp_file_id())
{ }

OrderedGroup::~OrderedGroup()
{
    run.reset();
    buffer_manager.remove_tmp(current_file_id);
    buffer_manager.remove_tmp(aux_file_id);
}

void OrderedGroup::begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;

    run = get_run(buffer_manager.get_ppage(current_file_id, 0));

    object_ids.resize(order_info.saved_vars.size());
}

void OrderedGroup::reset()
{
    current_page = 0;
    page_position = 0;

    run = get_run(buffer_manager.get_ppage(current_file_id, 0));
}

void OrderedGroup::add(Binding& binding)
{
    if (run->is_full()) {
        total_pages++;
        run->sort();
        run = get_run(buffer_manager.get_ppage(current_file_id, total_pages));
        run->reset();
    }
    for (size_t i = 0; i < order_info.saved_vars.size(); i++) {
        object_ids[i] = binding[order_info.saved_vars[i]];
    }
    run->add(object_ids.data());
}

void OrderedGroup::end_appends()
{
    run->sort();
    total_pages++;
    run = nullptr;

    if (total_pages > 1) {
        merge_sort();
    }

    run = get_run(buffer_manager.get_ppage(current_file_id, 0));
}

bool OrderedGroup::next()
{
    if (page_position == run->get_tuple_count()) {
        current_page++;
        if (current_page >= total_pages) {
            return false;
        }
        run = get_run(buffer_manager.get_ppage(current_file_id, current_page));
        page_position = 0;
    }

    auto tuple = run->get(page_position);
    for (size_t i = 0; i < order_info.saved_vars.size(); i++) {
        parent_binding->add(order_info.saved_vars[i], tuple[i]);
    }
    page_position++;
    return true;
}

std::unique_ptr<OrderedGroupPage> OrderedGroup::get_run(PPage& run_page)
{
    return std::make_unique<OrderedGroupPage>(run_page, order_info);
}

void OrderedGroup::merge_sort()
{
    // Iterative merge sort implementation. Run to merge are a power of two
    uint64_t start_page;
    uint64_t end_page;
    uint64_t middle;
    uint64_t runs_to_merge = 1;

    OrderedGroupMerger merger(order_info);

    while (runs_to_merge < total_pages) {
        runs_to_merge *= 2;
        start_page = 0;
        middle = (runs_to_merge / 2) - 1;
        if (runs_to_merge > total_pages) {
            runs_to_merge = total_pages;
            end_page = runs_to_merge - 1;
        } else {
            end_page = runs_to_merge - 1;
        }
        while (start_page < total_pages) {
            if (start_page == end_page) {
                merger.copy_page(start_page, current_file_id, aux_file_id);
            } else {
                merger.merge(start_page, middle, middle + 1, end_page, current_file_id, aux_file_id);
            }
            start_page = end_page + 1;
            middle = start_page + (runs_to_merge / 2) - 1;
            end_page += runs_to_merge;
            if (end_page >= total_pages) {
                end_page = total_pages - 1;
            }
            if (middle >= end_page) {
                middle = (start_page + end_page) / 2;
            }
        }
        std::swap(current_file_id, aux_file_id);
    }
}
