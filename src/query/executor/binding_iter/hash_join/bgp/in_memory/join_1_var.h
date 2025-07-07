#pragma once

#include <memory>
#include <vector>
#include <boost/unordered/unordered_flat_map.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/hash_join/value.h"
#include "query/executor/binding_iter/hash_join/bgp/base.h"

namespace HashJoin { namespace BGP { namespace InMemory {

class Join1Var : public BindingIter {
public:
    Join1Var(
        std::unique_ptr<BindingIter> build_rel,
        std::unique_ptr<BindingIter> probe_rel,
        VarId                        join_var,
        std::vector<VarId>&&         build_vars,
        std::vector<VarId>&&         probe_vars
    );

    ~Join1Var();

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    // Optimizer decide which relation is probe and which is build
    std::unique_ptr<BindingIter> probe_rel;
    std::unique_ptr<BindingIter> build_rel;

private:
    VarId join_var;

    std::vector<VarId> build_vars;
    std::vector<VarId> probe_vars;

    Binding* parent_binding;

    uint64_t* enumerating_rows;

    // Data
    uint64_t* data_chunk; // Chunk
    std::vector<uint64_t*> data_chunks_dir; // Directory
    size_t data_chunk_index; // Chunk index

    // Hash table
    boost::unordered_flat_map<ObjectId,
                              HashJoin::Value,
                              HashJoin::BGP::ObjectIdHasher> hash_table;
    void build_hash_table();
};
}}}
