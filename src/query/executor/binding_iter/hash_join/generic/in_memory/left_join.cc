#include "left_join.h"

#include "storage/page.h"

using namespace std;
using namespace HashJoin;
using namespace HashJoin::Generic;
using namespace HashJoin::Generic::InMemory;

LeftJoin::LeftJoin(
    unique_ptr<BindingIter> _lhs,
    unique_ptr<BindingIter> _rhs,
    vector<VarId>&&           _join_vars,
    vector<VarId>&&           _lhs_vars,
    vector<VarId>&&           _rhs_vars
) :
    lhs               (std::move(_lhs)),
    rhs               (std::move(_rhs)),
    join_vars         (std::move(_join_vars)),
    lhs_vars          (std::move(_lhs_vars)),
    rhs_vars          (std::move(_rhs_vars)),
    N                 (join_vars.size()),
    pk_start          (new uint64_t[N]),
    probe_key         (Key(pk_start, N))
{
    data_chunk = new uint64_t[(rhs_vars.size() + 1) * Page::MDB_PAGE_SIZE];
    data_chunks_dir.push_back(data_chunk);
    data_chunk_index = 0;

    key_chunk = new uint64_t[N * Page::MDB_PAGE_SIZE];
    key_chunks_dir.push_back(key_chunk);
    key_chunk_index = 0;
}


LeftJoin::~LeftJoin() {
    // Avoid mem leaks
    for (auto block : data_chunks_dir) {
        delete[](block);
    }
    for (auto block: key_chunks_dir) {
        delete[](block);
    }
    delete[](pk_start);
}


void LeftJoin::begin(Binding& _parent_binding) {
    // set hash join in start state, always must be non enumerating_row
    enumerating_rows = nullptr;

    this->parent_binding = &_parent_binding;
    rhs_binding = make_unique<Binding>(parent_binding->size);
    lhs_binding = make_unique<Binding>(parent_binding->size);

    lhs->begin(*lhs_binding);
    rhs->begin(*rhs_binding);

    build_hash_table();
}


bool LeftJoin::next() {
    while (true) {
        // If enumerating_rows != nullptr, then a row must be returned
        if (enumerating_rows != nullptr) {
            // Set binding from current enumerating row
            for (uint_fast32_t i = 0; i < rhs_vars.size(); i++) {
                parent_binding->add(rhs_vars[i], ObjectId(enumerating_rows[i]));
            }
            // Update enumerating row to next value
            enumerating_rows = reinterpret_cast<uint64_t**>(enumerating_rows)[rhs_vars.size()];
            found_not_nulls++;
            return true;
        }
        else {
            if (lhs->next()) {
                for (size_t i = 0; i < N; i++) {
                    probe_key.start[i] = (*lhs_binding)[join_vars[i]].id;
                }
                auto iterator = hash_table.find(probe_key);
                for (auto& var : join_vars) {
                    parent_binding->add(var, (*lhs_binding)[var]);
                }
                for (auto& var : lhs_vars) {
                    parent_binding->add(var, (*lhs_binding)[var]);
                }

                if (iterator != hash_table.end()) {
                    enumerating_rows = (iterator->second).head;
                } else {
                    for (auto& var : rhs_vars) {
                        parent_binding->add(var, ObjectId::get_null());
                    }
                    found_nulls++;
                    return true;
                }
            } else {
                return false;
            }
        }
    }
}


void LeftJoin::reset() {
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

    // Set default probe key and last probe key
    for (size_t i = 0; i < N; i++) {
        probe_key.start[i] = ObjectId::MASK_NOT_FOUND;
    }

    // Spread reset to children
    rhs->reset();
    lhs->reset();

    build_hash_table();
}


void LeftJoin::assign_nulls() {
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


void LeftJoin::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "LeftJoinMemory(Not nulls: " << found_not_nulls << ", Nulls: " << found_nulls << ")\n";
    lhs->analyze(os, indent + 2);
    rhs->analyze(os, indent + 2);
}


void LeftJoin::build_hash_table() {
    auto data_tuple_size = rhs_vars.size() + 1;

    // Only to avoid seg fault in fist iteration due to IdHashWrapper comparision
    auto dummy_last_key = new uint64_t[N];
    for (size_t i = 0; i < N; i++) {
        dummy_last_key[i] = ObjectId::MASK_NOT_FOUND;
    }

    // Avoid ask for space to a new key in each iteration
    auto key_pointer = new uint64_t[N];
    Key key(key_pointer, N);

    Key last_key(dummy_last_key, N);
    Value* last_value = nullptr;

    while (rhs->next()) {
        // Get start index to store key and data
        auto start_key_index = key_chunk_index * N;
        auto start_data_index = data_chunk_index * data_tuple_size;

        key.start = &key_chunk[start_key_index];
        // Store key
        for (size_t i = 0; i < N; i++) {
            key_chunk[start_key_index + i] = (*rhs_binding)[join_vars[i]].id;
        }
        // Store data
        for (size_t i = 0; i < rhs_vars.size(); i++) {
            data_chunk[start_data_index + i] =  (*rhs_binding)[rhs_vars[i]].id;
        }
        // Set last data value as a null pointer
        auto casted_chunk = reinterpret_cast<uint64_t**>(data_chunk);
        casted_chunk[start_data_index + rhs_vars.size()] = nullptr;

        // Store pointer to the begin of the new row of data added
        auto data_pointer = &(data_chunk[start_data_index]);

        // Check if data chunk is full and add a new one if is needed
        data_chunk_index++;
        if (data_chunk_index == Page::MDB_PAGE_SIZE) {
            data_chunk = new uint64_t[data_tuple_size * Page::MDB_PAGE_SIZE];
            data_chunks_dir.push_back(data_chunk);
            data_chunk_index = 0;
        }

        // Check if last key is equal to current key that will be added
        if (key == last_key) {
            // If are equal, only update new value and pass to the next row
            auto casted_tail = reinterpret_cast<uint64_t**>(last_value->tail);
            casted_tail[rhs_vars.size()] = data_pointer;
            last_value->tail = data_pointer;
            continue;
        } else {
            // If not are the same, update chunk index
            // and check if key chunk is full
            key_chunk_index++;
            if (key_chunk_index == Page::MDB_PAGE_SIZE) {
                key_chunk = new uint64_t[N * Page::MDB_PAGE_SIZE];
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
                casted_tail[rhs_vars.size()] = data_pointer;
                iterator.first->second.tail = data_pointer;
            }
            // Update last_key and last value
            last_value = &iterator.first->second;
            last_key.start = key.start;
        }
    }
    delete[](dummy_last_key);
    delete[](key_pointer);
}
