#pragma once

#include <memory>
#include <vector>
#include <boost/unordered/unordered_flat_set.hpp>

#include "query/var_id.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/hash_join/generic/base.h"


namespace HashJoin { namespace Generic { namespace InMemory {
class AntiJoin : public BindingIter {
public:
    AntiJoin(
        std::unique_ptr<BindingIter> lhs_rel,
        std::unique_ptr<BindingIter> rhs_rel,
        std::vector<VarId>&&           join_vars,
        std::vector<VarId>&&           lhs_vars,
        std::vector<VarId>&&           rhs_vars
    );

    ~AntiJoin();
    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:

    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;

    std::vector<VarId> join_vars;
    std::vector<VarId> lhs_vars;
    std::vector<VarId> rhs_vars;

    const size_t N;
    std::unique_ptr<Binding> lhs_binding;
    std::unique_ptr<Binding> rhs_binding;
    Binding* parent_binding;

    // Key
    uint64_t* key_chunk; // Chunk
    std::vector<uint64_t*> key_chunks_dir; // Directory
    size_t key_chunk_index; // Chunk index


    // Hash table
    boost::unordered_flat_set<Key, Hasher> hash_table;
    void build_hash_table();

    // probe key: Avoid to ask for an uint64 array in each next call
    uint64_t* pk_start;
    Key probe_key;

    // Results counter: to statistics
    uint64_t found = 0;
};
}}}
