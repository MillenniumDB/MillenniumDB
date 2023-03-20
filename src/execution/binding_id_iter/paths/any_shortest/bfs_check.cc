/*
This is a similar implementation to BFSSimpleEnum.

The first difference is the end condition, which is now determined not only
by reaching an end state of the automaton, but also the target node, whose
object ID we store in end_object_id.

The second and biggest difference, is that we need not scan all the neighbors
of a node in order to find the result. Namely, the process can be describe as
follows:
- We are looking for a path from start_object_id to end_object_id
- (start_object_id,initState) initializes BFS (both visited and open)
- If start_object_id == end_object_id, and initState is also a final state
  we can return a result and the execution halts.
- Else, we iterate over the neighbors of start_object_id node according to
  the automaton.
- If at any point we detect end_object_id we can return.

Notice that here the results can be returned as soon as detected, since we
are simply checking whether the two nodes are connected by a path.
*/

#include "bfs_check.h"

#include <cassert>

#include "base/ids/var_id.h"
#include "execution/binding_id_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AnyShortest;

BFSCheck::BFSCheck(ThreadInfo* thread_info,
                   VarId       path_var,
                   Id          start,
                   Id          end,
                   RPQ_DFA     automaton,
                   unique_ptr<IndexProvider> provider) :
    thread_info       (thread_info),
    path_var          (path_var),
    start             (start),
    end               (end),
    automaton         (automaton),
    provider          (move(provider)) { }


void BFSCheck::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);
    auto start_state = visited.emplace(automaton.start_state,
                                       start_object_id,
                                       nullptr,
                                       true,
                                       ObjectId::get_null());
    open.push(start_state.first.operator->());

    // Store ID for end object
    end_object_id = (std::holds_alternative<ObjectId>(end)) ? std::get<ObjectId>(end)
                                                            : (*parent_binding)[std::get<VarId>(end)];
}


bool BFSCheck::next() {
    // Check if first state is final
    if (first_next) {
        first_next = false;
        auto current_state = open.front();

        // Return false if node does not exist in the database
        if (!provider->node_exists(current_state->node_id.id)) {
            open.pop();
            return false;
        }

        // Starting state is solution
        if (automaton.is_final_state[automaton.start_state] && current_state->node_id == end_object_id) {
            auto path_id = path_manager.set_path(current_state, path_var);
            parent_binding->add(path_var, path_id);
            queue<const SearchState*> empty;
            open.swap(empty);
            results_found++;
            return true;
        }
    }

    // Enumerate
    while (open.size() > 0) {
        auto current_state = open.front();

        // Expand state
        for (const auto& transition : automaton.from_to_connections[current_state->automaton_state]) {
            set_iter(transition, current_state);

            // Explore matching states
            while (iter->next()) {
                auto next_state = SearchState(
                    transition.to,
                    ObjectId(iter->get_node()),
                    current_state,
                    transition.inverse,
                    transition.type_id);

                auto next_state_pointer = visited.insert(next_state);

                // Check if next_state was added to visited
                if (next_state_pointer.second) {
                    open.push(next_state_pointer.first.operator->());

                    // Check if next_state is final
                    if (automaton.is_final_state[next_state.automaton_state]
                        && next_state.node_id == end_object_id)
                    {
                        auto path_id = path_manager.set_path(next_state_pointer.first.operator->(),
                                                             path_var);
                        parent_binding->add(path_var, path_id);
                        queue<const SearchState*> empty;
                        open.swap(empty);
                        results_found++;
                        return true;
                    }
                }
            }
        }

        // Pop to visit next state
        assert(iter->at_end());
        open.pop();
    }
    return false;
}


void BFSCheck::set_iter(const RPQ_DFA::Transition& transition, const SearchState* current_state) {
    // Get iterator from custom index
    iter = provider->get_iterator(transition.type_id.id, transition.inverse, current_state->node_id.id);
    idx_searches++;
}


void BFSCheck::reset() {
    // Empty open and visited
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);
    auto start_state = visited.emplace(automaton.start_state,
                                       start_object_id,
                                       nullptr,
                                       true,
                                       ObjectId::get_null());
    open.push(start_state.first.operator->());

    // Store ID for end object
    end_object_id = (std::holds_alternative<ObjectId>(end)) ? std::get<ObjectId>(end)
                                                            : (*parent_binding)[std::get<VarId>(end)];
}


void BFSCheck::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::Any::BFSCheck(idx_searches: " << idx_searches << ", found: " << results_found <<")";
}
