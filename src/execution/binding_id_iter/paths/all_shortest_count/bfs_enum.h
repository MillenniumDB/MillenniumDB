#pragma once

#include <memory>
#include <queue>
#include <type_traits>

#include "base/binding/binding_id_iter.h"
#include "base/ids/id.h"
#include "base/thread/thread_info.h"
#include "execution/binding_id_iter/paths/all_shortest_count/search_state.h"
#include "execution/binding_id_iter/paths/index_provider/path_index/path_index.h"
#include "parser/query/paths/automaton/rpq_automaton.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AllShortestCount {

// Holds necessary information for nodes reached with final states
struct ResultInfo {
    uint64_t count;
    uint64_t distance;

    ResultInfo(uint64_t count, uint64_t distance) :
        count    (count),
        distance (distance) { }
};

// Dummy structure for template usage
class DummyMap {
public:
    static inline void clear() { }
    static inline std::pair<uint64_t, ResultInfo>* end() {return nullptr;}
    template <class T>
    static inline void erase(T) { }
    static inline std::pair<uint64_t, ResultInfo>* find(uint64_t) {return nullptr;}
    static inline void insert(std::pair<uint64_t, ResultInfo>) { }
};

/*
BFSEnum finds all reachable nodes according to the RPQ from a fixed start
node, and then returns the count of shortest paths between the start node and each of these
reached nodes.
*/
template <bool MULTIPLE_FINAL> class BFSEnum : public BindingIdIter {
private:
    // Attributes determined in the constructor
    ThreadInfo*   thread_info;
    VarId         path_var;
    Id            start;
    VarId         end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // Attributes determined in begin
    BindingId* parent_binding;
    bool first_next = true;

    // Structs for BFS

    // Set of visited SearchStates
    robin_hood::unordered_node_set<SearchState> visited;

    // Queue of pointers to a SearchState from visited
    std::queue<const SearchState*> open;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t idx_searches = 0;

    // Explore neighbors from current state
    void explore_neighbors(const SearchState* current_state);

    // Template type for storing reached nodes with a final state, and their necessary info: NodeId -> Info
    typename std::conditional<MULTIPLE_FINAL, robin_hood::unordered_map<uint64_t, ResultInfo>, DummyMap>::type reached_final;

public:
    BFSEnum(ThreadInfo* thread_info,
            VarId       path_var,
            Id          start,
            VarId       end,
            RPQ_DFA     automaton,
            std::unique_ptr<IndexProvider> provider);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    void reset() override;
    void assign_nulls() override;
    bool next() override;
};
}} // namespace Paths::AllShortestCount
