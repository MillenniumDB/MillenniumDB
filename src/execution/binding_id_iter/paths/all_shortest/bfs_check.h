#pragma once

#include <memory>
#include <queue>

#include "base/binding/binding_id_iter.h"
#include "base/ids/id.h"
#include "base/thread/thread_info.h"
#include "execution/binding_id_iter/paths/all_shortest/search_state.h"
#include "execution/binding_id_iter/paths/index_provider/path_index/path_index.h"
#include "parser/query/paths/automaton/rpq_automaton.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AllShortest {

/*
BFSCheck returns all the shortest paths between two fixed nodes.
*/
class BFSCheck : public BindingIdIter {
private:
    // Attributes determined in the constructor
    ThreadInfo*   thread_info;
    VarId         path_var;
    Id            start;
    Id            end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // Attributes determined in begin
    BindingId* parent_binding;
    ObjectId end_object_id;
    bool first_next = true; // true in the first call of next

    // Structs for BFS

    // Set of visited SearchStates
    robin_hood::unordered_node_set<SearchState> visited;

    // Queue of pointers to a SearchState from visited
    std::queue<const SearchState*> open;

    // The index of the transition being currently explored
    uint32_t current_transition = 0;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t idx_searches = 0;

    // Used for enumeration
    const SearchState* saved_state_reached = nullptr;

    // Expand neighbors from current state
    bool expand_neighbors(const SearchState* current_state);

    // Set iterator for current node + transition
    void set_iter(const SearchState* current_state);

    // Iterator for index results
    std::unique_ptr<IndexIterator> iter;

    // Optimal distance to target node. UINT64_MAX means the node has not been explored yet.
    uint64_t optimal_distance;

public:
    BFSCheck(ThreadInfo* thread_info,
             VarId       path_var,
             Id          start,
             Id          end,
             RPQ_DFA     automaton,
             std::unique_ptr<IndexProvider> provider);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    void reset() override;
    bool next() override;
    void assign_nulls() override { }
};
}} // namespace Paths::AllShortest
