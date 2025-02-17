#pragma once

#include <memory>
#include <queue>
#include <type_traits>

#include <boost/unordered/unordered_node_set.hpp>
#include <boost/unordered/unordered_flat_set.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/any_walks/search_state.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/parser/paths/automaton/rpq_automaton.h"

#include "debug_mati.h"

namespace Paths { namespace Any {

/*
BFSMultipleStartsNaive
*/
template <bool MULTIPLE_FINAL>
class BFSMultipleStartsNaive : public BindingIter {
private:
    bool single_next();
    void single_reset(); 
    void set_new_start_node();
    // Attributes determined in the constructor
    VarId         path_var;
    std::vector<Id> start_nodes;
    VarId         end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    std::queue<ObjectId> start_nodes_q;
    ObjectId current_start_node;
    // where the results will be written, determined in begin()
    Binding* parent_binding;

    // Set of visited SearchStates
    boost::unordered_node_set<SearchState, std::hash<SearchState>> visited;

    // Queue for BFS. Pointers point to the states in visited
    std::queue<const SearchState*> open;

    // Iterator for current node expansion
    std::unique_ptr<EdgeIter> iter;

    // The index of the transition being currently explored
    uint_fast32_t current_transition;

    // true in the first call of next() and after a reset()
    bool first_next = true;

    // Template type for storing nodes reached with a final state
    typename std::conditional<MULTIPLE_FINAL,
                              boost::unordered_flat_set<uint64_t>,
                              DummySet>::type reached_final;

public:
    // Statistics
    uint_fast32_t idx_searches = 0;

    BFSMultipleStartsNaive(
        VarId                          path_var,
        std::vector<Id> start_nodes,
        VarId                          end,
        RPQ_DFA                        automaton,
        std::unique_ptr<IndexProvider> provider
    ) :
        path_var      (path_var),
        start_nodes         (start_nodes),
        end           (end),
        automaton     (automaton),
        provider      (std::move(provider)) { }


    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    void _reset() override;
    bool _next() override;

    // Expand neighbors from current state
    const SearchState* expand_neighbors(const SearchState& current_state);

    void assign_nulls() override {
        parent_binding->add(end, ObjectId::get_null());
        parent_binding->add(path_var, ObjectId::get_null());
    }

    // Set iterator for current node + transition
    inline void set_iter(const SearchState& s) {
        // Get current transition object from automaton
        auto& transition = automaton.from_to_connections[s.automaton_state][current_transition];

        // Get iterator from custom index
        iter = provider->get_iter(transition.type_id.id, transition.inverse, s.node_id.id);
        idx_searches++;
    }
};
}} // namespace Paths::Any
