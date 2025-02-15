#include "bfs_multiple_starts.h"

#include <iostream>
#include <boost/range/algorithm/set_algorithm.hpp>

#include "graph_models/quad_model/quad_object_id.h"
#include "graph_models/quad_model/quad_model.h"
#include "system/path_manager.h"

using namespace std;
using namespace Paths::Any;

template <bool MULTIPLE_FINAL>
void BFSMultipleStarts<MULTIPLE_FINAL>::_begin(Binding &_parent_binding)
{
    parent_binding = &_parent_binding;
    // first_next = true;

    //////////////////////////////////////////////////////////////
    // Override starting nodes using the nodes with label "start"
    //////////////////////////////////////////////////////////////
    start_nodes.clear();
    bool interruption = false;
    auto label_start = QuadObjectId::get_string("start").id;
    auto it = quad_model.label_node->get_range(&interruption, {label_start, 0}, {label_start, UINT64_MAX});

    std::cout << "starting nodes:\n";
    for (auto record = it.next(); record != nullptr; record = it.next())
    {
        ObjectId node((*record)[1]);
        std::cout << node << std::endl;
        start_nodes.push_back(node);
    }
    //////////////////////////////////////////////////////////////

    for (auto node : start_nodes)
    {
        ObjectId start_object_id = node.is_var() ? (*parent_binding)[node.get_var()] : node.get_OID();
        SearchNodeId start_node_id{automaton.start_state, start_object_id};
        bfses_to_be_visited[start_node_id] = {start_object_id};

        seen[start_object_id] = {{start_node_id, MultiSourceSearchState(
                                                     automaton.start_state,
                                                     start_object_id,
                                                     nullptr,
                                                     true,
                                                     ObjectId::get_null(),
                                                     start_object_id)}};
        bfss_that_reached_given_node[start_node_id] = {
            start_object_id};
        first_visit_q.push(start_node_id);
    }
    iter = make_unique<NullIndexIterator>();
}

template <bool MULTIPLE_FINAL>
void BFSMultipleStarts<MULTIPLE_FINAL>::_reset()
{
    // Empty open and visited
    bfses_to_be_visited.clear();
    bfses_to_be_visited_next.clear();
    seen.clear();
    bfss_that_reached_given_node.clear();
    visit_q = {};
    first_visit_q = {};

    for (auto node : start_nodes)
    {
        ObjectId start_object_id = node.is_var() ? (*parent_binding)[node.get_var()] : node.get_OID();
        SearchNodeId start_node_id{automaton.start_state, start_object_id};
        bfses_to_be_visited[start_node_id] = {start_object_id};

        seen[start_object_id] = {{start_node_id, MultiSourceSearchState(
                                                     automaton.start_state,
                                                     start_object_id,
                                                     nullptr,
                                                     true,
                                                     ObjectId::get_null(),
                                                     start_object_id)}};
        bfss_that_reached_given_node[start_node_id] = {
            start_object_id};
        first_visit_q.push(start_node_id);
    }
    iter = make_unique<NullIndexIterator>();
}

template <bool MULTIPLE_FINAL>
bool BFSMultipleStarts<MULTIPLE_FINAL>::_next()
{
    // Check if first state is final
    while (!first_visit_q.empty())
    {
        SearchNodeId curr_first_node = first_visit_q.front();
        first_visit_q.pop();

        if (provider->node_exists(curr_first_node.second.id))
        {
            visit_q.push(curr_first_node);
            if (automaton.is_final_state[automaton.start_state])
            {
                auto pointer_to_reached_state = &seen[curr_first_node.second].find(curr_first_node)->second;
                auto path_id = path_manager.set_path(pointer_to_reached_state, path_var);
                parent_binding->add(path_var, path_id);
                parent_binding->add(end, curr_first_node.second);
                _debug_mati() << "Returning first node " << curr_first_node.second << " " << curr_first_node.first << std::endl;
                return true;
            }
        }
    }

    while (visit_q.size() > 0)
    {
        while (visit_q.size() > 0)
        {
            auto current_node_id = visit_q.front();
            auto reached_final_state = expand_neighbors(current_node_id);
            // Enumerate reached solutions
            if (reached_final_state != nullptr)
            {
                _debug_mati() << "Reached final state for bfs id " << *reached_final_state << std::endl;
                auto path_id = path_manager.set_path(reached_final_state, path_var);
                parent_binding->add(path_var, path_id);
                parent_binding->add(end, reached_final_state->node_id);
                return true;
            }
            else
            {
                visit_q.pop();
            }
        }
        for (const auto &it : bfses_to_be_visited_next)
        {
            visit_q.push(it.first);
        }
        bfses_to_be_visited = bfses_to_be_visited_next;
        bfses_to_be_visited_next.clear();
    }
    return false;
}

template <typename Container>
inline void debug_print_container(const Container &c)
{
    for (const auto &elem : c)
    {
        _debug_mati_simple() << elem << " ";
    }
}

