#include "ordered_iter.h"

#include "system/buffer_manager.h"

namespace TextSearch {

OrderedIter::OrderedIter(std::unique_ptr<IndexIter> index_iter) :
    first_file_id(buffer_manager.get_tmp_file_id()),
    second_file_id(buffer_manager.get_tmp_file_id()),
    order_info({VarId(0)}, {true}, {VarId(1)})
{
    total_pages = 0;
    run = get_run(buffer_manager.get_ppage(first_file_id, total_pages));
    run->reset();

    // Save all the tuples of child_iter on disk and apply sort to each page
    while (index_iter->next()) {
        if (run->is_full()) {
            total_pages++;
            run->sort();
            run = get_run(buffer_manager.get_ppage(first_file_id, total_pages));
            run->reset();
        }

        ObjectId oids[2] = {
            ObjectId(index_iter->get_table_pointer()),
            ObjectId(index_iter->get_score())
        };
        // ObjectId oid(index_iter->get_table_pointer());
        run->add(oids);
    }
    run->sort();
    total_pages++;
    run = nullptr;

    merge_sort();
    run = get_run(buffer_manager.get_ppage(*output_file_id, 0));
}

OrderedIter::~OrderedIter()
{
    run.reset();
    buffer_manager.remove_tmp(first_file_id);
    buffer_manager.remove_tmp(second_file_id);
}

uint64_t OrderedIter::get_table_pointer() const
{
    return table_pointer;
}

uint64_t OrderedIter::get_score() const {
    return score;
}

void OrderedIter::reset()
{
    current_page = 0;
    page_position = 0;
}

bool OrderedIter::next()
{
    if (current_page >= total_pages) {
        return false;
    }

    if (page_position == run->get_tuple_count()) {
        current_page++;

        if (current_page >= total_pages) {
            return false;
        }

        run = get_run(buffer_manager.get_ppage(*output_file_id, current_page));
        page_position = 0;
    }

    const ObjectId* oids = run->get(page_position);
    table_pointer = oids[0].id;
    score = oids[1].id;

    page_position++;
    return true;
}

static int64_t compare(ObjectId a, ObjectId b)
{
    return a.id - b.id;
}

void OrderedIter::merge_sort()
{
    // Iterative merge sort implementation. Run to merge are a power of two
    uint_fast64_t start_page;
    uint_fast64_t end_page;
    uint_fast64_t middle;
    uint_fast64_t runs_to_merge = 1;

    TupleCollectionMerger merger(order_info, compare);

    bool output_is_in_second = false;

    const TmpFileId* source_pointer = &first_file_id;
    const TmpFileId* dest_pointer = &second_file_id;

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
                merger.copy_page(start_page, *source_pointer, *dest_pointer);
            } else {
                merger.merge(start_page, middle, middle + 1, end_page, *source_pointer, *dest_pointer);
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
        output_is_in_second = !output_is_in_second;
        source_pointer = output_is_in_second ? &second_file_id : &first_file_id;
        dest_pointer = output_is_in_second ? &first_file_id : &second_file_id;
    }
    output_file_id = output_is_in_second ? &second_file_id : &first_file_id;
}

std::unique_ptr<TupleCollectionPage> OrderedIter::get_run(PPage& run_page)
{
    return std::make_unique<TupleCollectionPage>(run_page, order_info, compare);
}

} // namespace TextSearch
