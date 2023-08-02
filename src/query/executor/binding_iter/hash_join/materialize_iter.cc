#include "materialize_iter.h"

#include <cmath>

#include "storage/buffer_manager.h"
#include "storage/file_manager.h"

using namespace std;
using namespace HashJoin;

MaterializeIter::MaterializeIter(
    const vector<VarId>& vars,
    uint64_t             depth
) :
    depth                (depth),
    vars                 (vars),
    tmp_file             (file_manager.get_tmp_file_id()),
    max_tuples_per_page  (Page::MDB_PAGE_SIZE / (vars.size() * sizeof(ObjectId))) { }


MaterializeIter::~MaterializeIter() {
    buffer_manager.unpin(*current_page);
    file_manager.remove_tmp(tmp_file);
}


void MaterializeIter::begin(Binding& _parent_binding) {
    this->parent_binding = &_parent_binding;

    current_page_number = 0;
    current_page_index = 0;
    tuples_returned = 0;

    current_page = &buffer_manager.get_tmp_page(tmp_file, 0);
    current_page->make_dirty();
    start_page_pointer = reinterpret_cast<ObjectId*>(current_page->get_bytes());
}


// begin must be called before add
void MaterializeIter::add() {
    if (current_page_index == max_tuples_per_page) {
        current_page_number++;
        current_page_index = 0;
        buffer_manager.unpin(*current_page);
        // Get new page
        current_page = &buffer_manager.get_tmp_page(tmp_file, current_page_number);
        current_page->make_dirty();
        start_page_pointer = reinterpret_cast<ObjectId*>(current_page->get_bytes());
    }

    ObjectId* direction = &start_page_pointer[current_page_index * vars.size()];

    for (auto& var : vars) {
        *direction = (*parent_binding)[var];
        direction++;
    }
    total_tuples++;
    current_page_index++;
}


unique_ptr<MaterializeIter> MaterializeIter::split(const vector<VarId>& join_vars) {
    uint64_t separator = pow(2, depth);
    depth++;

    // Avoid split read more than maximum bits allowed to make a split
    if (depth > MAX_DEPTH) {
        throw runtime_error("Stop split to avoid infinite loop");
    }

    uint64_t write_page_n = 0;
    uint64_t write_total_tuples = 0;
    uint64_t write_current_index = 0;

    auto new_split = make_unique<MaterializeIter>(vars, depth);

    new_split->begin(*parent_binding);
    reset();

    auto write_current_page = &buffer_manager.get_tmp_page(tmp_file, 0);
    write_current_page->make_dirty();
    auto write_start_page_pointer = reinterpret_cast<ObjectId*>(write_current_page->get_bytes());

    auto mask = ((1ul << depth) - 1ul);

    auto key = new uint64_t[join_vars.size()];
    // TODO: don't use next, read directly from page.
    // TODO: avoid modifying the binding
    while (next()) {
        for (size_t i = 0; i < join_vars.size(); i++) {
            key[i] = (*parent_binding)[join_vars[i]].id;
        }
        auto hash = HashFunctionWrapper(&key[0], join_vars.size() * sizeof(uint64_t)) & mask;

        if (hash < separator) {
            if (write_current_index == max_tuples_per_page) {
                write_page_n++;
                write_current_index = 0;
                buffer_manager.unpin(*write_current_page);
                write_current_page = &buffer_manager.get_tmp_page(tmp_file, write_page_n);
                write_current_page->make_dirty();
                write_start_page_pointer = reinterpret_cast<ObjectId*>(write_current_page->get_bytes());
            }
            ObjectId* direction = &write_start_page_pointer[write_current_index * vars.size()];
            for (auto& var : vars) {
                *direction = (*parent_binding)[var];
                direction++;
            }
            write_current_index++;
            write_total_tuples++;
        } else {
            new_split->add();
        }
    }

    total_tuples = write_total_tuples;
    buffer_manager.unpin(*write_current_page);

    delete[] key;
    return new_split;
}


