#include "left_join.h"

#include <cmath>

#include "storage/page/private_page.h"

using namespace std;
using namespace HashJoin;
using namespace HashJoin::Generic;
using namespace HashJoin::Generic::Hybrid;

LeftJoin::LeftJoin(
    unique_ptr<BindingIter> _lhs,
    unique_ptr<BindingIter> _rhs,
    vector<VarId>&&         _join_vars,
    vector<VarId>&&         _lhs_vars,
    vector<VarId>&&         _rhs_vars
) :
    original_lhs  (std::move(_lhs)),
    original_rhs  (std::move(_rhs)),
    join_vars     (std::move(_join_vars)),
    lhs_vars      (std::move(_lhs_vars)),
    rhs_vars      (std::move(_rhs_vars)),
    N             (join_vars.size()),
    pk_start      (new uint64_t[N]),
    probe_key     (Key(pk_start, N))
{
    for (auto var : join_vars) {
        all_lhs_vars.push_back(var);
        all_rhs_vars.push_back(var);
    }
    for (auto var : lhs_vars) {
        all_lhs_vars.push_back(var);
    }
    for (auto var : rhs_vars) {
        all_rhs_vars.push_back(var);
    }
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


void LeftJoin::_begin(Binding& _parent_binding) {
    this->parent_binding = &_parent_binding;

    lhs_binding = make_unique<Binding>(parent_binding->size);
    rhs_binding = make_unique<Binding>(parent_binding->size);

    original_lhs->begin(*lhs_binding);
    original_rhs->begin(*rhs_binding);

    lhs = original_lhs.get();
    rhs = original_rhs.get();

    current_partition = 0;
    enumerating_rows = nullptr;

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
            all_rhs_vars,
            join_vars,
            partitions);
        // Reset rhs to iterate with 0 partition
        rhs->reset();
    }
}


bool LeftJoin::_next() {
    while (true) {
        // If enumerating rows != nullptr, then a row must be returned
        if (enumerating_rows != nullptr) {
            // Set binding from current enumerating row
            for (uint_fast32_t i = 0; i < rhs_vars.size(); i++) {
                parent_binding->add((rhs_vars)[i], ObjectId(enumerating_rows[i]));
            }
            // Update enumerating row to next value
            enumerating_rows = reinterpret_cast<uint64_t**>(enumerating_rows)[rhs_vars.size()];
            found_not_nulls++;
            return true;
        }
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

        } else if (get_next_partition()) {
            prepare_chunks_for_new_partition();
            build_hash_table();
            enumerating_rows = nullptr;

        } else {
            return false;
        }
    }
}


void LeftJoin::_reset() {
    hash_table.clear();
    found_nulls = 0;
    found_not_nulls = 0;

    // Spread reset to children
    original_lhs->reset();
    original_rhs->reset();

    lhs = original_lhs.get();
    rhs = original_rhs.get();

    current_partition = 0;
    enumerating_rows = nullptr;

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
            all_rhs_vars,
            join_vars,
            partitions);
        rhs->reset();
    }
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
    original_lhs->assign_nulls();
    original_rhs->assign_nulls();
}


void LeftJoin::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


