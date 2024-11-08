#pragma once

#include <memory>
#include <queue>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/all_shortest_trails/search_state.h"
#include "query/parser/paths/automaton/rpq_automaton.h"
#include "misc/arena.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AllShortestTrails {

/*
BFSEnum returns all the shortest Trails to all reachable nodes from a starting node.
*/
class BFSEnum : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId         path_var;
    Id            start;
    VarId         end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // Attributes determined in begin
    Binding* parent_binding;

    // struct with all trails
    Arena<PathState> visited;

    // Queue of search states
    std::queue<SearchState> open;

    // Iterator for current node expansion
    std::unique_ptr<EdgeIter> iter;

    // The index of the transition being currently explored
    uint32_t current_transition;

    bool first_next = true;

    // Map of nodes reached with a final state + their optimal distance: NodeId -> OptimalDistance
    robin_hood::unordered_map<uint64_t, uint32_t> optimal_distances;

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    BFSEnum(
        VarId                          path_var,
        Id                             start,
        VarId                          end,
        RPQ_DFA                        automaton,
        std::unique_ptr<IndexProvider> provider
    ) :
        path_var      (path_var),
        start         (start),
        end           (end),
        automaton     (automaton),
        provider      (std::move(provider)) { }


    // Expand neighbors from current state
    const SearchState* expand_neighbors(const SearchState& current_state);

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    void _reset() override;
    bool _next() override;

    void assign_nulls() override {
        parent_binding->add(end, ObjectId::get_null());
        parent_binding->add(path_var, ObjectId::get_null());
    }

    // Set iterator for current node + transition
    inline void set_iter(const SearchState& s) {
        // Get current transition object from automaton
        auto& transition = automaton.from_to_connections[s.automaton_state][current_transition];

        // Get iterator from custom index
        iter = provider->get_iter(transition.type_id.id, transition.inverse, s.path_state->node_id.id);
        idx_searches++;
    }
};
}} // namespace Paths::AllShortestTrails
