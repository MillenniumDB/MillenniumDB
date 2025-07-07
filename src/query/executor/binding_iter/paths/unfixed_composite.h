#pragma once

#include <memory>

#include <boost/unordered/unordered_flat_set.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/parser/paths/automaton/rpq_automaton.h"

namespace Paths {
/*
UnfixedComposite  start by looking at the transitions from the initial state to find
start nodes that make sense.
It then does a search on that node using a nested path enum.
*/
class UnfixedComposite : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId path_var;
    VarId start;
    VarId end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // Attributes determined in begin
    Binding* parent_binding;

    // Only used to remember the starting nodes, in order to not repeat them
    boost::unordered_flat_set<uint64_t> visited;

    uint64_t last_start = ObjectId::get_null().id;

    // iterator for starting nodes
    std::unique_ptr<EdgeIter> iter;

    // index of the start transition for the current iter
    uint32_t current_start_transition = 0;

    const std::vector<RPQ_DFA::Transition>& start_transitions;

    bool set_next_starting_node();

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    std::unique_ptr<BindingIter> child_iter;

    UnfixedComposite(
        VarId path_var,
        VarId start,
        VarId end,
        RPQ_DFA _automaton,
        std::unique_ptr<IndexProvider> provider,
        std::unique_ptr<BindingIter> child_iter
    ) :
        path_var(path_var),
        start(start),
        end(end),
        automaton(_automaton),
        provider(std::move(provider)),
        start_transitions(automaton.from_to_connections[0]),
        child_iter(std::move(child_iter))
    { }

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    void _reset() override;
    bool _next() override;

    void assign_nulls() override
    {
        parent_binding->add(start, ObjectId::get_null());
        parent_binding->add(end, ObjectId::get_null());
        parent_binding->add(path_var, ObjectId::get_null());
    }
};
} // namespace Paths
