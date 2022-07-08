/*
This is the implementation of BFSSimpleEnum BindingID iterator,
which uses BFS in order to find results of a single property path query.

More theoretical details on the deployed algorithms can be found in:
Evaluating Navigational RDF Queries over the Web, HT2017
https://doi.org/10.1145/3078714.3078731

The main components of the algorithm can be summarized as follows:
1. The property path query is defined by a regular expression, so it gets
   converted into a finite state automaton (see the class YYY for detail).
2. The query has either a startNode or endNode specified, either in the
   query itself, or by an operator that piped results to it previously.
   Recall that this is always stored in start (even when the end node
   is assigned, in which case we run the regex in "reverse").
3. The graph database is viewed as an automaton itself.
4. A "virtual" cross product of automaton states and graph nodes is
   "constructed" (virtual since it is never materialized, but fetched
   on the fly).
5. An edge in this new graph exists between (nodeID1,automatonState1) and
   (nodeID2,automatonState2) if in our graph there is a connection
   (nodeID1,type,nodeID2,edge), and (automatonState1,type,automatonState2),
   is a transition in the automaton.
6. A BFS is done from (startNode,initState), where startNode is the starting
   node for the query (if specified), and initState is the initial state
   of the query automaton (if endNode is specified we run the automaton for
   the reverse regex).
7. A result is found if a pair (nodeID,finalState) is found in this product
   graph, where finalState is an accepting state of the automaton.

Another way of looking at the procedure is as follows:
(assume startNode is given as ID1, and the pattern is (ID1)=[a*]=>(?x))
1. We start with (ID1,initState) pushed onto the queue "open" (a class variable)
2. A B+ tree iter that looks for all nodes reachable from ID1 by an a-typed
   connection is constructed and iterated on. This is in "type_from_to_edge"
   B+ tree.
3. If IDn is a reached node, and (initState,a,nextState) is a transition in the
   query automaton, we:
    - check if (IDn,stateNext) was visited before
      (we store this in a set called "visited")
    - we push (IDn,stateNext) onto the queue called "open" (classic BFS)
    - we put (IDn,stateNext) in "visited" to not revisit it again
4. If the state we were processing is of the form (ID, endState)
   we can return a result.
5. Repeat (from step 2) until open is not empty

There is a glaring inefficiency in this algorithm: we return the result ID only
after iterating over all of it neighbours reached via an a-typed edge. This means
That we could have to do a large iteration while already having a result in hand,
in order to not lose the neighbours of the node. We solve this problem in a version
implemented in PropertyPathBFSIterEnum (see documentation there).

The is_first flag is revised to check if we are in the very first node, and when this
node can already be a result (such as in (Q1)=[a*]=>(?x), where a zero-length path
conforming to a* matches a path between Q1 and Q1).
*/

#include "bfs_simple_enum.h"

#include <cassert>

#include "base/ids/var_id.h"
#include "execution/binding_id_iter/paths/path_manager.h"
#include "query_optimizer/quad_model/quad_model.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/bplus_tree/bplus_tree_leaf.h"
#include "storage/index/record.h"

using namespace std;
using namespace Paths::AnyShortest;

BFSSimpleEnum::BFSSimpleEnum(ThreadInfo*   thread_info,
                             VarId         path_var,
                             Id            start,
                             VarId         end,
                             RPQAutomaton automaton) :
    thread_info       (thread_info),
    path_var          (path_var),
    start             (start),
    end               (end),
    automaton         (automaton) { }


void BFSSimpleEnum::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;

    // Create start object id
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start)
                             ? std::get<ObjectId>(start)
                             : (*parent_binding)[std::get<VarId>(start)]);

    // Add start object to open and visited
    open.emplace(automaton.get_start(),
                 start_object_id,
                 nullptr,
                 true,
                 ObjectId::get_null());

    visited.emplace(automaton.get_start(),
                    start_object_id,
                    nullptr,
                    true,
                    ObjectId::get_null());

    is_first = true;
    min_ids[2] = 0;
    max_ids[2] = 0xFFFFFFFFFFFFFFFF;
    min_ids[3] = 0;
    max_ids[3] = 0xFFFFFFFFFFFFFFFF;
}


