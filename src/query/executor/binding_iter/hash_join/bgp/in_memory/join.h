#pragma once

#include <memory>
#include <vector>
#include <boost/unordered/unordered_flat_map.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/hash_join/value.h"
#include "query/executor/binding_iter/hash_join/bgp/base.h"

namespace HashJoin { namespace BGP { namespace InMemory {

template<std::size_t N> class Join : public BindingIter {
public:
    Join(
        std::unique_ptr<BindingIter> build_rel,
        std::unique_ptr<BindingIter> probe_rel,
        std::vector<VarId>&&         join_vars,
        std::vector<VarId>&&         build_vars,
        std::vector<VarId>&&         probe_vars);

    ~Join();

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    // Optimizer decide which relation is probe and which is build
    std::unique_ptr<BindingIter> probe_rel;
    std::unique_ptr<BindingIter> build_rel;

private:
    std::vector<VarId> join_vars;
    std::vector<VarId> build_vars;
    std::vector<VarId> probe_vars;

    Binding* parent_binding;

    // When a key of Probe is found in hash table, the algorithm enters in
    // a 'enumerating state' (enumerating row != nullptr),
    // it means that in each next call a row stored in the
    // hash table with the key of the current probe row will be returned.

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
    boost::unordered_flat_map<HashJoin::BGP::Key<N>,
                                   HashJoin::Value,
                                   HashJoin::BGP::Hasher<N>> hash_table;
    void build_hash_table();

    // probe key: Avoid to ask for an uint64 array in each next call
    uint64_t pk_start [N];
    uint64_t last_pk_start[N];
    Key<N> probe_key;
    Key<N> last_probe_key;
};
}}}
