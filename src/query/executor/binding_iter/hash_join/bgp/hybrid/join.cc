#include "join.h"

#include <cmath>

#include "storage/page/private_page.h"

using namespace std;
using namespace HashJoin;
using namespace HashJoin::BGP;
using namespace HashJoin::BGP::Hybrid;

template<std::size_t N>
Join<N>::Join(
    unique_ptr<BindingIter> lhs,
    unique_ptr<BindingIter> rhs,
    vector<VarId>&&         join_vars,
    vector<VarId>&&         lhs_vars,
    vector<VarId>&&         rhs_vars
) :
    lhs           (std::move(lhs)),
    rhs           (std::move(rhs)),
    join_vars     (std::move(join_vars)),
    lhs_vars      (std::move(lhs_vars)),
    rhs_vars      (std::move(rhs_vars)),
    probe_key     (Key<N>(pk_start))
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


template<std::size_t N>
Join<N>::~Join() {
    // Avoid mem leaks
    for (auto block : data_chunks_dir) {
        delete[](block);
    }
    for (auto block: key_chunks_dir) {
        delete[](block);
    }
}


template<std::size_t N>
void Join<N>::_begin(Binding& _parent_binding) {
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


template<std::size_t N>
bool Join<N>::_next() {
    while (true) {
        // If enumerating rows != nullptr, then a row must be returned
        if (enumerating_rows != nullptr) {
            // Set binding from current enumerating row
            for (uint_fast32_t i = 0; i < build_vars->size(); i++) {
                parent_binding->add((*build_vars)[i], ObjectId(enumerating_rows[i]));
            }
            // Update enumerating row to next value
            enumerating_rows = reinterpret_cast<uint64_t**>(enumerating_rows)[build_vars->size()];
            return true;
        }
        if (probe->next()) {
            // If enumerating rows is nullptr ask for the next probe relation row
            // if has an entry in the hash table

            // Build key of probe row
            for (size_t i = 0; i < N; i++) {
                probe_key.start[i] = (*parent_binding)[join_vars[i]].id;
            }

            // Asks for an entry in the hash table
            auto iterator = hash_table.find(probe_key);
            // If has an entry, then update enumerating rows
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


template<std::size_t N>
void Join<N>::_reset() {
    hash_table.clear();

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


template<std::size_t N>
void Join<N>::assign_nulls() {
    lhs->assign_nulls();
    rhs->assign_nulls();
}


template<std::size_t N>
void Join<N>::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


template<std::size_t N>
bool Join<N>::build_0_partition() {
    auto data_tuple_size = lhs_vars.size() + 1;

    // Only to avoid seg fault in fist iteration due to Key comparision
    std::array<uint64_t, N> dummy_last_key;
    for (size_t i = 0; i < N; i++) {
        dummy_last_key[i] = ObjectId::MASK_NOT_FOUND;
    }

    // Avoid ask for space to a new key in each iteration
    Key<N> key(nullptr);

    // Optimization: Store the last key added, then compare the new key
    // if are equal to the last, avoid store in the chunk and compute
    // a hash, only modify the last value
    Key<N> last_key(dummy_last_key.data());
    Value* last_value = nullptr;

    while (lhs->next()) {
        auto start_key_index = key_chunk_index * N;
        auto start_data_index = data_chunk_index * data_tuple_size;

        auto current_key_pos = &key_chunk[start_key_index];
        key.start = current_key_pos;
        // Store key
        for (auto& var : join_vars) {
            *current_key_pos = (*parent_binding)[var].id;
            current_key_pos++;
        }
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
            auto iterator = hash_table.emplace(key, Value(data_pointer, data_pointer));
            // If the key already has been inserted, iterator.second = false
            if (!iterator.second) {
                // If the key already exists, get the value of the hash table
                // and make an append in the linked list
                auto casted_tail = reinterpret_cast<uint64_t**>(iterator.first->second.tail);
                casted_tail[lhs_vars.size()] = data_pointer;
                iterator.first->second.tail = data_pointer;
            }
            // Update last_key and last value
            last_value = &iterator.first->second;
            last_key.start = key.start;

            // Check if hash table does not exceed size limit
            if (hash_table.size() > MAX_HASH_TABLE_SIZE) {
                return false;
            }
        }
    }
    return true;
}


template<std::size_t N>
void Join<N>::build_hash_table() {
    auto data_tuple_size = build_vars->size() + 1;

    // Only to avoid seg fault in fist iteration due to Key comparision
    std::array<uint64_t, N> dummy_last_key;
    for (size_t i = 0; i < N; i++) {
        dummy_last_key[i] = ObjectId::MASK_NOT_FOUND;
    }

    // Avoid ask for space to a new key in each iteration
    Key<N> key(nullptr);

    // Optimization: Store the last key added, then compare the new key
    // if are equal to the last, avoid store in the chunk and compute
    // a hash, only modify the last value
    Key<N> last_key(dummy_last_key.data());
    Value* last_value = nullptr;

    while (build->next()) {
        auto start_key_index = key_chunk_index * N;
        auto start_data_index = data_chunk_index * data_tuple_size;

        auto current_key_pos = &key_chunk[start_key_index];
        key.start = current_key_pos;
        // Store key
        for (auto& var : join_vars) {
            *current_key_pos = (*parent_binding)[var].id;
            current_key_pos++;
        }

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
        if (data_chunk_index == PPage::SIZE) {
            data_chunk = new uint64_t[data_tuple_size * PPage::SIZE];
            data_chunks_dir.push_back(data_chunk);
            data_chunk_index = 0;
        }

        // Check if last key is equal to current key that will be added
        if (key == last_key) {
            // If are equal, only update new value and pass to the next row
            auto casted_tail = reinterpret_cast<uint64_t**>(last_value->tail);
            casted_tail[build_vars->size()] = data_pointer;
            last_value->tail = data_pointer;
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
            auto iterator = hash_table.emplace(key, Value(data_pointer, data_pointer));
            // If the key already has been inserted, iterator.second = false
            if (!iterator.second) {
                // If the key already exists, get the value of the hash table
                // and make an append in the linked list
                auto casted_tail = reinterpret_cast<uint64_t**>(iterator.first->second.tail);
                casted_tail[build_vars->size()] = data_pointer;
                iterator.first->second.tail = data_pointer;
            }
            // Update last_key and last value
            last_value = &iterator.first->second;
            last_key.start = key.start;
        }
    }
}


template<size_t N>
void Join<N>::prepare_chunks_for_new_partition() {
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
    data_chunk = new uint64_t[(build_vars->size() + 1) * PPage::SIZE];
    data_chunks_dir.push_back(data_chunk);

    // Size of key row: join var
    key_chunk = new uint64_t[N * PPage::SIZE];
    key_chunks_dir.push_back(key_chunk);
}


template<size_t N>
bool Join<N>::get_next_partition() {
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


template class HashJoin::BGP::Hybrid::Join<2>;
template class HashJoin::BGP::Hybrid::Join<3>;
template class HashJoin::BGP::Hybrid::Join<4>;