bool MaterializeIter::next() {
    // Check no return invalid tuples due to not full page
    if (tuples_returned < total_tuples) {
        if (current_page_index == max_tuples_per_page) {
            current_page_number++;
            current_page_index = 0;
            buffer_manager.unpin(*current_page);
            current_page = &buffer_manager.get_tmp_page(tmp_file, current_page_number);
            start_page_pointer = reinterpret_cast<ObjectId*>(current_page->get_bytes());
        }
        // Get next tuple
        ObjectId* direction = &start_page_pointer[current_page_index * vars.size()];
        for (auto& var : vars) {
            parent_binding->add(var, *direction);
            direction++;
        }
        // Update to next tuple
        current_page_index++;
        tuples_returned++;
        return true;
    }
    return false;
}


void MaterializeIter::reset() {
    current_page_number = 0;
    current_page_index = 0;
    tuples_returned = 0;
    buffer_manager.unpin(*current_page);
    current_page = &buffer_manager.get_tmp_page(tmp_file, 0);
    start_page_pointer = reinterpret_cast<ObjectId*>(current_page->get_bytes());
}


void MaterializeIter::analyze(std::ostream& /*os*/, int /*indent*/) const { }


void MaterializeIter::assign_nulls() { }


void MaterializeIter::make_partitions(
    BindingIter* lhs,
    BindingIter* rhs,
    const uint64_t depth,
    const size_t max_hash_table_size,
    Binding& lhs_binding,
    Binding& rhs_binding,
    const vector<VarId>& lhs_vars,
    const vector<VarId>& rhs_vars,
    const vector<VarId>& join_vars,
    vector<pair<unique_ptr<MaterializeIter>, unique_ptr<MaterializeIter>>>& partitions
) {
    size_t N = join_vars.size();
    uint64_t* key = new uint64_t[N];
    auto mask = ((1ul << depth) - 1ul);
    for (size_t i = 0; i < pow(2, depth); i++) {
        auto lhs_partition = make_unique<MaterializeIter>(lhs_vars, depth);
        auto rhs_partition = make_unique<MaterializeIter>(rhs_vars, depth);
        lhs_partition->begin(lhs_binding);
        rhs_partition->begin(rhs_binding);
        partitions.push_back(make_pair(std::move(lhs_partition), std::move(rhs_partition)));
    }
    // lhs has been already advanced to max hash table size tuple
    while (lhs->next()) {
        for (size_t i = 0; i < N; i++) {
            key[i] = lhs_binding[join_vars[i]].id;
        }
        auto hash = HashFunctionWrapper(&key[0], N * sizeof(uint64_t)) & mask;
        partitions[hash].first->add();
    }

    // Partition probe
    while (rhs->next()) {
        for (size_t i = 0; i < N; i++) {
            key[i] = rhs_binding[join_vars[i]].id;
        }
        auto hash = HashFunctionWrapper(&key[0], N * sizeof(uint64_t)) & mask;
        partitions[hash].second->add();
    }

    // TODO: maybe lazy evaluation to improve when we don't need all the results
    for (size_t i = 0; i < partitions.size(); i++) {
        if (partitions[i].first->total_tuples > max_hash_table_size &&
            partitions[i].second->total_tuples > max_hash_table_size)
        {
            auto lhs_split = partitions[i].first->split(join_vars);
            auto rhs_split = partitions[i].second->split(join_vars);
            // Avoid add partitions with trivially 0 results
            if (partitions[i].first->total_tuples == 0 ||
                partitions[i].second->total_tuples == 0)
            {
                partitions[i].first = std::move(lhs_split);
                partitions[i].second = std::move(rhs_split);
            } else if (lhs_split->total_tuples != 0 && rhs_split->total_tuples != 0) {
                partitions.push_back(make_pair(std::move(lhs_split), std::move(rhs_split)));
            }
            i--;
        }
    }
    delete[](key);
}