bool LeftJoin::build_0_partition() {
    auto data_tuple_size = rhs_vars.size() + 1;

    // Avoid ask for space to a new key in each iteration
    auto start_key = new uint64_t[N];
    Key key(start_key, N);

    while (original_rhs->next()) {
        auto start_key_index = key_chunk_index * N;
        auto start_data_index = data_chunk_index * data_tuple_size;

        auto current_key_pos = &key_chunk[start_key_index];
        key.start = current_key_pos;
        // Store key
        for (auto& var : join_vars) {
            *current_key_pos = (*rhs_binding)[var].id;
            current_key_pos++;
        }
        // Store data
        auto current_data_pos = &data_chunk[start_data_index];
        for (auto& var: rhs_vars) {
            *current_data_pos = (*rhs_binding)[var].id;
            current_data_pos++;
        }

        // Set last data value as a null pointer
        auto casted_chunk = reinterpret_cast<uint64_t**>(data_chunk);
        casted_chunk[start_data_index + rhs_vars.size()] = nullptr;

        // Store pointer to the begin of the new row of data added
        auto data_pointer = &(data_chunk[start_data_index]);

        // Check if data chunk is full and add a new one if is needed
        data_chunk_index++;

        if (data_chunk_index == PPage::SIZE) {
            data_chunk = new uint64_t[data_tuple_size * PPage::SIZE];
            data_chunks_dir.push_back(data_chunk);
            data_chunk_index = 0;
        }

        auto iterator = hash_table.emplace(key, Value(data_pointer, data_pointer));
        if (iterator.second) {
            key_chunk_index++;
            if (key_chunk_index == PPage::SIZE) {
                key_chunk = new uint64_t[N * PPage::SIZE];
                key_chunks_dir.push_back(key_chunk);
                key_chunk_index = 0;
            }
        } else {
            auto casted_tail = reinterpret_cast<uint64_t**>(iterator.first->second.tail);
            casted_tail[rhs_vars.size()] = data_pointer;
            iterator.first->second.tail = data_pointer;
        }
        // Check if hash table does not exceed size limit
        if (hash_table.size() > MAX_HASH_TABLE_SIZE) {
            delete[](start_key);
            return false;
        }
    }
    delete[](start_key);
    return true;
}


void LeftJoin::build_hash_table() {
    auto data_tuple_size = rhs_vars.size() + 1;
    // Avoid ask for space to a new key in each iteration
    uint64_t* start_key = new uint64_t[N];
    Key key(start_key, N);

    while (rhs->next()) {
        auto start_key_index = key_chunk_index * N;
        auto start_data_index = data_chunk_index * data_tuple_size;

        auto current_key_pos = &key_chunk[start_key_index];
        key.start = current_key_pos;
        // Store key
        for (auto& var : join_vars) {
            *current_key_pos = (*rhs_binding)[var].id;
            current_key_pos++;
        }

        // Store data
        auto current_data_pos = &data_chunk[start_data_index];
        for (auto& var: rhs_vars) {
            *current_data_pos = (*rhs_binding)[var].id;
            current_data_pos++;
        }

        // Set last data value as a null pointer
        auto casted_chunk = reinterpret_cast<uint64_t**>(data_chunk);
        casted_chunk[start_data_index + rhs_vars.size()] = nullptr;

        // Store pointer to the begin of the new row of data added
        auto data_pointer = &(data_chunk[start_data_index]);

        // Check if data chunk is full and add a new one if is needed
        data_chunk_index++;
        if (data_chunk_index == PPage::SIZE) {
            data_chunk = new uint64_t[data_tuple_size * PPage::SIZE];
            data_chunks_dir.push_back(data_chunk);
            data_chunk_index = 0;
        }

        auto iterator = hash_table.emplace(key, Value(data_pointer, data_pointer));
        if (iterator.second) {
            key_chunk_index++;
            if (key_chunk_index == PPage::SIZE) {
                key_chunk = new uint64_t[N * PPage::SIZE];
                key_chunks_dir.push_back(key_chunk);
                key_chunk_index = 0;
            }
        } else {
            auto casted_tail = reinterpret_cast<uint64_t**>(iterator.first->second.tail);
            casted_tail[rhs_vars.size()] = data_pointer;
            iterator.first->second.tail = data_pointer;
        }
    }
    delete[](start_key);
}


void LeftJoin::prepare_chunks_for_new_partition() {
    hash_table.clear();

    // Delete chunks except first to avoid an unnecessary
    // request for space
    for (auto data_chunk : data_chunks_dir) {
        delete[] (data_chunk);
    }

    for (auto key_chunk : key_chunks_dir) {
        delete[] (key_chunk);
    }

    // Resize directories
    data_chunks_dir.resize(0);
    key_chunks_dir.resize(0);

    // Reset indexes
    data_chunk_index = 0;
    key_chunk_index = 0;

    // Size of data row:  build attr + 1 (one space to reserve next pointer)
    data_chunk = new uint64_t[(rhs_vars.size() + 1) * PPage::SIZE];
    data_chunks_dir.push_back(data_chunk);

    // Size of key row: join var
    key_chunk = new uint64_t[N * PPage::SIZE];
    key_chunks_dir.push_back(key_chunk);
}


bool LeftJoin::get_next_partition() {
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
