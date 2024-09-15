#include "order_by.h"

#include "query/exceptions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "system/buffer_manager.h"
#include "system/file_manager.h"

using namespace std;

OrderBy::OrderBy(
    unique_ptr<BindingIter>  child_iter,
    const set<VarId>&        _saved_vars,
    vector<VarId>&&          order_vars,
    vector<bool>&&           ascending,
    int64_t(*_compare)(ObjectId, ObjectId)
) :
    child_iter     (std::move(child_iter)),
    order_vars     (std::move(order_vars)),
    ascending      (std::move(ascending)),
    first_file_id  (buffer_manager.get_tmp_file_id()),
    second_file_id (buffer_manager.get_tmp_file_id()),
    compare        (_compare)
{
    uint_fast32_t current_index = 0;
    for (auto& var : _saved_vars) {
        saved_vars.insert({ var, current_index });
        current_index++;
    }
}


OrderBy::~OrderBy() {
    run.reset();
    buffer_manager.remove_tmp(first_file_id);
    buffer_manager.remove_tmp(second_file_id);
}


void OrderBy::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    child_iter->begin(*parent_binding);

    total_pages = 0;
    run = get_run(buffer_manager.get_ppage(first_file_id, total_pages));
    run->reset();
    std::vector<ObjectId> object_ids(saved_vars.size());

    // Save all the tuples of child_iter on disk and apply sort to each page
    while (child_iter->next()) {
        if (run->is_full()) {
            total_pages++;
            run->sort();
            run = get_run(buffer_manager.get_ppage(first_file_id, total_pages));
            run->reset();
        }
        for (auto&& [var, index] : saved_vars) {
            object_ids[index] = (*parent_binding)[var];
        }
        run->add(object_ids);
    }
    run->sort();
    total_pages++;
    run = nullptr;

    merge_sort(order_vars);
    run = get_run(buffer_manager.get_ppage(*output_file_id, 0));

}


void OrderBy::_reset() {
    current_page = 0;
    page_position = 0;
}


bool OrderBy::_next() {
    if (page_position == run->get_tuple_count()) {
        current_page++;
        if (current_page >= total_pages) {
            return false;
        }
        run = get_run(buffer_manager.get_ppage(*output_file_id, current_page));
        page_position = 0;
    }

    auto saved_objects = run->get(page_position);
    for (auto&& [var, index] : saved_vars) {
        parent_binding->add(var, saved_objects[index]);
    }
    page_position++;
    return true;
}


void OrderBy::assign_nulls() {
    for (size_t i = 0; i < saved_vars.size(); i++) {
        (*parent_binding)[VarId(i)] = ObjectId::get_null();
    }
}


void OrderBy::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


void OrderBy::merge_sort(const std::vector<VarId>& order_var_ids) {
    // Iterative merge sort implementation. Run to merge are a power of two
    uint_fast64_t start_page;
    uint_fast64_t end_page;
    uint_fast64_t middle;
    uint_fast64_t runs_to_merge = 1;

    MergeOrderedTupleIdCollection merger(
        saved_vars,
        order_var_ids,
        ascending,
        compare
    );

    // output_file_id = &first_file_id;
    bool output_is_in_second = false;

    const TmpFileId* source_pointer = &first_file_id;
    const TmpFileId* dest_pointer   = &second_file_id;

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
        dest_pointer   = output_is_in_second ? &first_file_id  : &second_file_id;
    }
    output_file_id = output_is_in_second ? &second_file_id : &first_file_id;
}


std::unique_ptr<TupleIdCollection> OrderBy::get_run(PPage& run_page) {
    return make_unique<TupleIdCollection>(
        run_page,
        saved_vars,
        order_vars,
        ascending,
        compare
    );
}
