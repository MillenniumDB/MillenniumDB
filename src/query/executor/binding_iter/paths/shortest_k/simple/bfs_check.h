#pragma once

#include <memory>
#include <queue>

#include "misc/arena.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/executor/binding_iter/paths/shortest_k/simple/search_state.h"
#include "query/parser/paths/automaton/rpq_automaton.h"

namespace Paths { namespace ShortestKSimple {

/*
AllShortestSimple::BFSCheck returns all the simple paths between two fixed nodes.
If CYCLIC template is true, the path may have the same node for the start and end node
*/
template<bool CYCLIC>
class BFSCheck : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId path_var;
    Id start;
    Id end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;
    uint_fast32_t K; // we suppose K >= 2

    // where the results will be written, determined in begin()
    Binding* parent_binding;

    // if `end` is an ObjectId, this has the same value
    // if `end` is a variable, this has its the value in the binding
    // its value is setted in begin() and reset()
    ObjectId end_object_id;

    // struct with all simple paths
    Arena<PathState> visited;

    // Queue for BFS
    std::queue<SearchState> open;

    // Iterator for current node expansion
    std::unique_ptr<EdgeIter> iter;

    // The index of the transition being currently explored
    uint_fast32_t current_transition;

    std::vector<const PathState*> solutions;

    // Used to return when an empty path is a solution
    bool first_state_is_solution;

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    BFSCheck(
        VarId path_var,
        Id start,
        Id end,
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

    bool expand_neighbors(const SearchState& current_state);

    void print(std::ostream& os, int indent, bool stats) const override;

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override
    {
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

private:
    void expand_first_state();
};
}} // namespace Paths::ShortestKSimple
