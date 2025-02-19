#pragma once

#include <memory>
#include <queue>

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include "misc/arena.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/executor/binding_iter/paths/shortest_k/trails/search_state.h"
#include "query/parser/paths/automaton/rpq_automaton.h"

namespace Paths { namespace ShortestKTrails {

class BFSEnum : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId path_var;
    Id start;
    VarId end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;
    uint_fast32_t K; // we suppose K >= 2

    // Attributes determined in begin
    Binding* parent_binding;

    // struct with all simple paths
    Arena<PathState> visited;

    // Queue for BFS
    std::queue<SearchState> open;

    // Iterator for current node expansion
    std::unique_ptr<EdgeIter> iter;

    // The index of the transition being currently explored
    uint_fast32_t current_transition;

    boost::unordered_map<uint64_t, std::vector<const PathState*>> solutions;

    uint_fast32_t enumerating_result_i;

    uint_fast32_t enumerating_result_last;

    std::vector<const PathState*>* reached_final_states;

    // save the node id of the reached path solution
    boost::unordered_set<uint64_t> pending_finals;

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    BFSEnum(
        VarId path_var,
        Id start,
        VarId end,
        RPQ_DFA automaton,
        std::unique_ptr<IndexProvider> provider,
        uint_fast32_t K
    ) :
        path_var(path_var),
        start(start),
        end(end),
        automaton(automaton),
        provider(std::move(provider)),
        K(K)
    { }

    std::vector<const PathState*>* expand_neighbors(const SearchState& current_state);

    void accept_visitor(BindingIterVisitor& visitor) override;

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override
    {
        parent_binding->add(end, ObjectId::get_null());
        parent_binding->add(path_var, ObjectId::get_null());
    }

    // Set iterator for current node + transition
    inline void set_iter(const SearchState& s)
    {
        // Get current transition object from automaton
        auto& transition = automaton.from_to_connections[s.automaton_state][current_transition];

        // Get iterator from custom index
        iter = provider->get_iter(transition.type_id.id, transition.inverse, s.path_state->node_id.id);
        idx_searches++;
    }
};
}} // namespace Paths::ShortestKTrails
