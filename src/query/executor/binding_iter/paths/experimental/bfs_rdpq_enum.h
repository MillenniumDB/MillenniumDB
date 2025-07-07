#pragma once

#include <array>
#include <queue>

#include <boost/unordered/unordered_node_set.hpp>

#include "graph_models/object_id.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/any_walks/search_state.h"
#include "query/parser/paths/automaton/rdpq_automaton.h"
#include "storage/index/bplus_tree/bplus_tree.h"

namespace Paths { namespace Any {

/*
BFS_RDPQEnum enumerates paths 'from' or 'to' a specific node, using the BFS algorithm and the DE automaton.
It uses class members to keep track of the current edge & data transitions that are being explored.
*/
class BFS_RDPQEnum : public BindingIter {
public:
    BFS_RDPQEnum(VarId path_var, Id start, VarId end, RDPQAutomaton automaton) :
        path_var(path_var),
        start(start),
        end(end),
        automaton(automaton)
    { }

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    void _reset() override;
    bool _next() override;

    void assign_nulls() override
    {
        parent_binding->add(end, ObjectId::get_null());
        parent_binding->add(path_var, ObjectId::get_null());
    }

    // Set iter attribute that obtains all states that are connected with
    // the current_state through a specific edge transition
    void set_iter(const SearchState& current_state);

    // Statistics
    uint_fast32_t idx_searches = 0;

private:
    // Attributes determined in the constructor
    VarId path_var;
    Id start;
    VarId end;
    RDPQAutomaton automaton;

    // Attributes determined in begin
    Binding* parent_binding;
    bool first_next = true;

    // Ranges to search in BPT. They are not local variables because some positions are reused.
    std::array<uint64_t, 4> min_ids;
    std::array<uint64_t, 4> max_ids;

    boost::unordered_node_set<SearchState, std::hash<SearchState>> visited;
    // open stores a pointer to a SearchState stored in visited
    // that allows to avoid use visited.find to get a pointer and
    // use the state extracted of open directly.
    std::queue<const SearchState*> open;

    // Stores the children of state in expansion
    BptIter<4> iter;
    const Record<4UL>* child_record;

    // The index of the transitions that are currently being explored
    uint32_t current_data_transition;
    uint32_t current_edge_transition;

    // Evaluate data checks for a specific node
    bool eval_data_check(
        uint64_t node,
        std::vector<std::tuple<Operators, ObjectId, ObjectId>>& property_checks
    );

    // Obtain the next state to add to open (if it exists)
    boost::unordered_node_set<SearchState, std::hash<SearchState>>::iterator
        expand_neighbors(const SearchState& current_state);
};
}} // namespace Paths::Any
