#pragma once

#include <memory>
#include <stack>
#include <type_traits>

#include "base/binding/binding_id_iter.h"
#include "base/ids/id.h"
#include "base/thread/thread_info.h"
#include "execution/binding_id_iter/paths/any/search_state.h"
#include "execution/binding_id_iter/paths/any_shortest/search_state.h"
#include "parser/query/paths/automaton/rpq_automaton.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace Any {

// Dummy structure for template usage
class DummySet {
public:
    static inline void clear() { }
    static inline int end() {return 0;}
    static inline int find(uint64_t) {return 0;}
    static inline void insert(uint64_t) { }
};

/*
DFSEnum returns a single path to all reachable nodes from a starting node, using DFS.
*/
template <bool MULTIPLE_FINAL> class DFSEnum : public BindingIdIter {
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

    // Structs for DFS

    // Set of visited SearchStates
    robin_hood::unordered_node_set<Paths::AnyShortest::SearchState> visited;

    // Stack of DFSSearchStates
    std::stack<DFSSearchState> open;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t idx_searches  = 0;

    // Get next state of interest
    robin_hood::unordered_node_set<Paths::AnyShortest::SearchState>::iterator
      current_state_has_next(DFSSearchState& current_state);

    // Set iterator for current node + transition
    void set_iter(DFSSearchState& current_state);

    // Template type for storing reached nodes with a final state
    typename std::conditional<MULTIPLE_FINAL, robin_hood::unordered_set<uint64_t>, DummySet>::type reached_final;

public:
    DFSEnum(ThreadInfo* thread_info,
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
}} // namespace Paths::Any
