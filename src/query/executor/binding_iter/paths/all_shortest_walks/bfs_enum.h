#pragma once

#include <memory>
#include <queue>
#include <type_traits>

#include "misc/arena.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/all_shortest_walks/search_state.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/parser/paths/automaton/rpq_automaton.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AllShortest {

// Dummy structure for template usage
class DummyMap {
public:
    static inline void clear() { }
    static inline std::pair<uint64_t, size_t>* end() { return nullptr; }
    static inline std::pair<uint64_t, size_t>* find(uint64_t) { return nullptr; }
    static inline void insert(std::pair<uint64_t, size_t>) { }
};

/*
AllShortest::BFSEnum finds all reachable nodes from a fixed node (via RPQ) and
returns all the shortest paths between each of them.
The typical BFS algorithm gives optimal paths over the pairs <node, automaton_state>,
so we can reach the same node with different final states and they can have
different path lengths, so we need to do an additional checking.
The MULTIPLE_FINAL final is an optimization to avoid that additional checking
when the automaton has only one final state.
*/
template <bool MULTIPLE_FINAL>
class BFSEnum : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId         path_var;
    Id            start;
    VarId         end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // where the results will be written, determined in begin()
    Binding* parent_binding;

    // Remembers which states were explored. A structure with pointer stability is required
    robin_hood::unordered_node_set<SearchState> visited;

    // Queue for BFS. Pointers point to the states in visited
    std::queue<const SearchState*> open;

    // Iterator for current node expansion
    std::unique_ptr<EdgeIter> iter;

    // The index of the transition being currently explored
    uint_fast32_t current_transition;

    // true in the first call of next() and after a reset()
    bool first_next = true;

    // Once the search reaches a solution we might find many paths
    // and we need to enumerate them one by one.
    // When solution_found != nullptr we are enumerating the paths found.
    const SearchState* solution_found = nullptr;

    // Template type for storing reached nodes with a final state, and their optimal distance:
    // NodeId -> OptimalDistance
    typename std::conditional<MULTIPLE_FINAL,
                              robin_hood::unordered_map<uint64_t, size_t>,
                              DummyMap>::type optimal_distances;

    Arena<IterTransition> iter_arena;

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    BFSEnum(
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

    // Explore neighbors searching for a solution.
    // Returns true if a solution was found. In that case
    // solution_found was set.
    bool expand_neighbors(const SearchState& current_state);

    void accept_visitor(BindingIterVisitor& visitor) override;

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override {
        parent_binding->add(path_var, ObjectId::get_null());
        parent_binding->add(end, ObjectId::get_null());
    }

    // Set iterator for current node + transition
    inline void set_iter(const SearchState& current_state) {
        // Get current transition object from automaton
        auto& transition = automaton.from_to_connections[current_state.automaton_state][current_transition];

        // Get iterator from custom index
        iter = provider->get_iter(transition.type_id.id, transition.inverse, current_state.node_id.id);
        idx_searches++;
    }
};
}} // namespace Paths::AllShortest
