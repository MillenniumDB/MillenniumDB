#include "join.h"

#include <cassert>

#include "storage/page/private_page.h"

using namespace std;
using namespace HashJoin;
using namespace HashJoin::Generic;
using namespace HashJoin::Generic::InMemory;

Join::Join(
    unique_ptr<BindingIter> _lhs,
    unique_ptr<BindingIter> _rhs,
    vector<VarId>&&         _join_vars,
    vector<VarId>&&         _lhs_vars,
    vector<VarId>&&         _rhs_vars
) :
    rhs             (std::move(_rhs)),
    lhs             (std::move(_lhs)),
    join_vars       (std::move(_join_vars)),
    lhs_vars        (std::move(_lhs_vars)),
    rhs_vars        (std::move(_rhs_vars)),
    N               (join_vars.size()),
    pk_start        (new uint64_t[N]),
    last_pk_start   (new uint64_t[N]),
    rhs_key         (Key(pk_start, N)),
    last_rhs_key    (Key(last_pk_start, N))
{
    // Size of data row:  lhs attr  + 1 (one space to reserve next pointer)
    data_chunk = new uint64_t[(lhs_vars.size() + 1) * PPage::SIZE];
    // Add to directory
    data_chunks_dir.push_back(data_chunk);
    // Set chunk index in 0
    data_chunk_index = 0;

    // Size of key row: join var
    key_chunk = new uint64_t[N * PPage::SIZE];
    // Add to directory
    key_chunks_dir.push_back(key_chunk);
    // Set chunk index in 0
    key_chunk_index = 0;

}


Join::~Join() {
    // Avoid mem leaks
    for (auto block : data_chunks_dir) {
        delete[](block);
    }
    for (auto block: key_chunks_dir) {
        delete[](block);
    }
    delete[](pk_start);
    delete[](last_pk_start);
}


void Join::_begin(Binding& _parent_binding) {
    // set hash join in start state, always must be non enumerating_row
    enumerating_rows = nullptr;

    // Set dummy last_rhs key for avoid seg fault in first next iteration
    for (size_t i = 0; i < N; i++) {
        last_rhs_key.start[i] = ObjectId::MASK_NOT_FOUND;
    }

    this->parent_binding = &_parent_binding;
    rhs_binding = make_unique<Binding>(parent_binding->size);
    lhs_binding = make_unique<Binding>(parent_binding->size);

    lhs->begin(*lhs_binding);
    rhs->begin(*rhs_binding);

    build_hash_table();
}


bool Join::_next() {
    while (true) {
        // If enumerating rows != nullptr, then a row must be returned
        if (enumerating_rows != nullptr) {
            // Set binding from current enumerating row
            for (uint_fast32_t i = 0; i < lhs_vars.size(); i++) {
                parent_binding->add(lhs_vars[i], ObjectId(enumerating_rows[i]));
            }
            // Update enumerating row to next value
            enumerating_rows = reinterpret_cast<uint64_t**>(enumerating_rows)[lhs_vars.size()];
            return true;
        }
        else {
            // If enumerating rows is nullptr ask for the next rhs relation row
            // if has an entry in the hash table
            if (rhs->next()) {
                // Build key of rhs row
                for (size_t i = 0; i < N; i++) {
                    rhs_key.start[i] = (*rhs_binding)[join_vars[i]].id;
                }

                // Check if rhs key is equal to last key returned
                // to avoid an ask to hash table
                if (rhs_key == last_rhs_key) {
                    for (auto& var : join_vars) {
                        parent_binding->add(var, (*rhs_binding)[var]);
                    }
                    for (auto& var : rhs_vars) {
                        parent_binding->add(var, (*rhs_binding)[var]);
                    }
                    enumerating_rows = last_enumerating_rows;
                    continue;
                }
                // Asks for an entry in the hash table
                auto iterator = hash_table.find(rhs_key);
                // If has an entry, then update enumerating rows
                if (iterator != hash_table.end()) {
                    for (auto& var : join_vars) {
                        parent_binding->add(var, (*rhs_binding)[var]);
                    }
                    for (auto& var : rhs_vars) {
                        parent_binding->add(var, (*rhs_binding)[var]);
                    }
                    enumerating_rows = (iterator->second).head;
                    // Store enumerating rows and last key for
                    // try to avoid asking hash table in the following iterations
                    last_enumerating_rows = (iterator->second).head;
                    for (size_t i = 0; i < N; i++) {
                        last_rhs_key.start[i] = rhs_key.start[i];
                    }
                }
            }
            else {
                return false;
            }
        }
    }
}