template <bool MULTIPLE_FINAL>
const MultiSourceSearchState *BFSMultipleStarts<MULTIPLE_FINAL>::expand_neighbors(const SearchNodeId &current_state_id)
{
    // Check if this is the first time that current_state is explored
    if (iter->at_end())
    {
        _debug_mati() << "state explored first time " << std::endl;
        current_transition = 0;
        // Check if automaton state has transitions
        if (automaton.from_to_connections[current_state_id.first].size() == 0)
        {
            return nullptr;
        }
        set_iter(current_state_id);
    }

    while (!start_nodes_for_current_iteration.empty())
    {
        auto bfs_id = start_nodes_for_current_iteration.front();
        _debug_mati() << "start nodes for current iteration not empty! start_nodes_for_current_iteration.size() = " << start_nodes_for_current_iteration.size() << ", bfs_id = " << bfs_id << std::endl;
        start_nodes_for_current_iteration.pop();
        auto ms_search_state_it = seen[bfs_id].find(node_for_current_iteration);
        assert(ms_search_state_it != seen[bfs_id].end());
        auto ms_search_state = &(ms_search_state_it->second);
        return ms_search_state;
    }

    // Iterate over the remaining transitions of current_state
    // Don't start from the beginning, resume where it left thanks to current_transition and iter (pipeline)
    while (current_transition < automaton.from_to_connections[current_state_id.first].size())
    {
        auto &transition = automaton.from_to_connections[current_state_id.first][current_transition];

        // Iterate over records until a final state is reached
        while (iter->next())
        {

            _debug_mati() << "current_state_id: " << current_state_id.first << ", " << current_state_id.second << std::endl;
            auto bfses_to_be_visited_by_current_state = bfses_to_be_visited[current_state_id];

            _debug_mati() << "bfses_to_be_visited_by_current_state: ";
            debug_print_container(bfses_to_be_visited_by_current_state);
            _debug_mati_simple() << std::endl;

            SearchNodeId new_node_id = {transition.to, ObjectId(iter->get_reached_node())};
            _debug_mati() << "new_node_id: " << new_node_id.first << ", " << new_node_id.second << std::endl;
            auto bfses_that_reached_the_new_state = bfss_that_reached_given_node[new_node_id];

            _debug_mati() << "bfses_that_reached_the_new_state: ";
            debug_print_container(bfses_that_reached_the_new_state);
            _debug_mati_simple() << std::endl;

            boost::unordered_node_set<ObjectId, objectid_hash> difference;
            boost::set_difference(bfses_to_be_visited_by_current_state, bfses_that_reached_the_new_state, std::inserter(difference, difference.end()));

            _debug_mati() << "difference: ";
            debug_print_container(difference);
            _debug_mati_simple() << std::endl;

            if (!difference.empty())
            {
                bfses_to_be_visited_next[new_node_id].insert(difference.begin(), difference.end());
                for (const auto &bfs_to_visit_new_state_next : difference)
                {
                    MultiSourceSearchState s(
                        new_node_id.first,
                        new_node_id.second,
                        &(seen[bfs_to_visit_new_state_next].find(current_state_id)->second),
                        transition.inverse,
                        transition.type_id,
                        bfs_to_visit_new_state_next);
                    seen[bfs_to_visit_new_state_next].insert({new_node_id, s});
                    bfss_that_reached_given_node[new_node_id].insert(bfs_to_visit_new_state_next);
                }
                // Check if new path is solution
                if (automaton.is_final_state[new_node_id.first])
                {
                    _debug_mati() << "new state is final state " << std::endl;
                    _debug_mati() << "start_nodes_for_current_iteration: size() = " << start_nodes_for_current_iteration.size() << std::endl;
                    // TODO think if I should be clearing this or not...
                    std::queue<ObjectId> empty_queue;
                    std::swap(start_nodes_for_current_iteration, empty_queue);
                    for (const auto &bfs_to_visit_new_state_next : difference)
                    {
                        start_nodes_for_current_iteration.push(bfs_to_visit_new_state_next);
                    }
                    _debug_mati() << "old node_for_current_iteration " << node_for_current_iteration.first << ", " << node_for_current_iteration.second << std::endl;
                    node_for_current_iteration = new_node_id;
                    _debug_mati() << "new node_for_current_iteration " << node_for_current_iteration.first << ", " << node_for_current_iteration.second << std::endl;
                    {
                        auto bfs_id = start_nodes_for_current_iteration.front();
                        start_nodes_for_current_iteration.pop();
                        auto ms_search_state_it = seen[bfs_id].find(node_for_current_iteration);
                        assert(ms_search_state_it != seen[bfs_id].end());
                        auto ms_search_state = &(ms_search_state_it->second);
                        return ms_search_state;
                    }
                }
            }
        }

        // Construct new iter with the next transition (if there exists one)
        current_transition++;
        if (current_transition < automaton.from_to_connections[current_state_id.first].size())
        {
            set_iter(current_state_id);
        }
    }
    return nullptr;
}

template <bool MULTIPLE_FINAL>
void BFSMultipleStarts<MULTIPLE_FINAL>::accept_visitor(BindingIterVisitor &visitor)
{
    visitor.visit(*this);
}

template class Paths::Any::BFSMultipleStarts<true>;
template class Paths::Any::BFSMultipleStarts<false>;
