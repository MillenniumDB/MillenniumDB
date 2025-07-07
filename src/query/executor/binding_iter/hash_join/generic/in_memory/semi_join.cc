#include "semi_join.h"

#include <cassert>

#include "storage/page/private_page.h"

using namespace std;
using namespace HashJoin::Generic;
using namespace HashJoin::Generic::InMemory;

SemiJoin::SemiJoin(
    unique_ptr<BindingIter> _lhs,
    unique_ptr<BindingIter> _rhs,
    vector<VarId>&& _join_vars,
    vector<VarId>&& _lhs_vars,
    vector<VarId>&& _rhs_vars
) :
    lhs(std::move(_lhs)),
    rhs(std::move(_rhs)),
    join_vars(std::move(_join_vars)),
    lhs_vars(std::move(_lhs_vars)),
    rhs_vars(std::move(_rhs_vars)),
    N(join_vars.size()),
    lhs_key_start(new uint64_t[N]),
    lhs_key(Key(lhs_key_start, N))
{
    key_chunk = new uint64_t[N * PPage::SIZE];
    key_chunks_dir.push_back(key_chunk);
    key_chunk_index = 0;
}

SemiJoin::~SemiJoin()
{
    // Avoid mem leaks
    for (auto block : key_chunks_dir) {
        delete[] (block);
    }
    delete[] (lhs_key_start);
}

void SemiJoin::_begin(Binding& _parent_binding)
{
    this->parent_binding = &_parent_binding;
    rhs_binding = make_unique<Binding>(parent_binding->size);
    lhs_binding = make_unique<Binding>(parent_binding->size);

    lhs->begin(*lhs_binding);
    rhs->begin(*rhs_binding);

    build_hash_table();
}

bool SemiJoin::_next()
{
    while (lhs->next()) {
        for (size_t i = 0; i < N; i++) {
            lhs_key.start[i] = (*lhs_binding)[join_vars[i]].id;
        }
        auto iterator = hash_table.find(lhs_key);
        if (iterator != hash_table.end()) {
            for (auto& var : join_vars) {
                parent_binding->add(var, (*lhs_binding)[var]);
            }
            for (auto& var : lhs_vars) {
                parent_binding->add(var, (*lhs_binding)[var]);
            }
            return true;
        }
    }
    return false;
}

void SemiJoin::_reset()
{
    hash_table.clear();

    // Delete chunks except first to avoid an unnecessary
    // request for space
    for (size_t i = 1; i < key_chunks_dir.size(); i++) {
        delete[] (key_chunks_dir[i]);
    }
    // Resize directories
    key_chunks_dir.resize(1);

    // Reset indexes
    key_chunk_index = 0;

    // Set chunks to first element in array
    key_chunk = key_chunks_dir[0];

    // Set default probe key and last probe key
    for (size_t i = 0; i < N; i++) {
        lhs_key.start[i] = ObjectId::MASK_NOT_FOUND;
    }

    // Spread reset to children
    rhs->reset();
    lhs->reset();

    build_hash_table();
}

void SemiJoin::assign_nulls()
{
    for (auto& var : join_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    for (auto& var : lhs_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    for (auto& var : rhs_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    rhs->assign_nulls();
    lhs->assign_nulls();
}

void SemiJoin::build_hash_table()
{
    // Avoid ask for space to a new key in each iteration
    auto key_pointer = new uint64_t[N];
    Key key(key_pointer, N);

    while (rhs->next()) {
        // Get start index to store key and data
        auto start_key_index = key_chunk_index * N;

        key.start = &key_chunk[start_key_index];
        // Store key
        for (size_t i = 0; i < N; i++) {
            key_chunk[start_key_index + i] = (*rhs_binding)[join_vars[i]].id;
        }

        // Try to create a new entry in the hash table
        auto iterator = hash_table.emplace(key);

        // Update chunks index only if value has inserted
        if (iterator.second) {
            key_chunk_index++;
            if (key_chunk_index == PPage::SIZE) {
                key_chunk = new uint64_t[N * PPage::SIZE];
                key_chunks_dir.push_back(key_chunk);
                key_chunk_index = 0;
            }
        }
    }
    delete[] (key_pointer);
}