void Join::_reset() {
    hash_table.clear();

    // Delete chunks except first to avoid an unnecessary
    // request for space
    for (size_t i = 1; i < data_chunks_dir.size(); i++) {
        delete[] (data_chunks_dir[i]);
    }

    for (size_t i = 1; i < key_chunks_dir.size(); i++) {
        delete[] (key_chunks_dir[i]);
    }
    // Resize directories
    data_chunks_dir.resize(1);
    key_chunks_dir.resize(1);

    // Reset indexes
    data_chunk_index = 0;
    key_chunk_index = 0;

    // Set chunks to first element in array
    key_chunk = key_chunks_dir[0];
    data_chunk = data_chunks_dir[0];

    // Set enumerating rows as nullptr like in begin
    enumerating_rows = nullptr;
    last_enumerating_rows = nullptr;

    // Set default rhs key and last rhs key
    for (size_t i = 0; i < N; i++) {
        rhs_key.start[i] = ObjectId::MASK_NOT_FOUND;
        last_rhs_key.start[i] = ObjectId::MASK_NOT_FOUND;
    }

    // Spread reset to children
    lhs->reset();
    rhs->reset();

    build_hash_table();
}


void Join::assign_nulls() {
    for (auto& var : join_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    for (auto& var : lhs_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    for (auto& var : rhs_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
    lhs->assign_nulls();
    rhs->assign_nulls();
}


void Join::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


void Join::build_hash_table() {
    auto data_tuple_size = lhs_vars.size() + 1;

    // Only to avoid seg fault in fist iteration due to Key comparision
    auto dummy_last_key = new uint64_t[N];
    for (size_t i = 0; i < N; i++) {
        dummy_last_key[i] = ObjectId::MASK_NOT_FOUND;
    }

    // Avoid ask for space to a new key in each iteration
    auto start_key = new uint64_t[N];
    Key key(start_key, N);

    // Optimization: Store the last key added, then compare the new key
    // if are equal to the last, avoid store in the chunk and compute
    // a hash, only modify the last value
    Key last_key(dummy_last_key, N);
    Value* last_value = nullptr;

    while (lhs->next()) {
        // Get start index to store key and data
        auto start_key_index = key_chunk_index * N;
        auto start_data_index = data_chunk_index * data_tuple_size;

        key.start = &key_chunk[start_key_index];
        // Store key
        for (size_t i = 0; i < N; i++) {
            key_chunk[start_key_index + i] = (*lhs_binding)[join_vars[i]].id;
        }
        // Store data
        for (size_t i = 0; i < lhs_vars.size(); i++) {
            data_chunk[start_data_index + i] =  (*lhs_binding)[lhs_vars[i]].id;
        }
        // Set last data value as a null pointer
        auto casted_chunk = reinterpret_cast<uint64_t**>(data_chunk);
        casted_chunk[start_data_index + lhs_vars.size()] = nullptr;

        // Store pointer to the begin of the new row of data added
        auto data_pointer = &(data_chunk[start_data_index]);

        // Check if data chunk is full and add a new one if is needed
        data_chunk_index++;
        if (data_chunk_index == PPage::SIZE) {
            data_chunk = new uint64_t[data_tuple_size * PPage::SIZE];
            data_chunks_dir.push_back(data_chunk);
            data_chunk_index = 0;
        }

        // Check if last key is equal to current key that will be added
        if (key == last_key) {
            // If are equal, only update new value and pass to the next row
            auto casted_tail = reinterpret_cast<uint64_t**>(last_value->tail);
            casted_tail[lhs_vars.size()] = data_pointer;
            last_value->tail = data_pointer;
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

            // Try to create a new entry in the hash table
            auto iterator = hash_table.emplace(key,
                                               Value(data_pointer,
                                                     data_pointer)
                                    );
            // If the key already has been inserted, iterator.second = false
            if (!(iterator.second)) {
                // If the key already exists, get the value of the hash table
                // and make an append in the linked list
                auto casted_tail = reinterpret_cast<uint64_t**>(iterator.first->second.tail);
                casted_tail[lhs_vars.size()] = data_pointer;
                iterator.first->second.tail = data_pointer;
            }
            // Update last_key and last value
            last_value = &iterator.first->second;
            last_key.start = key.start;
        }
    }
    delete[](dummy_last_key);
    delete[](start_key);
}
