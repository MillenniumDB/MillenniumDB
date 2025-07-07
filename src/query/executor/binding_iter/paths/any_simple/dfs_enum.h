#pragma once

#include <memory>
#include <set>
#include <stack>

#include <boost/unordered/unordered_flat_set.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/any_simple/search_state.h"
#include "query/parser/paths/automaton/rpq_automaton.h"

namespace Paths { namespace AnySimple {

/*
DFSEnum returns a single Simple Path to all
reachable nodes from a starting node, using DFS.
*/
template<bool CYCLIC>
class DFSEnum : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId path_var;
    Id start;
    VarId end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // Attributes determined in begin
    Binding* parent_binding;
    ObjectId current_start;
    bool first_next = true;

    // Stack for search states + paths
    std::stack<SearchStateDFS> open;

    // Set of nodes reached with a final state
    boost::unordered_flat_set<uint64_t> reached_final;

    std::set<uint64_t> current_state_nodes;

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    DFSEnum(VarId path_var, Id start, VarId end, RPQ_DFA automaton, std::unique_ptr<IndexProvider> provider) :
        path_var(path_var),
        start(start),
        end(end),
        automaton(automaton),
        provider(std::move(provider))
    { }

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    void _reset() override;
    bool _next() override;

    // Get next state of interest
    SearchStateDFS* expand_neighbors(SearchStateDFS& current_state);

    void assign_nulls() override
    {
        parent_binding->add(end, ObjectId::get_null());
        parent_binding->add(path_var, ObjectId::get_null());
    }

    // Set iterator for current node + transition
    void set_iter(SearchStateDFS& s)
    {
        // Get current transition object from automaton
        auto& transition = automaton.from_to_connections[s.automaton_state][s.transition];

        // Get iterator from custom index
        s.iter = provider->get_iter(transition.type_id.id, transition.inverse, s.node_id.id);
        idx_searches++;
    }
};
}} // namespace Paths::AnySimple
