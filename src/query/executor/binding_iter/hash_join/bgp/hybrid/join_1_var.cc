#include "join_1_var.h"

#include <cmath>
#include <cassert>

#include "storage/page.h"

using namespace std;
using namespace HashJoin;
using namespace HashJoin::BGP;
using namespace HashJoin::BGP::Hybrid;

Join1Var::Join1Var(
    unique_ptr<BindingIter> lhs,
    unique_ptr<BindingIter> rhs,
    vector<VarId>&&           join_vars,
    vector<VarId>&&           lhs_vars,
    vector<VarId>&&           rhs_vars
) :
    lhs           (std::move(lhs)),
    rhs           (std::move(rhs)),
    join_vars     (std::move(join_vars)),
    lhs_vars      (std::move(lhs_vars)),
    rhs_vars      (std::move(rhs_vars))
{
    assert(join_vars.size() == 1);

    for (auto var : lhs_vars) {
        all_lhs_vars.push_back(var);
    }

    for (auto var : rhs_vars) {
        all_rhs_vars.push_back(var);
    }
    all_lhs_vars.push_back(join_vars[0]);
    all_rhs_vars.push_back(join_vars[0]);
}


Join1Var::~Join1Var() {
    // Avoid mem leaks
    for (auto block : data_chunks_dir) {
        delete[](block);
    }
}


void Join1Var::begin(Binding& _parent_binding) {
    this->parent_binding = &_parent_binding;
    lhs->begin(*parent_binding);
    rhs->begin(*parent_binding);

    build = lhs.get();
    probe = rhs.get();
    build_vars = &lhs_vars;
    probe_vars = &rhs_vars;

    current_partition = 0;
    enumerating_rows = nullptr;

    prepare_chunks_for_new_partition();
    // Try to load all lhs in hash table
    if (!build_0_partition()) {
        MaterializeIter::make_partitions(
            lhs.get(),
            rhs.get(),
            depth,
            MAX_HASH_TABLE_SIZE,
            *parent_binding,
            *parent_binding,
            all_lhs_vars,
            all_rhs_vars,
            join_vars,
            partitions);
        // Reset probe to iterate with 0 partition
        probe->reset();
    }
}


bool Join1Var::next() {
    while (true) {
        if (enumerating_rows != nullptr) {
            for (uint_fast32_t i = 0; i < build_vars->size(); i++) {
                parent_binding->add((*build_vars)[i], ObjectId(enumerating_rows[i]));
            }
            enumerating_rows = reinterpret_cast<uint64_t**>(enumerating_rows)[build_vars->size()];
            found++;
            return true;
        }
        if (probe->next()) {
            auto iterator = hash_table.find((*parent_binding)[join_vars[0]]);
            if (iterator != hash_table.end()) {
                enumerating_rows = (iterator->second).head;
            }

        } else if (get_next_partition()) {
            prepare_chunks_for_new_partition();
            build_hash_table();
            enumerating_rows = nullptr;

        } else {
            return false;
        }
    }
}


void Join1Var::reset() {
    hash_table.clear();
    found = 0;

    // Spread reset to children
    lhs->reset();
    rhs->reset();

    build = lhs.get();
    probe = rhs.get();
    build_vars = &lhs_vars;
    probe_vars = &rhs_vars;

    current_partition = 0;
    enumerating_rows = nullptr;

    prepare_chunks_for_new_partition();

    // Try to put all lhs in hash table
    if (!build_0_partition()) {
        MaterializeIter::make_partitions(
            lhs.get(),
            rhs.get(),
            depth,
            MAX_HASH_TABLE_SIZE,
            *parent_binding,
            *parent_binding,
            all_lhs_vars,
            all_rhs_vars,
            join_vars,
            partitions);
        probe->reset();
    }
}


void Join1Var::assign_nulls() {
    lhs->assign_nulls();
    rhs->assign_nulls();
}


void Join1Var::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Join1Var(found: " << found << "\n";
    lhs->analyze(os, indent + 2);
    os << ",\n";
    rhs->analyze(os, indent + 2);
    os << "\n";
    os << std::string(indent, ' ');
    os << ")";
}


