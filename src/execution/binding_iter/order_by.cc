#include "order_by.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdlib>

#include "base/exceptions.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/tuple_collection/tuple_collection.h"

using namespace std;

OrderBy::OrderBy(ThreadInfo*             _thread_info,
                 unique_ptr<BindingIter> _child,
                 const set<VarId>&       _saved_vars,
                 vector<VarId>           _order_vars,
                 vector<bool>            _ascending) :
    thread_info    (_thread_info),
    child          (move(_child)),
    order_vars     (move(_order_vars)),
    ascending      (move(_ascending)),
    first_file_id  (file_manager.get_tmp_file_id()),
    second_file_id (file_manager.get_tmp_file_id())
{
    uint_fast32_t current_index = 0;
    for (auto& var : _saved_vars) {
        saved_vars.insert({ var, current_index });
        current_index++;
    }
}


OrderBy::~OrderBy() {
    run.reset();
    file_manager.remove_tmp(first_file_id);
    file_manager.remove_tmp(second_file_id);
}


void OrderBy::begin(std::ostream& os) {
    child->begin(os);

    total_pages = 0;
    run = get_run(buffer_manager.get_tmp_page(first_file_id, total_pages));
    run->reset();
    std::vector<GraphObject> graph_objects(saved_vars.size());

    // Save all the tuples of child in disk and apply sort to each page
    while (child->next()) {
        if (run->is_full()) {
            total_pages++;
            run->sort();
            run = get_run(buffer_manager.get_tmp_page(first_file_id, total_pages));
            run->reset();
        }
        for (auto&& [var, index] : saved_vars) {
            graph_objects[index] = (*child)[var];
        }
        run->add(graph_objects);
    }
    run->sort();
    total_pages++;
    run = nullptr;
    merge_sort(order_vars);
    run = get_run(buffer_manager.get_tmp_page(*output_file_id, 0));
}


bool OrderBy::next() {
    if (page_position == run->get_tuple_count()) {
        current_page++;
        if (current_page >= total_pages) {
            return false;
        }
        run = get_run(buffer_manager.get_tmp_page(*output_file_id, current_page));
        page_position = 0;
    }
    saved_objects = run->get(page_position);
    page_position++;
    return true;
}


 GraphObject OrderBy::operator[](VarId var) const {
    auto search = saved_vars.find(var);
    if (search != saved_vars.end()) {
        return saved_objects[search->second];
    } else {
        throw LogicException("saved_vars must contain VarId(" + std::to_string(var.id) + ")");
    }
}


void OrderBy::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    child->analyze(os, indent);
    os << "OrderBy(";
    for (auto& var_id : order_vars) {
        os << " VarId(" << var_id.id << ")";
    }
    os << " )\n";
}


void OrderBy::merge_sort(const std::vector<VarId>& order_var_ids) {
    // Iterative merge sort implementation. Run to merge are a power of two
    uint_fast64_t start_page;
    uint_fast64_t end_page;
    uint_fast64_t middle;
    uint_fast64_t runs_to_merge = 1;

    MergeOrderedTupleCollection merger(saved_vars,
                                       order_var_ids,
                                       ascending,
                                       &thread_info->interruption_requested);

    // output_file_id = &first_file_id;
    bool output_is_in_second = false;

    TmpFileId* source_pointer = &first_file_id;
    TmpFileId* dest_pointer   = &second_file_id;

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


std::unique_ptr<TupleCollection> OrderBy::get_run(Page& run_page) {
    return make_unique<TupleCollection>(run_page, saved_vars, order_vars, ascending);
}
