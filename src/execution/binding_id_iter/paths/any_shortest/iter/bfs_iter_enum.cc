/*
This is the implementation of BFSIterEnum BindingID iterator,
which uses BFS in order to find results of a single property path query.
We note that this is the default property path evaluation algorithm in
MillenniumDB.

More theoretical details on the deployed algorithms can be found in:
Evaluating Navigational RDF Queries over the Web, HT2017
https://doi.org/10.1145/3078714.3078731

The main components of the algorithm can be summarized as follows:
1. The property path query is defined by a regular expression, so it gets
   converted into a finite state automaton (see the class PathAutomaton
   for details).
2. The query has either a startNode or endNode specified (but not both),
   either in the query itself, or by an operator that piped results to it
   previously. Recall that this is always stored in start (even when the end
   node is assigned, in which case we run the regex in "reverse").
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
2. The front of the queue open is extracted (but not popped). An appropriate
   SearchState iterator is constructed in the current_state_has_next function.
   In particular, for the query (ID1)=[a*]=>(?x) we want all the nodes reachable
   by an a-typed edge from ID1. The iterator will return SearchState of the form
   (ID',automatonState'), where (ID1,a,ID',e) is in from_type_to_edge relation,
   and (initState,a,automatonState) is a transition in the automaton.
   The SearchState iter takes care of all the possible transitions stemming from
   the state specified at the front of the queue before popping it, thus returning
   all the feasible next SearchStates in succession.
3. If (IDn,nextState) is returned by the SearchState iter, we:
    - check if (IDn,stateNext) was visited before and fetch the next one if it was
      (we store this in a set called "visited")
      -> note that this is checked inside the current_state_has_next iterator
    - we put (IDn,stateNext) in "visited" to not revisit it again
    - we push (IDn,stateNext) onto the queue called "open" (classic BFS)
    - in the case that stateNext is a final state, we can return the result
      (note that the B+tree iter stays alive in order to keep expanding from
      the appropriate child of the current node when looking for the next result)
4. Repeat (from step 2) until open is not empty

The SearchState iterator needs to be constructed and maintained in order to be able
to return results as soon as they are discovered upon scaning the children of the
current node. This makes the search efficient than simply pushing these states onto
the queue. For example in the query (ID1)=[a+]=>(?x), while scanning all the a-typed
edges stemming from ID1, each one will be a result, but if we were just to push them
onto the queue, we would need to scan all of them before the first result would be
returned, while we already have a result after a single next() call to the B+ tree
iter.

Note that the number of pages that need to be pinned in the buffer to execute this
algorithm is 1. Namely, we only need the page of the live B+ tree iterator, which
scans the children of the node Node in the SearchState=(Node,automatonState) that
is on the top of the queue open.

The first_next flag is revised to check if this is the first time that next is being
called. This is done to take care of the borderline case when the initial state of
the automaton is also the final state, so that the startNode of our search needs to
be returned (such as in (Q1)=[a*]=>(?x), where a zero-length path conforming to a*
matches a path between Q1 and Q1). Notice that we also check whether this node is
present in the database.

*/

#include "bfs_iter_enum.h"

#include <cassert>

#include "base/ids/var_id.h"
#include "execution/binding_id_iter/paths/path_manager.h"
#include "storage/index/record.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/bplus_tree/bplus_tree_leaf.h"

using namespace std;
using namespace Paths::AnyShortest;

BFSIterEnum::BFSIterEnum(ThreadInfo*   thread_info,
                         BPlusTree<1>& nodes,
                         BPlusTree<4>& type_from_to_edge,
                         BPlusTree<4>& to_type_from_edge,
                         VarId         path_var,
                         Id            start,
                         VarId         end,
                         PathAutomaton automaton) :
    thread_info       (thread_info),
    nodes             (nodes),
    type_from_to_edge (type_from_to_edge),
    to_type_from_edge (to_type_from_edge),
    path_var          (path_var),
    start             (start),
    end               (end),
    automaton         (automaton) { }


void BFSIterEnum::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;

    iter = nullptr;
    // Add start object id to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ?
        std::get<ObjectId>(start) :
        (*parent_binding)[std::get<VarId>(start)]);

    auto state_inserted = visited.emplace(automaton.get_start(),
                                            start_object_id,
                                            nullptr,
                                            true,
                                            ObjectId::get_null());

    open.push(state_inserted.first.operator->());

    min_ids[2] = 0;
    max_ids[2] = 0xFFFFFFFFFFFFFFFF;
    min_ids[3] = 0;
    max_ids[3] = 0xFFFFFFFFFFFFFFFF;
}


