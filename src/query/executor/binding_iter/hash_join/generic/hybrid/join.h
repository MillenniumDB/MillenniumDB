#pragma once

#include <boost/unordered/unordered_flat_map.hpp>
#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/hash_join/generic/base.h"
#include "query/executor/binding_iter/hash_join/materialize_iter.h"
#include "query/executor/binding_iter/hash_join/value.h"

namespace HashJoin { namespace Generic { namespace Hybrid {
class Join : public BindingIter {
public:
    Join(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs,
        std::vector<VarId>&& join_vars,
        std::vector<VarId>&& lhs_vars,
        std::vector<VarId>&& rhs_vars
    );

    ~Join();

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;

private:
    static constexpr size_t MAX_HASH_TABLE_SIZE = 2000;

    uint64_t depth = 3;

    Binding* parent_binding;
    std::unique_ptr<Binding> lhs_binding;
    std::unique_ptr<Binding> rhs_binding;
    Binding* build_binding;
    Binding* probe_binding;

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

    size_t N;

    std::vector<VarId> all_lhs_vars;
    std::vector<VarId> all_rhs_vars;

    // Partitions
    uint64_t current_partition = 0;

    std::vector<
        std::pair<std::unique_ptr<HashJoin::MaterializeIter>, std::unique_ptr<HashJoin::MaterializeIter>>>
        partitions;

    // Key
    uint64_t* key_chunk; // Chunk
    size_t key_chunk_index; // Chunk index
    std::vector<uint64_t*> key_chunks_dir; // Directory

    // Data
    uint64_t* data_chunk; // Chunk
    size_t data_chunk_index; // Chunk index
    std::vector<uint64_t*> data_chunks_dir; // Directory

    // Hash table
    boost::unordered_flat_map<HashJoin::Generic::Key, HashJoin::Value, HashJoin::Generic::Hasher> hash_table;

    // probe key: Avoid to ask for an uint64 array in each next call
    uint64_t* pk_start;
    uint64_t* last_pk_start;

    HashJoin::Generic::Key probe_key;

    bool build_0_partition(); // True if all build is in memory hash table
    void build_hash_table();

    // Clear to set new partitions
    void prepare_chunks_for_new_partition();

    bool get_next_partition();
};

}}} // namespace HashJoin::Generic::Hybrid
