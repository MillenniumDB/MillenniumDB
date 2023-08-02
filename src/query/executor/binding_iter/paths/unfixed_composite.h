#pragma once

#include <memory>

#include "query/executor/binding_iter.h"
#include "query/id.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/parser/paths/automaton/rpq_automaton.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths {
/*
UnfixedComposite  start by looking at the transitions from the initial state to find
start nodes that make sense.
It then does a search on that node using a nested path enum.
*/
class UnfixedComposite : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId         path_var;
    VarId         start;
    VarId         end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // Attributes determined in begin
    Binding* parent_binding;

    std::unique_ptr<BindingIter> path_enum;

    // Only used to remember the starting nodes, in order to not repeat them
    robin_hood::unordered_set<uint64_t> visited;

    uint64_t last_start = ObjectId::get_null().id;

    // iterator for starting nodes
    std::unique_ptr<EdgeIter> iter;

    // index of the start transition for the current iter
    uint32_t current_start_transition = 0;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t idx_searches = 0;

    const std::vector<RPQ_DFA::Transition>& start_transitions;

    bool set_next_starting_node();

public:
    UnfixedComposite(
        VarId                          path_var,
        VarId                          start,
        VarId                          end,
        RPQ_DFA                        _automaton,
        std::unique_ptr<IndexProvider> provider,
        std::unique_ptr<BindingIter> path_enum
    ) :
        path_var          (path_var),
        start             (start),
        end               (end),
        automaton         (_automaton),
        provider          (std::move(provider)),
        path_enum         (std::move(path_enum)),
        start_transitions (automaton.from_to_connections[0]) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    void reset() override;
    bool next() override;

    void assign_nulls() override {
        parent_binding->add(start, ObjectId::get_null());
        parent_binding->add(end, ObjectId::get_null());
        parent_binding->add(path_var, ObjectId::get_null());
    }
};
} // namespace Paths
