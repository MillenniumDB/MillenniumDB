#pragma once

#include <memory>
#include <array>
#include <vector>
#include <boost/unordered/unordered_flat_map.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/hash_join/value.h"
#include "query/executor/binding_iter/hash_join/materialize_iter.h"
#include "query/executor/binding_iter/hash_join/bgp/base.h"

namespace HashJoin { namespace BGP { namespace Hybrid {
template<std::size_t N> class Join : public BindingIter {
public:
    Join(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs,
        std::vector<VarId>&&           join_vars,
        std::vector<VarId>&&           lhs_vars,
        std::vector<VarId>&&           rhs_vars
    );

    ~Join();

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    static constexpr size_t MAX_HASH_TABLE_SIZE = 100000;

    uint64_t depth = 3;

    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;

    Binding* parent_binding;

    // Pointers to store build and probe relations
    BindingIter* build;
    BindingIter* probe;
    std::vector<VarId>* build_vars;
    std::vector<VarId>* probe_vars;

    // When a key of Probe is found in hash table, the algorithm enters in
    // a 'enumerating state' (enumerating row != nullptr),
    // it means that in each next call a row stored in the
    // hash table with the key of the current probe row will be returned.

    uint64_t* enumerating_rows;

    std::vector<VarId> join_vars;
    std::vector<VarId> lhs_vars;
    std::vector<VarId> rhs_vars;

    std::vector<VarId> all_lhs_vars;
    std::vector<VarId> all_rhs_vars;

    // Partitions
    uint64_t current_partition = 0;
    uint64_t current_partition_stack = 0;

    std::vector<std::pair<std::unique_ptr<HashJoin::MaterializeIter>,
                          std::unique_ptr<HashJoin::MaterializeIter>>> partitions;

    /*
    Keys and data (non join vars of build relation) will be stored in a array
    in memory. The structure of both is described at following:
        1- Create a chunk to store a certain number of keys or data.
        2- Store the pointer to this chunk in chunk_dir
        3- Key/Data will be stored in a chunk (array of uint64). chunk_index
          indicates the place where the new element will be placed
        4- When chunk is full, a new chunk will be created and a pointer to this
           will be stored in chunks_dir
    */
    // Key
    uint64_t* key_chunk;                   // Chunk
    size_t key_chunk_index;                // Chunk index
    std::vector<uint64_t*> key_chunks_dir; // Directory

    // Data
    uint64_t* data_chunk;                   // Chunk
    size_t data_chunk_index;                // Chunk index
    std::vector<uint64_t*> data_chunks_dir; // Directory


    // Hash table
    boost::unordered_flat_map<HashJoin::BGP::Key<N>,
                                   HashJoin::Value,
                                   HashJoin::BGP::Hasher<N>> hash_table;


    // probe key: Avoid to ask for an uint64 array in each next call
    uint64_t pk_start[N];
    uint64_t last_pk_start[N];

    HashJoin::BGP::Key<N> probe_key;

    // Results counter: to statistics
    uint64_t found = 0;

    bool build_0_partition(); // True if all build is in memory hash table
    void build_hash_table();

    // Clear to set new partitions
    void prepare_chunks_for_new_partition();

    bool get_next_partition();
};
}}}
