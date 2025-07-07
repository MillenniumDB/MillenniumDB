#pragma once

#include <memory>
#include <set>
#include <stack>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/any_trails/search_state.h"
#include "query/parser/paths/automaton/rpq_automaton.h"

namespace Paths { namespace AnyTrails {

/*
DFSCheck checks if there's a Trail between two fixed nodes, using DFS.
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

    // Stack for search states + paths
    std::stack<SearchStateDFS> open;

    std::set<uint64_t> current_state_edges;

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    DFSCheck(
        VarId                          path_var,
        Id                             start,
        Id                             end,
        RPQ_DFA                        automaton,
        std::unique_ptr<IndexProvider> provider
    ) :
        path_var  (path_var),
        start     (start),
        end       (end),
        automaton (automaton),
        provider  (std::move(provider)) { }

    // Get next state of interest
    SearchStateDFS* expand_neighbors(SearchStateDFS& current_state);

    // Set iterator for current node + transition
    void set_iter(SearchStateDFS& current_state);

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    void _reset() override;
    bool _next() override;

    void assign_nulls() override {
        parent_binding->add(path_var, ObjectId::get_null());
    }
};
}} // namespace Paths::AnyTrails
