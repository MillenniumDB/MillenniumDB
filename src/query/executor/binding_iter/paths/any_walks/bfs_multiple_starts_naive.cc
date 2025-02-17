#include "bfs_multiple_starts_naive.h"
#include <iostream>
#include <boost/range/algorithm/set_algorithm.hpp>

#include "graph_models/quad_model/quad_object_id.h"
#include "graph_models/quad_model/quad_model.h"
#include "system/path_manager.h"


using namespace std;
using namespace Paths::Any;

template <bool MULTIPLE_FINAL>
void BFSMultipleStartsNaive<MULTIPLE_FINAL>::_begin(Binding &_parent_binding)
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
        start_nodes_q.push(start_object_id);
    }
    set_new_start_node();
    auto state_inserted = visited.emplace(automaton.start_state,
                                          current_start_node,
                                          nullptr,
                                          true,
                                          ObjectId::get_null());
    open.push(state_inserted.first.operator->());
    iter = make_unique<NullIndexIterator>();
}

template <bool MULTIPLE_FINAL>
void BFSMultipleStartsNaive<MULTIPLE_FINAL>::set_new_start_node()
{
    assert(start_nodes_q.size() >= 1);
    current_start_node = start_nodes_q.front();
    start_nodes_q.pop();
}
template <bool MULTIPLE_FINAL>
bool BFSMultipleStartsNaive<MULTIPLE_FINAL>::single_next()
{
    // Check if first state is final
    if (first_next)
    {
        first_next = false;
        auto current_state = open.front();

        // Return false if node does not exist in the database
        if (!provider->node_exists(current_state->node_id.id))
        {
            open.pop();
            return false;
        }

        // Starting state is solution
        if (automaton.is_final_state[automaton.start_state])
        {
            auto reached_state = SearchState(automaton.start_state,
                                             current_state->node_id,
                                             nullptr,
                                             true,
                                             ObjectId::get_null());
            if (MULTIPLE_FINAL)
            {
                reached_final.insert(current_state->node_id.id);
            }
            auto path_id = path_manager.set_path(visited.insert(reached_state).first.operator->(), path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state->node_id);
            return true;
        }
    }

    while (open.size() > 0)
    {
        auto current_state = open.front();
        auto reached_final_state = expand_neighbors(*current_state);

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
            // Pop and visit next state
            open.pop();
        }
    }
    return false;
}

template <bool MULTIPLE_FINAL>
void BFSMultipleStartsNaive<MULTIPLE_FINAL>::single_reset()
{
    // Empty open and visited
    queue<const SearchState *> empty;
    open.swap(empty);
    visited.clear();
    if (MULTIPLE_FINAL)
    {
        reached_final.clear();
    }
    first_next = true;

    auto state_inserted = visited.emplace(automaton.start_state,
                                          current_start_node,
                                          nullptr,
                                          true,
                                          ObjectId::get_null());
    open.push(state_inserted.first.operator->());
    iter = make_unique<NullIndexIterator>();
}

template <bool MULTIPLE_FINAL>
void BFSMultipleStartsNaive<MULTIPLE_FINAL>::_reset()
{

    for (auto node : start_nodes)
    {
        ObjectId start_object_id = node.is_var() ? (*parent_binding)[node.get_var()] : node.get_OID();
        start_nodes_q.push(start_object_id);
    }
    set_new_start_node();
    single_reset();
}

template <bool MULTIPLE_FINAL>
bool BFSMultipleStartsNaive<MULTIPLE_FINAL>::_next()
{
    if (single_next()) {
        return true;
    }
    if (!start_nodes_q.empty()) {
        set_new_start_node();
        single_reset();
        return _next();
    } else {
        return false;
    }
}

template <bool MULTIPLE_FINAL>
const SearchState *BFSMultipleStartsNaive<MULTIPLE_FINAL>::expand_neighbors(const SearchState &current_state)
{
    // Check if this is the first time that current_state is explored
    if (iter->at_end())
    {
        current_transition = 0;
        // Check if automaton state has transitions
        if (automaton.from_to_connections[current_state.automaton_state].size() == 0)
        {
            return nullptr;
        }
        set_iter(current_state);
    }

    // Iterate over the remaining transitions of current_state
    // Don't start from the beginning, resume where it left thanks to current_transition and iter (pipeline)
    while (current_transition < automaton.from_to_connections[current_state.automaton_state].size())
    {
        auto &transition = automaton.from_to_connections[current_state.automaton_state][current_transition];

        // Iterate over records until a final state is reached
        while (iter->next())
        {
            SearchState next_state(transition.to,
                                   ObjectId(iter->get_reached_node()),
                                   &current_state,
                                   transition.inverse,
                                   transition.type_id);
            auto visited_state = visited.insert(next_state);

            // If next state was visited for the first time
            if (visited_state.second)
            {
                auto reached_state = visited_state.first;
                open.push(reached_state.operator->());

                // Check if new path is solution
                if (automaton.is_final_state[reached_state->automaton_state])
                {
                    if (MULTIPLE_FINAL)
                    {
                        auto node_reached_final = reached_final.find(reached_state->node_id.id);
                        if (node_reached_final == reached_final.end())
                        {
                            reached_final.insert(reached_state->node_id.id);
                            return reached_state.operator->();
                        }
                    }
                    else
                    {
                        return reached_state.operator->();
                    }
                }
            }
        }

        // Construct new iter with the next transition (if there exists one)
        current_transition++;
        if (current_transition < automaton.from_to_connections[current_state.automaton_state].size())
        {
            set_iter(current_state);
        }
    }
    return nullptr;
}

template <bool MULTIPLE_FINAL>
void BFSMultipleStartsNaive<MULTIPLE_FINAL>::accept_visitor(BindingIterVisitor &visitor)
{
    visitor.visit(*this);
}

template class Paths::Any::BFSMultipleStartsNaive<true>;
template class Paths::Any::BFSMultipleStartsNaive<false>;
