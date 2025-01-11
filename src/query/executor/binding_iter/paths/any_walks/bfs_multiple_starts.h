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

namespace Paths { namespace Any {

/*
BFSMultipleStarts returns a single path to all
reachable nodes from a starting node, using BFS.
*/
template <bool MULTIPLE_FINAL>
class BFSMultipleStarts : public BindingIter {
    using SearchNodeId = std::pair<uint32_t, ObjectId>;
private:
    // Attributes determined in the constructor
    VarId         path_var;
    boost::unordered_node_set<Id>            start_nodes;
    boost::unordered_node_set<VarId>         end_nodes;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // where the results will be written, determined in begin()
    Binding* parent_binding;

    boost::unordered_node_map<Id, boost::unordered_node_set<MultipleStartsSearchState>> seen;
    boost::unordered_node_map<SearchNodeId, boost::unordered_node_set<ObjectId>> bfss_that_reached_given_node;

    // visit and visit_next. Maybe we can use set instead of map and the values (bfs ids) can be taken from `bfss_that_reached_given_node`.
    boost::unordered_node_map<SearchNodeId, boost::unordered_node_set<ObjectId>> bfses_to_be_visited;
    boost::unordered_node_map<SearchNodeId, boost::unordered_node_set<ObjectId>> bfses_to_be_visited_next;
    
    
    // Queue for BFS. Pointers point to the states in visited
    // std::queue<const SearchState*> open;

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

    BFSMultipleStarts(
        VarId                          path_var,
        boost::unordered_node_set<Id>                             start_nodes,
        boost::unordered_node_set<VarId>                          end_nodes,
        RPQ_DFA                        automaton,
        std::unique_ptr<IndexProvider> provider
    ) :
        path_var      (path_var),
        start_nodes         (start_nodes),
        end_nodes           (end_nodes),
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
