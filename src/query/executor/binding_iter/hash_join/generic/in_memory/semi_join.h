#pragma once

#include <memory>
#include <vector>
#include <boost/unordered/unordered_flat_set.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/hash_join/generic/base.h"

namespace HashJoin { namespace Generic { namespace InMemory {
class SemiJoin : public BindingIter {
public:
    SemiJoin(
        std::unique_ptr<BindingIter> lhs_rel,
        std::unique_ptr<BindingIter> rhs_rel,
        std::vector<VarId>&&         join_vars,
        std::vector<VarId>&&         lhs_vars,
        std::vector<VarId>&&         rhs_vars
    );

    ~SemiJoin();

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;

private:
    std::vector<VarId> join_vars;
    std::vector<VarId> lhs_vars;
    std::vector<VarId> rhs_vars;

    size_t N;

    std::unique_ptr<Binding> rhs_binding;
    std::unique_ptr<Binding> lhs_binding;
    Binding* parent_binding;

    // Key
    uint64_t* key_chunk; // Chunk
    std::vector<uint64_t*> key_chunks_dir; // Directory
    size_t key_chunk_index; // Chunk index

    boost::unordered_flat_set<Key, Hasher> hash_table;
    void build_hash_table();

    // probe key: Avoid to ask for an uint64 array in each next call
    uint64_t* lhs_key_start;
    Key lhs_key;
};
}}}
