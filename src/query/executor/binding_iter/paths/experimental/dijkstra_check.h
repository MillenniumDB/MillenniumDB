#pragma once

#include <memory>
#include <queue>

#include "query/executor/binding_iter.h"
#include "query/parser/paths/automaton/rdpq_automaton.h"
#include "query/executor/binding_iter/paths/experimental/search_state_dijkstra.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace Any {

/*
DijkstraCheck checks if there's a path between two fixed nodes in the graph that satisfies an RPQ, similar to BFSCheck.
If the path exists, it returns the shortest one according to a cost projection from an edge property.
The automaton used is a DE automaton.
*/
class DijkstraCheck : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId         path_var;
    Id            start;
    Id            end;
    RDPQAutomaton automaton;
    ObjectId      cost_key;  // Key for the property value that generates the cost

    // Attributes determined in begin
    Binding* parent_binding;
    ObjectId end_object_id;
    bool is_first;  // true in the first call of next

    // Ranges to search in BPT. They are not local variables because some positions are reused.
    std::array<uint64_t, 4> min_ids;
    std::array<uint64_t, 4> max_ids;

    // Structs for Dijkstra
    robin_hood::unordered_node_set<SearchStateDijkstra> visited;
    // open stores a pointer to a SearchStateDijkstra stored in visited
    // that allows to avoid use visited.find to get a pointer and
    // use the state extracted of the open directly.

    // Use min priority queue, where the priority is the cost
    std::priority_queue<DijkstraQueueState, std::vector<DijkstraQueueState>, DijkstraQueueStateComp> open;
    uint64_t edge_cost;

    // Stores the children of state in expansion
    BptIter<4> iter;

    // Evaluate data checks for a specific node
    bool eval_data_check(
        uint64_t node,
        std::vector<std::tuple<Operators, ObjectId, ObjectId>>& property_checks
    );

    // Constructs iter according to transition
    void set_iter(const RDPQTransition& transition, const SearchStateDijkstra* current_state);

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    DijkstraCheck(
        VarId                          path_var,
        Id                             start,
        Id                             end,
        RDPQAutomaton                  automaton
    ) :
        path_var      (path_var),
        start         (start),
        end           (end),
        automaton     (automaton) { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    void _reset() override;
    bool _next() override;

    void assign_nulls() override {
        parent_binding->add(path_var, ObjectId::get_null());
    }
};
}} // namespace Paths::Any
