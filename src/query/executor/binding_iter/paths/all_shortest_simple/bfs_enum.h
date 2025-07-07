#pragma once

#include <memory>
#include <queue>

#include <boost/unordered/unordered_flat_map.hpp>

#include "misc/arena.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/all_shortest_simple/search_state.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/parser/paths/automaton/rpq_automaton.h"

namespace Paths { namespace AllShortestSimple {

/*
AllShortestSimple::BFSEnum returns all the shortest Simple Paths to all
reachable nodes from a fixed starting node.
*/
template<bool CYCLIC>
class BFSEnum : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId path_var;
    Id start;
    VarId end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

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

    // true in the first call of next() and after a reset()
    bool first_next = true;

    // Map of nodes reached with a final state + their optimal distance: NodeId -> OptimalDistance
    boost::unordered_flat_map<uint64_t, uint32_t> optimal_distances;

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    BFSEnum(VarId path_var, Id start, VarId end, RPQ_DFA automaton, std::unique_ptr<IndexProvider> provider) :
        path_var(path_var),
        start(start),
        end(end),
        automaton(automaton),
        provider(std::move(provider))
    { }

    // Explore neighbors searching for a solution.
    // returns a pointer to the object added to visited when a solution is found
    // or nullptr when there are no more results
    const PathState* expand_neighbors(const SearchState& current_state);

    void print(std::ostream& os, int indent, bool stats) const override;

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
}} // namespace Paths::AllShortestSimple
