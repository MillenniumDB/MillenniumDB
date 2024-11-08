#pragma once

#include <memory>
#include <vector>
#include <boost/unordered/unordered_flat_map.hpp>

#include "query/var_id.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/hash_join/value.h"
#include "query/executor/binding_iter/hash_join/generic/base.h"

namespace HashJoin { namespace Generic { namespace InMemory {

class Join : public BindingIter {
public:
    Join(
        std::unique_ptr<BindingIter> lhs_rel,
        std::unique_ptr<BindingIter> rhs_rel,
        std::vector<VarId>&&         join_vars,
        std::vector<VarId>&&         lhs_vars,
        std::vector<VarId>&&         rhs_vars
    );

    ~Join();

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    // Optimizer decide which relation is rhs and which is lhs
    std::unique_ptr<BindingIter> rhs;
    std::unique_ptr<BindingIter> lhs;

private:
    std::vector<VarId> join_vars;
    std::vector<VarId> lhs_vars;
    std::vector<VarId> rhs_vars;

    size_t N;

    std::unique_ptr<Binding> lhs_binding;
    std::unique_ptr<Binding> rhs_binding;
    Binding* parent_binding;

    // When a key of Probe is found in hash table, the algorithm enters in
    // a 'enumerating state' (enumerating row != nullptr),
    // it means that in each next call a row stored in the
    // hash table with the key of the current rhs row will be returned.

    uint64_t* enumerating_rows;
    uint64_t* last_enumerating_rows;

    // Key
    uint64_t* key_chunk; // Chunk
    std::vector<uint64_t*> key_chunks_dir; // Directory
    size_t key_chunk_index; // Chunk index

    // Data
    uint64_t* data_chunk; // Chunk
    std::vector<uint64_t*> data_chunks_dir; // Directory

    size_t data_chunk_index; // Chunk index

    // Hash table
    boost::unordered_flat_map<HashJoin::Generic::Key,
                              HashJoin::Value,
                              HashJoin::Generic::Hasher> hash_table;
    void build_hash_table();

    // rhs key: Avoid to ask for an uint64 array in each next call
    uint64_t* pk_start;
    uint64_t* last_pk_start;
    Key rhs_key;
    Key last_rhs_key;
};
}}}
