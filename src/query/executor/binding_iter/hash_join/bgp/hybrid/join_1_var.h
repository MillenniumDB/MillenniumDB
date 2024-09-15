#pragma once

#include <memory>
#include <vector>
#include <boost/unordered/unordered_flat_map.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/hash_join/value.h"
#include "query/executor/binding_iter/hash_join/materialize_iter.h"
#include "query/executor/binding_iter/hash_join/bgp/base.h"

namespace HashJoin { namespace BGP { namespace Hybrid {

class Join1Var : public BindingIter {
public:
    Join1Var(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs,
        std::vector<VarId>&&         join_vars,
        std::vector<VarId>&&         lhs_vars,
        std::vector<VarId>&&         rhs_vars
    );

    ~Join1Var();

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;

private:
    static constexpr size_t MAX_HASH_TABLE_SIZE = 100000;

    uint64_t depth = 3;

    Binding* parent_binding;

    // Pointers to store build and probe relations
    BindingIter* build;
    BindingIter* probe;
    std::vector<VarId>* build_vars;
    std::vector<VarId>* probe_vars;

    uint64_t* enumerating_rows;

    std::vector<VarId> join_vars;
    std::vector<VarId> lhs_vars;
    std::vector<VarId> rhs_vars;

    std::vector<VarId> all_lhs_vars;
    std::vector<VarId> all_rhs_vars;

    // Partitions
    uint64_t current_partition = 0;

    std::vector<std::pair<std::unique_ptr<HashJoin::MaterializeIter>,
                          std::unique_ptr<HashJoin::MaterializeIter>>> partitions;


    // Data
    uint64_t* data_chunk;                   // Chunk
    size_t data_chunk_index;                // Chunk index
    std::vector<uint64_t*> data_chunks_dir; // Directory

    // Hash table
    boost::unordered_flat_map<ObjectId,
                              HashJoin::Value,
                              HashJoin::BGP::ObjectIdHasher> hash_table;

    bool build_0_partition(); // True if all build is in memory hash table
    void build_hash_table();

    // Clear to set new partitions
    void prepare_chunks_for_new_partition();

    bool get_next_partition();
};

}}}