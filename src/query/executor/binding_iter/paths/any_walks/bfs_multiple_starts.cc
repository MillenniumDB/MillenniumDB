#include "bfs_multiple_starts.h"

#include "system/path_manager.h"
#include <boost/range/algorithm/set_algorithm.hpp>

using namespace std;
using namespace Paths::Any;

template <bool MULTIPLE_FINAL>
void BFSMultipleStarts<MULTIPLE_FINAL>::_begin(Binding &_parent_binding)
{
    parent_binding = &_parent_binding;
    // first_next = true;

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

    if (MULTIPLE_FINAL)
    {
        reached_final.clear();
    }

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
                if (MULTIPLE_FINAL)
                {
                    reached_final.insert(curr_first_node.second.id);
                }
                auto pointer_to_reached_state = &seen[curr_first_node.second].find(curr_first_node)->secod;
                auto path_id = path_manager.set_path(pointer_to_reached_state, path_var);
                parent_binding->add(path_var, path_id);
                parent_binding->add(end, curr_first_node.second);
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

template <bool MULTIPLE_FINAL>
const MultiSourceSearchState *BFSMultipleStarts<MULTIPLE_FINAL>::expand_neighbors(const SearchNodeId &current_state_id)
{
    // Check if this is the first time that current_state is explored
    if (iter->at_end())
    {
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

            auto bfses_to_be_visited_by_current_state = bfses_to_be_visited[current_state_id];
            SearchNodeId new_node_id = {transition.to, ObjectId(iter->get_reached_node())};
            auto bfses_that_reached_the_new_state = bfss_that_reached_given_node[new_node_id];

            boost::unordered_node_set<ObjectId, objectid_hash> difference;
            boost::set_difference(bfses_to_be_visited_by_current_state, bfses_that_reached_the_new_state, std::inserter(difference, difference.end()));

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
            }
            // Check if new path is solution
            if (automaton.is_final_state[new_node_id.first])
            {
                for (const auto &bfs_to_visit_new_state_next : difference)
                {
                    start_nodes_for_current_iteration.push(bfs_to_visit_new_state_next);
                }
                node_for_current_iteration = new_node_id;
                if (MULTIPLE_FINAL)
                {
                    auto node_reached_final = reached_final.find(new_node_id.second.id);
                    if (node_reached_final == reached_final.end())
                    {
                        reached_final.insert(new_node_id.second.id);
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
                else
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
