#pragma once

#include <memory>
#include <queue>

#include <boost/unordered/unordered_node_set.hpp>

#include "misc/arena.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/all_shortest_walks/search_state.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/parser/paths/automaton/rpq_automaton.h"

namespace Paths { namespace AllShortest {

/*
AllShortest::BFSCheck returns all the shortest paths between two fixed nodes.
*/
class BFSCheck : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId path_var;
    Id start;
    Id end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // where the results will be written, determined in begin()
    Binding* parent_binding;

    // if `end` is an ObjectId, this has the same value
    // if `end` is a variable, this has its the value in the binding
    // its value is setted in begin() and reset()
    ObjectId end_object_id;

    // Remembers which states were explored. A structure with pointer stability is required
    boost::unordered_node_set<SearchState, std::hash<SearchState>> visited;

    // Queue for BFS. Pointers point to the states in visited
    std::queue<const SearchState*> open;

    // Iterator for current node expansion
    std::unique_ptr<EdgeIter> iter;

    // The index of the transition being currently explored
    uint_fast32_t current_transition;

    // true in the first call of next() and after a reset()
    bool first_next = true;

    // Once the search reaches a solution we might find many paths
    // and we need to enumerate them one by one.
    // When solution_found != nullptr we are enumerating the paths found.
    const SearchState* solution_found = nullptr;

    // Optimal distance to target node. UINT64_MAX means the node has not been explored yet.
    uint64_t optimal_distance = UINT64_MAX;

    Arena<TransitionLinkedList> iter_arena;

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    BFSCheck(VarId path_var, Id start, Id end, RPQ_DFA automaton, std::unique_ptr<IndexProvider> provider) :
        path_var(path_var),
        start(start),
        end(end),
        automaton(automaton),
        provider(std::move(provider))
    { }

    // Explore neighbors searching for a solution.
    // Returns true if a solution was found. In that case
    // solution_found was setted.
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
        iter = provider->get_iter(transition.type_id.id, transition.inverse, s.node_id.id);
        idx_searches++;
    }
};
}} // namespace Paths::AllShortest