bool BFSIterEnum::next() {
    // Check if first node is final
    if (first_next) {
        first_next = false;

        const auto current_state = open.front();
        auto node_iter = nodes.get_range(&thread_info->interruption_requested,
                                         Record<1>({current_state->node_id.id}),
                                         Record<1>({current_state->node_id.id}));
        // Return false if node does not exists in bd
        if (node_iter->next() == nullptr) {
            open.pop();
            return false;
        }

        if (automaton.start_is_final) {
            auto reached_key = SearchState(automaton.get_final_state(),
                                           current_state->node_id,
                                           nullptr,
                                           true,
                                           ObjectId::get_null());

            auto path_id = path_manager.set_path(visited.insert(reached_key).first.operator->(), path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state->node_id);
            results_found++;
            return true;
        }
    }
    while (open.size() > 0) {
        const auto current_state = open.front();
        auto state_reached = current_state_has_next(current_state);
        // If has next state then state_reached does not point to visited.end()
        if (state_reached != visited.end()) {
            open.push(state_reached.operator->());

            if (state_reached->automaton_state == automaton.get_final_state()) {
                // set binding;
                auto path_id = path_manager.set_path(state_reached.operator->(), path_var);
                parent_binding->add(path_var, path_id);
                parent_binding->add(end, state_reached->node_id);
                results_found++;
                return true;
            }
        } else {
            // Pop and visit next state
            iter = nullptr;
            open.pop();
        }
    }
    return false;
}


robin_hood::unordered_node_set<SearchState>::iterator
    BFSIterEnum::current_state_has_next(const SearchState* current_state)
{
    if (iter == nullptr) { // if is first time that State is explore
        current_transition = 0;
        // Check automaton state has transitions
        if (current_transition >= automaton.transitions[current_state->automaton_state].size()) {
            return visited.end();
        }
        // Constructs iter
        set_iter(current_state);
    }
    // Iterate over automaton_start state transtions
    while (current_transition < automaton.transitions[current_state->automaton_state].size()) {
        auto& transition = automaton.transitions[current_state->automaton_state][current_transition];
        auto child_record = iter->next();
        // Iterate over next_childs
        while (child_record != nullptr) {

            auto next_state_key = SearchState(
                transition.to,
                ObjectId(child_record->ids[2]),
                current_state,
                transition.inverse,
                transition.type);

            auto inserted_state = visited.insert(next_state_key);
            // Inserted_state.second = true if state was inserted in visited
            if (inserted_state.second) {
                // Return pointer to state in visited
                return inserted_state.first;
            }
            child_record = iter->next();
        }
        // Constructs new iter
        current_transition++;
        if (current_transition < automaton.transitions[current_state->automaton_state].size()) {
            set_iter(current_state);
        }
    }
    return visited.end();
}


void BFSIterEnum::set_iter(const SearchState* current_state) {
    // Gets current transition object from automaton
    const auto& transition = automaton.transitions[current_state->automaton_state][current_transition];
    // Gets iter from correct bpt with transition.inverse
    if (transition.inverse) {
        min_ids[0] = current_state->node_id.id;
        max_ids[0] = current_state->node_id.id;
        min_ids[1] = transition.type.id;
        max_ids[1] = transition.type.id;
        iter = to_type_from_edge.get_range(&thread_info->interruption_requested,
                                           Record<4>(min_ids),
                                           Record<4>(max_ids));
    } else {
        min_ids[0] = transition.type.id;
        max_ids[0] = transition.type.id;
        min_ids[1] = current_state->node_id.id;
        max_ids[1] = current_state->node_id.id;
        iter = type_from_to_edge.get_range(&thread_info->interruption_requested,
                                           Record<4>(min_ids),
                                           Record<4>(max_ids));
    }
    bpt_searches++;
}


void BFSIterEnum::reset() {
    // Empty open and visited
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();
    first_next = true;
    iter = nullptr;

    // Add start object id to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ?
        std::get<ObjectId>(start) :
        (*parent_binding)[std::get<VarId>(start)]);

    auto state_inserted = visited.emplace(automaton.get_start(),
                                          start_object_id,
                                          nullptr,
                                          true,
                                          ObjectId::get_null());

    open.push(state_inserted.first.operator->());
}


void BFSIterEnum::assign_nulls() {
    parent_binding->add(end, ObjectId::get_null());
}


void BFSIterEnum::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::AnyShortest::BFSIterEnum(bpt_searches: " << bpt_searches
       << ", found: " << results_found <<")";
}