bool BFSSimpleEnum::next() {
    // BFS classic implementation
    while (open.size() > 0) {
        auto& current_state = open.front();
        // Expand state. Explore reachable nodes with automaton transitions
        for (const auto& transition : automaton.from_to_connections[current_state.automaton_state]) {
            // Constructs iter with current automaton transition
            auto iter = set_iter(transition, current_state);
            auto child_record = iter->next();

            // Explore nodes
            while (child_record != nullptr) {
                auto next_state = SearchState(transition.to,
                                              ObjectId(child_record->ids[2]),
                                              visited.find(current_state).operator->(),
                                              transition.inverse,
                                              transition.type_id);

                // Check if this node has been already visited
                if (visited.find(next_state) == visited.end()) {
                    // Add to open and visited set
                    open.push(next_state);
                    visited.insert(next_state);
                }
                child_record = iter->next();
            }
        }
        if (is_first) {
            is_first = false;
            auto start_node_iter = quad_model.nodes->get_range(&thread_info->interruption_requested,
                                                               Record<1>({current_state.node_id.id}),
                                                               Record<1>({current_state.node_id.id}));

            // Return false if node does not exists in bd
            if (start_node_iter->next() == nullptr) {
                open.pop();
                return false;
            }
            if (current_state.automaton_state == automaton.get_start() &&
                automaton.start_is_final)
            {
                results_found++;
                auto path_object_id = path_manager.set_path(visited.find(current_state).operator->(),
                                                            path_var);

                parent_binding->add(path_var, path_object_id);
                parent_binding->add(end, current_state.node_id);
                open.pop();  // Pop to visit next state
                return true;
            }
        }
        // Check if current state is final
        else if (current_state.automaton_state == automaton.get_final_state()) {
            results_found++;
            auto path_object_id = path_manager.set_path(visited.find(current_state).operator->(),
                                                        path_var);

            parent_binding->add(path_var, path_object_id);
            parent_binding->add(end, current_state.node_id);
            open.pop(); // Pop to visit next state
            return true;
        }
        open.pop(); // Pop to visit next state
    }
    return false;
}


unique_ptr<BptIter<4>> BFSSimpleEnum::set_iter(const Transition& transition,
                                               const SearchState& current_state)
{
    unique_ptr<BptIter<4>> iter = nullptr;
    // Get iter from correct bpt_tree according to inverse attribute
    if (transition.inverse) {
        min_ids[0] = current_state.node_id.id;
        max_ids[0] = current_state.node_id.id;
        min_ids[1] = transition.type_id.id;
        max_ids[1] = transition.type_id.id;
        iter = quad_model.to_type_from_edge->get_range(&thread_info->interruption_requested,
                                                       Record<4>(min_ids),
                                                       Record<4>(max_ids));
    } else {
        min_ids[0] = transition.type_id.id;
        max_ids[0] = transition.type_id.id;
        min_ids[1] = current_state.node_id.id;
        max_ids[1] = current_state.node_id.id;
        iter = quad_model.type_from_to_edge->get_range(&thread_info->interruption_requested,
                                                       Record<4>(min_ids),
                                                       Record<4>(max_ids));
    }
    bpt_searches++;
    return iter;
}


void BFSSimpleEnum::reset() {
    // Empty open and visited
    queue<SearchState> empty;
    open.swap(empty);
    visited.clear();
    is_first = true;

    // Add start object id to open and visited structures
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start)
                             ? std::get<ObjectId>(start)
                             : (*parent_binding)[std::get<VarId>(start)]);

    open.emplace(automaton.get_start(),
                 start_object_id,
                 nullptr,
                 true,
                 ObjectId::get_null());

    visited.emplace(automaton.get_start(),
                    start_object_id,
                    nullptr,
                    true,
                    ObjectId::get_null());
}


void BFSSimpleEnum::assign_nulls() {
    parent_binding->add(end, ObjectId::get_null());
}


void BFSSimpleEnum::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AnyShortest::BFSSimpleEnum(bpt_searches: " << bpt_searches
       << ", found: " << results_found << ")";
}
