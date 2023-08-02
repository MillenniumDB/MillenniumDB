#pragma once

#include <memory>
#include <stack>
#include <type_traits>

#include "query/executor/binding_iter.h"
#include "query/id.h"
#include "query/executor/binding_iter/paths/any/search_state.h"
#include "query/executor/binding_iter/paths/any_shortest/search_state.h"
#include "query/parser/paths/automaton/rpq_automaton.h"
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
template <bool MULTIPLE_FINAL>
class DFSEnum : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId         path_var;
    Id            start;
    VarId         end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // Attributes determined in begin
    Binding* parent_binding;
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
      expand_neighbors(DFSSearchState& current_state);

    // Template type for storing reached nodes with a final state
    typename std::conditional<MULTIPLE_FINAL, robin_hood::unordered_set<uint64_t>, DummySet>::type reached_final;

public:
    DFSEnum(
        VarId                          path_var,
        Id                             start,
        VarId                          end,
        RPQ_DFA                        automaton,
        std::unique_ptr<IndexProvider> provider
    ) :
        path_var      (path_var),
        start         (start),
        end           (end),
        automaton     (automaton),
        provider      (std::move(provider)) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    void reset() override;
    bool next() override;

    void assign_nulls() override {
        parent_binding->add(end, ObjectId::get_null());
        parent_binding->add(path_var, ObjectId::get_null());
    }

    // Set iterator for current node + transition
    inline void set_iter(DFSSearchState& s) {
        // Get current transition object from automaton
        auto& transition = automaton.from_to_connections[s.state][s.current_transition];

        // Get iterator from custom index
        s.iter = provider->get_iter(transition.type_id.id, transition.inverse, s.node_id.id);
        idx_searches++;
    }
};
}} // namespace Paths::Any
