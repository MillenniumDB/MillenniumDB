#pragma once

#include <memory>
#include <queue>

#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_flat_set.hpp>
#include <boost/unordered/unordered_node_map.hpp>
#include <boost/unordered/unordered_node_set.hpp>

#include "misc/arena.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/executor/binding_iter/paths/shortest_k/walks/search_state.h"
#include "query/parser/paths/automaton/rpq_automaton.h"

namespace Paths { namespace ShortestKWalks {

class BFSEnum : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId path_var;
    Id start;
    VarId end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;
    size_t K;

    // where the results will be written, determined in begin()
    Binding* parent_binding;

    // Remembers which states were explored. A structure with pointer stability is required
    boost::unordered_node_set<SearchState, std::hash<SearchState>> visited;

    // Queue for BFS. Pointers point to the states in visited
    std::queue<const SearchState*> open;

    // Iterator for current node expansion
    std::unique_ptr<EdgeIter> iter;

    // The index of the transition being currently explored
    size_t current_transition;

    // Reached {NodeId, State} -> PathCount
    // Used to ensure the exploration (expand_neighbors) terminates
    // without an infinite loop
    boost::unordered_flat_map<NodeState, size_t, NodeStateHasher> path_counts;

    // NodeId -> {paths, count}
    boost::unordered_node_map<uint64_t, Solution> solutions;

    Arena<IterTransition> iter_arena;

    // save the NodeId of the reached path solution
    boost::unordered_flat_set<uint64_t> pending_finals;

    // counter for results for the current reached_final_states
    // so it does not exceed K solutions
    size_t returned_solutions;

    // nullpointer or points to some element (paths) in solutions
    Solution* current_solution;

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    BFSEnum(
        VarId path_var,
        Id start,
        VarId end,
        RPQ_DFA automaton,
        std::unique_ptr<IndexProvider> provider,
        uint64_t K
    ) :
        path_var(path_var),
        start(start),
        end(end),
        automaton(automaton),
        provider(std::move(provider)),
        K(K)
    { }

    // Explore neighbors searching for a solution.
    // Returns nullptr when the exploration ends without finding a result.
    Solution* expand_neighbors(const SearchState& current_state);

    void print(std::ostream& os, int indent, bool stats) const override;

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override
    {
        parent_binding->add(path_var, ObjectId::get_null());
        parent_binding->add(end, ObjectId::get_null());
    }

    // Set iterator for current node + transition
    inline void set_iter(const SearchState& current_state)
    {
        // Get current transition object from automaton
        auto& transition = automaton.from_to_connections[current_state.automaton_state][current_transition];

        // Get iterator from custom index
        iter = provider->get_iter(transition.type_id.id, transition.inverse, current_state.node_id.id);
        idx_searches++;
    }

private:
    void expand_first_state();
};
}} // namespace Paths::ShortestKWalks
