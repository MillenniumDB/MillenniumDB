#include "semi_join.h"

#include <cmath>

#include "storage/page.h"

using namespace std;
using namespace HashJoin;
using namespace HashJoin::Generic;
using namespace HashJoin::Generic::Hybrid;

SemiJoin::SemiJoin(
    unique_ptr<BindingIter> _lhs,
    unique_ptr<BindingIter> _rhs,
    vector<VarId>&&           _join_vars,
    vector<VarId>&&           _lhs_vars,
    vector<VarId>&&           _rhs_vars
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


SemiJoin::~SemiJoin() {
    // Avoid mem leaks
    for (auto block: key_chunks_dir) {
        delete[](block);
    }
    delete[](pk_start);
}


void SemiJoin::begin(Binding& _parent_binding) {
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
            *parent_binding,
            *parent_binding,
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


bool SemiJoin::next() {
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
            if (iterator != hash_table.end()) {
                for (auto& var : join_vars) {
                    parent_binding->add(var, (*lhs_binding)[var]);
                }
                for (auto& var : lhs_vars) {
                    parent_binding->add(var, (*lhs_binding)[var]);
                }
                found++;
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


void SemiJoin::reset() {
    hash_table.clear();
    found = 0;

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


void SemiJoin::assign_nulls() {
    for (auto& var : join_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    for (auto& var : lhs_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    for (auto& var : rhs_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    original_lhs->assign_nulls();
    original_rhs->assign_nulls();
}


void SemiJoin::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "SemiJoinHybrid(Found: " << found << ")\n";
    original_lhs->analyze(os, indent + 2);
    original_rhs->analyze(os, indent + 2);
}


bool SemiJoin::build_0_partition() {

    while (original_rhs->next()) {
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
            if (key_chunk_index == Page::MDB_PAGE_SIZE) {
                key_chunk = new uint64_t[N * Page::MDB_PAGE_SIZE];
                key_chunks_dir.push_back(key_chunk);
                key_chunk_index = 0;
            }

            // Check if hash table does not exceed size limit
            if (hash_table.size() > MAX_HASH_TABLE_SIZE) {
                return false;
            }

        }
    }
    return true;
}


void SemiJoin::build_hash_table() {
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
            if (key_chunk_index == Page::MDB_PAGE_SIZE) {
                key_chunk = new uint64_t[N * Page::MDB_PAGE_SIZE];
                key_chunks_dir.push_back(key_chunk);
                key_chunk_index = 0;
            }
        }
    }
}


void SemiJoin::prepare_chunks_for_new_partition() {
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
    key_chunk = new uint64_t[N * Page::MDB_PAGE_SIZE];
    key_chunks_dir.push_back(key_chunk);
}


bool SemiJoin::get_next_partition() {
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