bool Join1Var::build_0_partition() {
    auto data_tuple_size = lhs_vars.size() + 1;

    while (lhs->next()) {
        auto start_data_index = data_chunk_index * data_tuple_size;

        // Store data
        auto current_data_pos = &data_chunk[start_data_index];
        for (auto& var: lhs_vars) {
            *current_data_pos = (*parent_binding)[var].id;
            current_data_pos++;
        }

        // Set last data value as a null pointer
        auto casted_chunk = reinterpret_cast<uint64_t**>(data_chunk);
        casted_chunk[start_data_index + lhs_vars.size()] = nullptr;

        // Store pointer to the begin of the new row of data added
        auto data_pointer = &(data_chunk[start_data_index]);

        // Check if data chunk is full and add a new one if is needed
        data_chunk_index++;

        if (data_chunk_index == Page::MDB_PAGE_SIZE) {
            data_chunk = new uint64_t[data_tuple_size * Page::MDB_PAGE_SIZE];
            data_chunks_dir.push_back(data_chunk);
            data_chunk_index = 0;
        }

        auto iterator = hash_table.emplace((*parent_binding)[join_vars[0]],
                                           Value(data_pointer, data_pointer));
        // If the key already has been inserted, iterator.second = false
        if (!iterator.second) {
            auto casted_tail = reinterpret_cast<uint64_t**>(iterator.first->second.tail);
            casted_tail[lhs_vars.size()] = data_pointer;
            iterator.first->second.tail = data_pointer;
        }
        // Check if hash table does not exceed size limit
        if (hash_table.size() > MAX_HASH_TABLE_SIZE) {
            return false;
        }
    }
    return true;
}


void Join1Var::build_hash_table() {
    auto data_tuple_size = build_vars->size() + 1;

    while (build->next()) {
        auto start_data_index = data_chunk_index * data_tuple_size;

        // Store data
        auto current_data_pos = &data_chunk[start_data_index];
        for (auto& var: *build_vars) {
            *current_data_pos = (*parent_binding)[var].id;
            current_data_pos++;
        }

        // Set last data value as a null pointer
        auto casted_chunk = reinterpret_cast<uint64_t**>(data_chunk);
        casted_chunk[start_data_index + build_vars->size()] = nullptr;

        // Store pointer to the begin of the new row of data added
        auto data_pointer = &(data_chunk[start_data_index]);

        // Check if data chunk is full and add a new one if is needed
        data_chunk_index++;
        if (data_chunk_index == Page::MDB_PAGE_SIZE) {
            data_chunk = new uint64_t[data_tuple_size * Page::MDB_PAGE_SIZE];
            data_chunks_dir.push_back(data_chunk);
            data_chunk_index = 0;
        }

        auto iterator = hash_table.emplace((*parent_binding)[join_vars[0]],
                                           Value(data_pointer, data_pointer));

        if (!iterator.second) {
            auto casted_tail = reinterpret_cast<uint64_t**>(iterator.first->second.tail);
            casted_tail[build_vars->size()] = data_pointer;
            iterator.first->second.tail = data_pointer;
        }
    }
}


void Join1Var::prepare_chunks_for_new_partition() {
    hash_table.clear();

    // Delete chunks except first to avoid an unnecessary
    // request for space
    for (auto data_chunk : data_chunks_dir) {
        delete[] (data_chunk);
    }
    // Resize directories
    data_chunks_dir.resize(0);

    // Reset indexes
    data_chunk_index = 0;

    // Size of data row:  build attr + 1 (one space to reserve next pointer)
    data_chunk = new uint64_t[(build_vars->size() + 1) * Page::MDB_PAGE_SIZE];
    data_chunks_dir.push_back(data_chunk);
}


bool Join1Var::get_next_partition() {
    while (current_partition < partitions.size()) {
        //std::cout << "First: " << partitions[current_partition].first->total_tuples << std::endl;
        //std::cout << "Second: " << partitions[current_partition].second->total_tuples << std::endl;
        // Avoid partitions where build or probe does not has tuples
        if (partitions[current_partition].first->total_tuples == 0 ||
            partitions[current_partition].second->total_tuples == 0)
        {
            current_partition++;
            continue;
        }
        // Check the smallest partition to use with build
        if (partitions[current_partition].first->total_tuples <
            partitions[current_partition].second->total_tuples)
        {
            build = partitions[current_partition].first.get();
            probe = partitions[current_partition].second.get();

            build_vars = &lhs_vars;
            probe_vars = &rhs_vars;
        } else {
            build = partitions[current_partition].second.get();
            probe = partitions[current_partition].first.get();

            build_vars = &rhs_vars;
            probe_vars = &lhs_vars;
        }
        probe->reset();
        build->reset();
        current_partition++;
        return true;
    }
    return false;
}
