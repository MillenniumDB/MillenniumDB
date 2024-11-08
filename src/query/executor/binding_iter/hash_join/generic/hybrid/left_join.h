#pragma once

#include <memory>
#include <vector>
#include <boost/unordered/unordered_flat_map.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/hash_join/value.h"
#include "query/executor/binding_iter/hash_join/materialize_iter.h"
#include "query/executor/binding_iter/hash_join/generic/base.h"

namespace HashJoin { namespace Generic { namespace Hybrid {
class LeftJoin : public BindingIter {
public:
    LeftJoin(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs,
        std::vector<VarId>&&         join_vars,
        std::vector<VarId>&&         lhs_vars,
        std::vector<VarId>&&         rhs_vars
    );

    ~LeftJoin();

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    // Results counter: to statistics
    uint64_t found_nulls = 0;
    uint64_t found_not_nulls = 0;

    std::unique_ptr<BindingIter> original_lhs;
    std::unique_ptr<BindingIter> original_rhs;

private:
    static constexpr size_t MAX_HASH_TABLE_SIZE = 2000;

    uint64_t depth = 3;

    BindingIter* lhs;
    BindingIter* rhs;

    Binding* parent_binding;
    std::unique_ptr<Binding> lhs_binding;
    std::unique_ptr<Binding> rhs_binding;

    // When a key of Probe is found in hash table, the algorithm enters in
    // a 'enumerating state' (enumerating row != nullptr),
    // it means that in each next call a row stored in the
    // hash table with the key of the current probe row will be returned.

    uint64_t* enumerating_rows;

    std::vector<VarId> join_vars;
    std::vector<VarId> lhs_vars;
    std::vector<VarId> rhs_vars;

    size_t N;

    std::vector<VarId> all_lhs_vars;
    std::vector<VarId> all_rhs_vars;


    // Partitions
    uint64_t current_partition = 0;

    std::vector<std::pair<std::unique_ptr<HashJoin::MaterializeIter>,
                          std::unique_ptr<HashJoin::MaterializeIter>>> partitions;

    // Key
    uint64_t* key_chunk;                   // Chunk
    size_t key_chunk_index;                // Chunk index
    std::vector<uint64_t*> key_chunks_dir; // Directory

    // Data
    uint64_t* data_chunk;                   // Chunk
    size_t data_chunk_index;                // Chunk index
    std::vector<uint64_t*> data_chunks_dir; // Directory

    // Hash table
    boost::unordered_flat_map<HashJoin::Generic::Key,
                                   HashJoin::Value,
                                   HashJoin::Generic::Hasher> hash_table;

    // probe key: Avoid to ask for an uint64 array in each next call
    uint64_t* pk_start;

    HashJoin::Generic::Key probe_key;

    bool build_0_partition(); // True if all build is in memory hash table
    void build_hash_table();

    // Clear to set new partitions
    void prepare_chunks_for_new_partition();

    bool get_next_partition();
};
}}}
