#pragma once

#include <queue>

#include <boost/unordered/unordered_node_set.hpp>

#include "graph_models/object_id.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/any_walks/search_state.h"
#include "query/parser/paths/automaton/rdpq_automaton.h"
#include "storage/index/bplus_tree/bplus_tree.h"

namespace Paths { namespace Any {

/*
BFS_RDPQCheck checks if there's a path between two fixed nodes in the graph that satisfies an RPQ.
If the path exists, it returns the shortest one according to edge traversals. The automaton used is a DE automaton.
This means that once the first call to next() returns, there are no more results, which is why it's not necessary
to have class members that keep track of the transitions being explored (unlike the iter algorithms).
*/
class BFS_RDPQCheck : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId path_var;
    Id start;
    Id end;
    RDPQAutomaton automaton;

    // Attributes determined in begin
    Binding* parent_binding;
    ObjectId end_object_id;
    bool is_first; // True in the first call of next

    // Ranges to search in BPT. They are not local variables because some positions are reused.
    std::array<uint64_t, 4> min_ids;
    std::array<uint64_t, 4> max_ids;

    boost::unordered_node_set<SearchState, std::hash<SearchState>> visited;
    // open stores a pointer to a SearchState stored in visited
    // that allows to avoid use visited.find to get a pointer and
    // use the state extracted of the open directly.
    std::queue<const SearchState*> open;
    BptIter<4> iter;

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    BFS_RDPQCheck(ThreadInfo* thread_info, VarId path_var, Id start, Id end, RDPQAutomaton automaton);

    BFS_RDPQCheck(VarId path_var, Id start, Id end, RDPQAutomaton automaton) :
        path_var(path_var),
        start(start),
        end(end),
        automaton(automaton)
    { }

    // Evaluate data checks for a specific node
    bool eval_data_check(
        uint64_t node,
        std::vector<std::tuple<Operators, ObjectId, ObjectId>>& property_checks
    );

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    void _reset() override;
    bool _next() override;

    void assign_nulls() override
    {
        parent_binding->add(path_var, ObjectId::get_null());
    }
    // Constructs iter according to transition
    void set_iter(const RDPQTransition& transition, const SearchState& current_state);
};
}} // namespace Paths::Any
