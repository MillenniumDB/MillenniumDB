#pragma once

#include <memory>
#include <queue>
#include <type_traits>

#include "base/binding/binding_id_iter.h"
#include "base/ids/id.h"
#include "base/thread/thread_info.h"
#include "execution/binding_id_iter/paths/all_shortest/search_state.h"
#include "execution/binding_id_iter/paths/index_provider/path_index/path_index.h"
#include "parser/query/paths/automaton/rpq_automaton.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AllShortest {

// Dummy structure for template usage
class DummyMap {
public:
    static inline void clear() { }
    static inline std::pair<uint64_t, size_t>* end() {return nullptr;}
    static inline std::pair<uint64_t, size_t>* find(uint64_t) {return nullptr;}
    static inline void insert(std::pair<uint64_t, size_t>) { }
};

/*
BFSEnum returns the shortest paths to all
reachable nodes from a starting node.
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

    // Template type for storing reached nodes with a final state, and their optimal distance: NodeId -> OptimalDistance
    typename std::conditional<MULTIPLE_FINAL, robin_hood::unordered_map<uint64_t, size_t>, DummyMap>::type reached_final;

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
}} // namespace Paths::AllShortest
