#pragma once

#include <memory>
#include <vector>
#include <boost/unordered/unordered_flat_set.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/hash_join/value.h"
#include "query/executor/binding_iter/hash_join/materialize_iter.h"
#include "query/executor/binding_iter/hash_join/generic/base.h"
#include "third_party/robin_hood/robin_hood.h"

namespace HashJoin { namespace Generic { namespace Hybrid {
class SemiJoin : public BindingIter {
public:
    SemiJoin(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs,
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

    std::vector<VarId> join_vars;
    std::vector<VarId> lhs_vars;
    std::vector<VarId> rhs_vars;

    size_t N;

    std::vector<VarId> all_lhs_vars;

    // Partitions
    uint64_t current_partition = 0;

    std::vector<std::pair<std::unique_ptr<HashJoin::MaterializeIter>,
                          std::unique_ptr<HashJoin::MaterializeIter>>> partitions;

    // Key
    uint64_t* key_chunk;                   // Chunk
    size_t key_chunk_index;                // Chunk index
    std::vector<uint64_t*> key_chunks_dir; // Directory

    // Hash table
    boost::unordered_flat_set<HashJoin::Generic::Key,
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
