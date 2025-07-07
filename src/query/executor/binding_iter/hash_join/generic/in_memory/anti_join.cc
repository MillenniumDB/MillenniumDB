#include "anti_join.h"

#include "storage/page/private_page.h"

using namespace std;
using namespace HashJoin::Generic;
using namespace HashJoin::Generic::InMemory;

AntiJoin::AntiJoin(
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
    pk_start(new uint64_t[N]),
    probe_key(Key(pk_start, N))
{
    key_chunk = new uint64_t[N * PPage::SIZE];
    key_chunks_dir.push_back(key_chunk);
    key_chunk_index = 0;
}

AntiJoin::~AntiJoin()
{
    // Avoid mem leaks
    for (auto block : key_chunks_dir) {
        delete[] (block);
    }
    delete[] (pk_start);
}

void AntiJoin::_begin(Binding& _parent_binding)
{
    // set hash join in start state, always must be non enumerating_row
    this->parent_binding = &_parent_binding;
    rhs_binding = make_unique<Binding>(parent_binding->size);
    lhs_binding = make_unique<Binding>(parent_binding->size);

    lhs->begin(*lhs_binding);
    rhs->begin(*rhs_binding);
    build_hash_table();
}

bool AntiJoin::_next()
{
    while (lhs->next()) {
        for (size_t i = 0; i < N; i++) {
            probe_key.start[i] = (*lhs_binding)[join_vars[i]].id;
        }
        auto iterator = hash_table.find(probe_key);
        // Only return lhs tuples that does not match
        if (iterator == hash_table.end()) {
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

void AntiJoin::_reset()
{
    hash_table.clear();

    for (size_t i = 1; i < key_chunks_dir.size(); i++) {
        delete[] (key_chunks_dir[i]);
    }
    // Resize directory
    key_chunks_dir.resize(1);

    // Reset indexes
    key_chunk_index = 0;

    // Set chunks to first element in array
    key_chunk = key_chunks_dir[0];

    // Set default probe key and last probe key
    for (size_t i = 0; i < N; i++) {
        probe_key.start[i] = ObjectId::MASK_NOT_FOUND;
    }

    // Spread reset to children
    rhs->reset();
    lhs->reset();

    build_hash_table();
}

void AntiJoin::assign_nulls()
{
    for (auto& var : join_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    for (auto& var : lhs_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    lhs->assign_nulls();
}

void AntiJoin::build_hash_table()
{
    // Only to avoid seg fault in fist iteration due to IdHashWrapper comparision
    uint64_t* dummy_last_key = new uint64_t[N];
    for (size_t i = 0; i < N; i++) {
        dummy_last_key[i] = ObjectId::MASK_NOT_FOUND;
    }
    Key last_key(dummy_last_key, N);

    // Avoid ask for space to a new key in each iteration
    uint64_t* key_pointer = new uint64_t[N];
    Key key(key_pointer, N);

    while (rhs->next()) {
        // Get start index to store key and data
        auto start_key_index = key_chunk_index * N;

        key.start = &key_chunk[start_key_index];
        // Store key
        for (size_t i = 0; i < N; i++) {
            key_chunk[start_key_index + i] = (*rhs_binding)[join_vars[i]].id;
        }

        // Check if last key is equal to current key that will be added
        if (key == last_key) {
            continue;
        } else {
            // If not are the same, update chunk index
            // and check if key chunk is full
            key_chunk_index++;
            if (key_chunk_index == PPage::SIZE) {
                key_chunk = new uint64_t[N * PPage::SIZE];
                key_chunks_dir.push_back(key_chunk);
                key_chunk_index = 0;
            }

            hash_table.emplace(key);
            last_key.start = key.start;
        }
    }
    delete[] (dummy_last_key);
    delete[] (key_pointer);
}
