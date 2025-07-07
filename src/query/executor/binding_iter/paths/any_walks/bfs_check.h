#pragma once

#include <memory>
#include <queue>

#include <boost/unordered/unordered_node_set.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/any_walks/search_state.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/parser/paths/automaton/rpq_automaton.h"

namespace Paths { namespace Any {

/*
BFSCheck checks if there's a path between two fixed nodes, using BFS.
*/
class BFSCheck : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId path_var;
    Id start;
    Id end;
    RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // where the results will be written, determined in begin()
    Binding* parent_binding;

    // if `end` is an ObjectId, this has the same value
    // if `end` is a variable, this has its the value in the binding
    // its value is setted in begin() and reset()
    ObjectId end_object_id;

    /// Remembers which states were explored. A structure with pointer stability is required
    boost::unordered_node_set<DirectionalSearchState, std::hash<DirectionalSearchState>> visited;

    // Queues for BFS. Pointers point to the states in visited
    std::queue<const DirectionalSearchState*> open;

    // Number of states in Open for each traversal direction
    size_t forward_states = 0;
    size_t backward_states = 0;

    // Iterator for current node expansion
    std::unique_ptr<EdgeIter> iter;

    // true in the first call of next() and after a reset()
    bool first_next = true;

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

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;

    void assign_nulls() override
    {
        parent_binding->add(path_var, ObjectId::get_null());
    }

    // Set iterator for current node + transition
    inline void set_iter(const RPQ_DFA::Transition& transition, const DirectionalSearchState& current_state)
    {
        // Get iterator from custom index
        iter = provider->get_iter(transition.type_id.id, transition.inverse, current_state.node_id.id);
        idx_searches++;
    }

    // Merge converging paths from bidirectional search
    const DirectionalSearchState* merge_directions(
        const DirectionalSearchState* forward_state,
        const DirectionalSearchState* backward_state,
        bool merge_inverse,
        ObjectId merge_type
    );
};
}} // namespace Paths::Any
