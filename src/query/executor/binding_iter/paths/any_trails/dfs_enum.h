#pragma once

#include <memory>
#include <set>
#include <stack>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/any_trails/search_state.h"
#include "query/parser/paths/automaton/rpq_automaton.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AnyTrails {

/*
DFSEnum returns a single Trail to all
reachable nodes from a starting node, using DFS.
*/
class DFSEnum : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId         path_var;
    Id            start;
    VarId         end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // Attributes determined in begin
    Binding* parent_binding;
    bool first_next = true;

    // Stack for search states + paths
    std::stack<SearchStateDFS> open;

    // Get next state of interest
    SearchStateDFS* expand_neighbors(SearchStateDFS& current_state);

    // Set iterator for current node + transition
    void set_iter(SearchStateDFS& current_state);

    // Set of nodes reached with a final state
    robin_hood::unordered_set<uint64_t> reached_final;

    std::set<uint64_t> current_state_edges;

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    DFSEnum(
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

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    void _reset() override;
    bool _next() override;

    void assign_nulls() override {
        parent_binding->add(end, ObjectId::get_null());
        parent_binding->add(path_var, ObjectId::get_null());
    }
};
}} // namespace Paths::AnyTrails
