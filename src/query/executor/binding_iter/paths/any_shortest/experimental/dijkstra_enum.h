#pragma once

#include <array>
#include <memory>
#include <queue>
#include <variant>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/any_shortest/experimental/search_state_dijkstra.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"
#include "query/parser/paths/automaton/rdpq_automaton.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AnyShortest {
/*
DijkstraEnum enumerates shortest paths in a similar way to BFSIterEnum, but using the projection
of a specific property over the edges of the path as the cost (instead of the number of edge traversals).
To assure optimality for the shortest paths, the results are returned only after the state is removed from open.
*/
class DijkstraEnum : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId         path_var;
    Id            start;
    VarId         end;
    RDPQAutomaton automaton;
    ObjectId      cost_key;  // Key for the property value that generates the cost

    // Attributes determined in begin
    Binding* parent_binding;
    bool first_next = true;

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
    const Record<4>* child_record;

    // The index of the transitions that are currently being explored
    uint32_t current_data_transition;
    uint32_t current_edge_transition;

    // Evaluate data checks for a specific node
    bool eval_data_check(
        uint64_t node,
        std::vector<std::tuple<Operators, ObjectId, ObjectId>>& property_checks
    );

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t bpt_searches = 0;

    // Obtain the next state to add to open or to update (if it exists)
    robin_hood::unordered_node_set<SearchStateDijkstra>::iterator
      expand_neighbors(const SearchStateDijkstra* current_state);

    // Set iter attribute that obtains all states that are connected with
    // the current_state through a specific edge transition
    void set_iter(const SearchStateDijkstra* current_state);

public:
    DijkstraEnum(
        VarId                          path_var,
        Id                             start,
        VarId                          end,
        RDPQAutomaton                  automaton
    ) :
        path_var      (path_var),
        start         (start),
        end           (end),
        automaton     (automaton) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    void reset() override;
    bool next() override;

    void assign_nulls() override {
        parent_binding->add(end, ObjectId::get_null());
        parent_binding->add(path_var, ObjectId::get_null());
    }
};
}} // namespace Paths::AnyShortest
