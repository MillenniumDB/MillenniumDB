#pragma once

#include <memory>
#include <queue>
#include <type_traits>

#include <boost/unordered/unordered_node_set.hpp>
#include <boost/unordered/unordered_flat_set.hpp>
#include <boost/unordered/unordered_node_map.hpp>
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/paths/any_walks/search_state.h"
#include "query/executor/binding_iter/paths/index_provider/path_index.h"
#include "query/parser/paths/automaton/rpq_automaton.h"
#include "search_state_multiple_starts.h"

#include "debug_mati.h"

// For unordered set
struct objectid_hash
{
    std::size_t operator()(const ObjectId &o) const
    {
        return o.id;
    }
};

struct searchnodeid_hash
{
    std::size_t operator()(const std::pair<uint32_t, ObjectId> &p) const
    {
        return p.second.id ^ p.first;
    }
};
namespace Paths
{
    namespace Any
    {

        /*
        BFSMultipleStarts returns a single path to all
        reachable nodes from a starting node, using BFS.
        */
        template <bool MULTIPLE_FINAL>
        class BFSMultipleStarts : public BindingIter
        
        {
            using SearchNodeId = std::pair<uint32_t, ObjectId>;

        private:
            // Attributes determined in the constructor
            VarId path_var;
            std::vector<Id> start_nodes;
            VarId end;
            const RPQ_DFA automaton;
            std::unique_ptr<IndexProvider> provider;

            // where the results will be written, determined in begin()
            Binding *parent_binding;

            boost::unordered_node_map<ObjectId, boost::unordered_node_map<SearchNodeId, MultiSourceSearchState, searchnodeid_hash>, objectid_hash> seen;
            boost::unordered_node_map<SearchNodeId, boost::unordered_node_set<ObjectId, objectid_hash>, searchnodeid_hash> bfss_that_reached_given_node;

            // visit and visit_next. Maybe we can use set instead of map and the values (bfs ids) can be taken from `bfss_that_reached_given_node`.
            boost::unordered_node_map<SearchNodeId, boost::unordered_node_set<ObjectId, objectid_hash>, searchnodeid_hash> bfses_to_be_visited;
            boost::unordered_node_map<SearchNodeId, boost::unordered_node_set<ObjectId, objectid_hash>, searchnodeid_hash> bfses_to_be_visited_next;

            std::queue<SearchNodeId> visit_q;
            std::queue<SearchNodeId> first_visit_q;

            // Iterator for current node expansion
            std::unique_ptr<EdgeIter> iter;

            // The index of the transition being currently explored
            uint_fast32_t current_transition;

            std::queue<ObjectId> start_nodes_for_current_iteration;
            SearchNodeId node_for_current_iteration;

        public:
            // Statistics
            uint_fast32_t idx_searches = 0;

            BFSMultipleStarts(
                VarId path_var,
                std::vector<Id> start_nodes,
                VarId end_nodes,
                RPQ_DFA automaton,
                std::unique_ptr<IndexProvider> provider) : path_var(path_var),
                                                           start_nodes(start_nodes),
                                                           end(end_nodes),
                                                           automaton(automaton),
                                                           provider(std::move(provider)) {

                _debug_mati() << "hello!" << std::endl;
                                                           }

            void accept_visitor(BindingIterVisitor &visitor) override;
            void _begin(Binding &parent_binding) override;
            void _reset() override;
            bool _next() override;

            // Expand neighbors from current state
            const MultiSourceSearchState *expand_neighbors(const SearchNodeId &current_state);

            void assign_nulls() override
            {
                parent_binding->add(end, ObjectId::get_null());
                parent_binding->add(path_var, ObjectId::get_null());
            }

            // Set iterator for current node + transition
            inline void set_iter(const SearchNodeId &s)
            {
                // Get current transition object from automaton
                auto &transition = automaton.from_to_connections[s.first][current_transition];

                // Get iterator from custom index
                iter = provider->get_iter(transition.type_id.id, transition.inverse, s.second.id);
                idx_searches++;
            }
        };

    }
} // namespace Paths::Any
