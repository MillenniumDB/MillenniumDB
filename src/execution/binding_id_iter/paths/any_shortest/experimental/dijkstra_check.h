#pragma once

#include <array>
#include <memory>
#include <queue>
#include <variant>

#include "base/binding/binding_id_iter.h"
#include "base/thread/thread_info.h"
#include "parser/query/paths/automaton/rdpq_automaton.h"
#include "execution/binding_id_iter/paths/any_shortest/experimental/search_state_dijkstra.h"
#include "execution/binding_id_iter/scan_ranges/scan_range.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AnyShortest {

/*
DijkstraCheck checks if there's a path between two fixed nodes in the graph that satisfies an RPQ, similar to BFSCheck.
If the path exists, it returns the shortest one according to a cost projection from an edge property.
The automaton used is a DE automaton.
*/
class DijkstraCheck : public BindingIdIter {
private:
    // Attributes determined in the constructor
    ThreadInfo*   thread_info;
    VarId         path_var;
    Id            start;
    Id            end;
    RDPQAutomaton automaton;
    ObjectId      cost_key;  // Key for the property value that generates the cost

    // Attributes determined in begin
    BindingId* parent_binding;
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
    std::unique_ptr<BptIter<4>> iter;

    // Evaluate data checks for a specific node
    bool eval_data_check(uint64_t node, std::vector<std::tuple<Operators, std::string, QueryElement>>& property_checks);

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t bpt_searches = 0;

    // Constructs iter according to transition
    void set_iter(const RDPQTransition& transition, const SearchStateDijkstra* current_state);

public:
    DijkstraCheck(ThreadInfo*   thread_info,
                  VarId         path_var,
                  Id            start,
                  Id            end,
                  RDPQAutomaton automaton,
                  ObjectId      cost_key);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    void reset() override;
    inline void assign_nulls() override { };
    bool next() override;
};
}} // namespace Paths::AnyShortest
