#pragma once

#include <memory>
#include <stack>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/any_walks/search_state.h"
#include "query/parser/paths/automaton/rpq_automaton.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace Any {

/*
DFSCheck checks if there's a path between two fixed nodes, using DFS.
*/
class DFSCheck : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId         path_var;
    Id            start;
    Id            end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // Attributes determined in begin
    Binding* parent_binding;
    ObjectId end_object_id;
    bool first_next = true;

    // Set of visited SearchStates
    robin_hood::unordered_node_set<Paths::Any::SearchState> visited;

    // Stack of DFSSearchStates
    std::stack<DFSSearchState> open;

public:
    // Statistics
    uint_fast32_t idx_searches  = 0;

    DFSCheck(
        VarId                          path_var,
        Id                             start,
        Id                             end,
        RPQ_DFA                        automaton,
        std::unique_ptr<IndexProvider> provider
    ) :
        path_var      (path_var),
        start         (start),
        end           (end),
        automaton     (automaton),
        provider      (std::move(provider)) { }

    // Get next state of interest
    robin_hood::unordered_node_set<Paths::Any::SearchState>::iterator
      expand_neighbors(DFSSearchState& current_state);

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    void _reset() override;
    bool _next() override;

    void assign_nulls() override {
        parent_binding->add(path_var, ObjectId::get_null());
    }

    // Set iterator for current node + transition
    inline void set_iter(DFSSearchState& s) {
        // Get current transition object from automaton
        auto& transition = automaton.from_to_connections[s.state][s.current_transition];

        // Get iterator from custom index
        s.iter = provider->get_iter(transition.type_id.id, transition.inverse, s.node_id.id);
        idx_searches++;
    }
};
}} // namespace Paths::Any
