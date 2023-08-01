#include "join.h"

#include <cassert>

#include "storage/page.h"

using namespace std;
using namespace HashJoin;
using namespace HashJoin::BGP;
using namespace HashJoin::BGP::InMemory;

template<std::size_t N>
Join<N>::Join(
    unique_ptr<BindingIter> _build_rel,
    unique_ptr<BindingIter> _probe_rel,
    vector<VarId>&&           _join_vars,
    vector<VarId>&&           _build_vars,
    vector<VarId>&&           _probe_vars
) :
    probe_rel         (std::move(_probe_rel)),
    build_rel         (std::move(_build_rel)),
    join_vars         (std::move(_join_vars)),
    build_vars        (std::move(_build_vars)),
    probe_vars        (std::move(_probe_vars)),
    probe_key         (Key<N>(pk_start)),
    last_probe_key    (Key<N>(last_pk_start))
{
    // Size of data row:  build attr  + 1 (one space to reserve next pointer)
    data_chunk = new uint64_t[(build_vars.size() + 1) * Page::MDB_PAGE_SIZE];
    // Add to directory
    data_chunks_dir.push_back(data_chunk);
    // Set chunk index in 0
    data_chunk_index = 0;

    // Size of key row: join var
    key_chunk = new uint64_t[N * Page::MDB_PAGE_SIZE];
    // Add to directory
    key_chunks_dir.push_back(key_chunk);
    // Set chunk index in 0
    key_chunk_index = 0;

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
void Join<N>::begin(Binding& _parent_binding) {
    // set hash join in start state, always must be non enumerating_row
    enumerating_rows = nullptr;

    // Set dummy last_probe key for avoid seg fault in first next iteration
    for (size_t i = 0; i < N; i++) {
        last_probe_key.start[i] = ObjectId::MASK_NOT_FOUND;
    }

    this->parent_binding = &_parent_binding;
    build_rel->begin(_parent_binding);
    probe_rel->begin(_parent_binding);

    build_hash_table();
}


template<std::size_t N>
bool Join<N>::next() {
    while (true) {
        // If enumerating_rows != nullptr, then a row must be returned
        if (enumerating_rows != nullptr) {
            // Set binding from current enumerating row
            for (uint_fast32_t i = 0; i < build_vars.size(); i++) {
                parent_binding->add(build_vars[i], ObjectId(enumerating_rows[i]));
            }
            // Update enumerating row to next value
            enumerating_rows = reinterpret_cast<uint64_t**>(enumerating_rows)[build_vars.size()];
            found++;
            return true;
        }
        else {
            // If enumerating rows is nullptr ask for the next probe relation row
            // if has an entry in the hash table
            if (probe_rel->next()) {
                // Build key of probe row
                for (size_t i = 0; i < N; i++) {
                    probe_key.start[i] = (*parent_binding)[join_vars[i]].id;
                }

                // Check if probe key is equal to last key returned
                // to avoid an ask to hash table
                if (probe_key == last_probe_key) {
                    enumerating_rows = last_enumerating_rows;
                    continue;
                }
                // Asks for an entry in the hash table
                auto iterator = hash_table.find(probe_key);
                // If has an entry, then update enumerating rows
                if (iterator != hash_table.end()) {
                    enumerating_rows = (iterator->second).head;
                    // Store enumerating rows and last key for
                    // try to avoid asking hash table in the following iterations
                    last_enumerating_rows = (iterator->second).head;
                    for (size_t i = 0; i < N; i++) {
                        last_probe_key.start[i] = probe_key.start[i];
                    }
                }
            }
            else {
                return false;
            }
        }
    }
}


template<std::size_t N>
void Join<N>::reset() {
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

    // Set default probe key and last probe key
    for (size_t i = 0; i < N; i++) {
        probe_key.start[i] = ObjectId::MASK_NOT_FOUND;
        last_probe_key.start[i] = ObjectId::MASK_NOT_FOUND;
    }

    // Spread reset to children
    build_rel->reset();
    probe_rel->reset();

    build_hash_table();
}


template<std::size_t N>
void Join<N>::assign_nulls() {
    build_rel->assign_nulls();
    probe_rel->assign_nulls();
}


template<std::size_t N>
void Join<N>::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Join(found: " << found << "\n";
    build_rel->analyze(os, indent + 2);
    os << ",\n";
    probe_rel->analyze(os, indent + 2);
    os << "\n";
    os << std::string(indent, ' ');
    os << ")";
}


template<std::size_t N>
void Join<N>::build_hash_table() {
    auto data_tuple_size = build_vars.size() + 1;

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

    while (build_rel->next()) {
        // Get start index to store key and data
        auto start_key_index = key_chunk_index * N;
        auto start_data_index = data_chunk_index * data_tuple_size;

        key.start = &key_chunk[start_key_index];
        // Store key
        for (size_t i = 0; i < N; i++) {
            key_chunk[start_key_index + i] = (*parent_binding)[join_vars[i]].id;
        }
        // Store data
        for (size_t i = 0; i < build_vars.size(); i++) {
            data_chunk[start_data_index + i] =  (*parent_binding)[build_vars[i]].id;
        }
        // Set last data value as a null pointer
        auto casted_chunk = reinterpret_cast<uint64_t**>(data_chunk);
        casted_chunk[start_data_index + build_vars.size()] = nullptr;

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
            casted_tail[build_vars.size()] = data_pointer;
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
                casted_tail[build_vars.size()] = data_pointer;
                iterator.first->second.tail = data_pointer;
            }
            // Update last_key and last value
            last_value = &iterator.first->second;
            last_key.start = key.start;
        }
    }
}


template class HashJoin::BGP::InMemory::Join<2>;
template class HashJoin::BGP::InMemory::Join<3>;
template class HashJoin::BGP::InMemory::Join<4>;
