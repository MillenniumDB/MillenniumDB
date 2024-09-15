#include "anti_join.h"

#include <cmath>

#include "storage/page/private_page.h"

using namespace std;
using namespace HashJoin;
using namespace HashJoin::Generic;
using namespace HashJoin::Generic::Hybrid;

AntiJoin::AntiJoin(
    unique_ptr<BindingIter> _lhs,
    unique_ptr<BindingIter> _rhs,
    vector<VarId>&&         _join_vars,
    vector<VarId>&&         _lhs_vars,
    vector<VarId>&&         _rhs_vars
) :
    original_lhs     (std::move(_lhs)),
    original_rhs     (std::move(_rhs)),
    join_vars        (std::move(_join_vars)),
    lhs_vars         (std::move(_lhs_vars)),
    rhs_vars         (std::move(_rhs_vars)),
    N                (join_vars.size()),
    pk_start         (new uint64_t[N]),
    probe_key        (Key(pk_start, N))
{
    for (auto var : join_vars) {
        all_lhs_vars.push_back(var);
    }
    for (auto var : lhs_vars) {
        all_lhs_vars.push_back(var);
    }
}


AntiJoin::~AntiJoin() {
    // Avoid mem leaks
    for (auto block: key_chunks_dir) {
        delete[](block);
    }
    delete[](pk_start);
}


void AntiJoin::_begin(Binding& _parent_binding) {
    this->parent_binding = &_parent_binding;

    lhs_binding = make_unique<Binding>(parent_binding->size);
    rhs_binding = make_unique<Binding>(parent_binding->size);

    original_lhs->begin(*lhs_binding);
    original_rhs->begin(*rhs_binding);

    lhs = original_lhs.get();
    rhs = original_rhs.get();

    current_partition = 0;

    prepare_chunks_for_new_partition();
    // Try to load all original_lhs in hash table
    if (!build_0_partition()) {
        MaterializeIter::make_partitions(
            original_lhs.get(),
            original_rhs.get(),
            depth,
            MAX_HASH_TABLE_SIZE,
            *lhs_binding,
            *rhs_binding,
            all_lhs_vars,
            join_vars,
            join_vars,
            partitions);
        // Reset rhs to iterate with 0 partition
        rhs->reset();
    }
    for (auto& var: rhs_vars) {
        parent_binding->add(var, ObjectId::get_null());

    }
}


bool AntiJoin::_next() {
    while (true) {
        if (lhs->next()) {
            // If enumerating rows is nullptr ask for the next probe relation row
            // if has an entry in the hash table

            // Build key of probe row
            for (size_t i = 0; i < N; i++) {
                probe_key.start[i] = (*lhs_binding)[join_vars[i]].id;
            }

            // Asks for an entry in the hash table
            auto iterator = hash_table.find(probe_key);

            // If has an entry, then update enumerating rows
            if (iterator == hash_table.end()) {
                for (auto& var : join_vars) {
                    parent_binding->add(var, (*lhs_binding)[var]);
                }
                for (auto& var : lhs_vars) {
                    parent_binding->add(var, (*lhs_binding)[var]);
                }
                return true;

            }

        } else if (get_next_partition()) {
            prepare_chunks_for_new_partition();
            build_hash_table();

        } else {
            return false;
        }
    }
}


void AntiJoin::_reset() {
    hash_table.clear();

    // Spread reset to children
    original_lhs->reset();
    original_rhs->reset();

    lhs = original_lhs.get();
    rhs = original_rhs.get();

    current_partition = 0;

    prepare_chunks_for_new_partition();

    // Try to put all original_lhs in hash table
    if (!build_0_partition()) {
        MaterializeIter::make_partitions(
            original_lhs.get(),
            original_rhs.get(),
            depth,
            MAX_HASH_TABLE_SIZE,
            *parent_binding,
            *parent_binding,
            all_lhs_vars,
            join_vars,
            join_vars,
            partitions);
        rhs->reset();
    }
}


void AntiJoin::assign_nulls() {
    for (auto& var : join_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    for (auto& var : lhs_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    lhs->assign_nulls();
}


void AntiJoin::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


bool AntiJoin::build_0_partition() {
    while (original_rhs->next()) {
        auto start_key_index = key_chunk_index * N;
        auto current_key_pos = &key_chunk[start_key_index];
        // Store key
        for (auto& var : join_vars) {
            *current_key_pos = (*rhs_binding)[var].id;
            current_key_pos++;
        }

        // Check if last key is equal to current key that will be added

        auto iterator = hash_table.emplace(&key_chunk[start_key_index], N);
        if (iterator.second) {
            key_chunk_index++;
            if (key_chunk_index == PPage::SIZE) {
                key_chunk = new uint64_t[N * PPage::SIZE];
                key_chunks_dir.push_back(key_chunk);
                key_chunk_index = 0;
            }
        }
        // Check if hash table does not exceed size limit
        if (hash_table.size() > MAX_HASH_TABLE_SIZE) {
            return false;
        }
    }
    return true;
}


void AntiJoin::build_hash_table() {
    while (rhs->next()) {
        auto start_key_index = key_chunk_index * N;
        auto current_key_pos = &key_chunk[start_key_index];
        // Store key
        for (auto& var : join_vars) {
            *current_key_pos = (*rhs_binding)[var].id;
            current_key_pos++;
        }

        auto iterator = hash_table.emplace(&key_chunk[start_key_index], N);
        if (iterator.second) {
            key_chunk_index++;
            if (key_chunk_index == PPage::SIZE) {
                key_chunk = new uint64_t[N * PPage::SIZE];
                key_chunks_dir.push_back(key_chunk);
                key_chunk_index = 0;
            }
        }
    }
}


void AntiJoin::prepare_chunks_for_new_partition() {
    hash_table.clear();

    // Delete chunks except first to avoid an unnecessary
    // request for space

    for (auto key_chunk : key_chunks_dir) {
        delete[] (key_chunk);
    }

    // Resize directories
    key_chunks_dir.resize(0);

    // Reset indexes
    key_chunk_index = 0;

    // Size of key row: join var
    key_chunk = new uint64_t[N * PPage::SIZE];
    key_chunks_dir.push_back(key_chunk);
}


bool AntiJoin::get_next_partition() {
    while (current_partition < partitions.size()) {

        lhs = partitions[current_partition].first.get();
        rhs = partitions[current_partition].second.get();

        lhs->reset();
        rhs->reset();
        current_partition++;
        return true;
    }
    return false;
}
