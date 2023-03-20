/*
This is the implementation of BFSEnum BindingID iterator,
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
to return results as soon as they are discovered upon scanning the children of the
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

#include "bfs_enum.h"

#include <cassert>

#include "base/ids/var_id.h"
#include "execution/binding_id_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AnyShortest;

template <bool MULTIPLE_FINAL>
BFSEnum<MULTIPLE_FINAL>::BFSEnum(ThreadInfo* thread_info,
                                 VarId       path_var,
                                 Id          start,
                                 VarId       end,
                                 RPQ_DFA     automaton,
                                 unique_ptr<IndexProvider> provider) :
    thread_info (thread_info),
    path_var    (path_var),
    start       (start),
    end         (end),
    automaton   (automaton),
    provider    (move(provider)) { }


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::begin(BindingId& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);
    auto state_inserted = visited.emplace(automaton.start_state,
                                          start_object_id,
                                          nullptr,
                                          true,
                                          ObjectId::get_null());
    open.push(state_inserted.first.operator->());
}


template <bool MULTIPLE_FINAL>
bool BFSEnum<MULTIPLE_FINAL>::next() {
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
        if (automaton.is_final_state[automaton.start_state]) {
            auto reached_state = SearchState(automaton.start_state,
                                             current_state->node_id,
                                             nullptr,
                                             true,
                                             ObjectId::get_null());
            if (MULTIPLE_FINAL) {
                reached_final.insert(current_state->node_id.id);
            }
            auto path_id = path_manager.set_path(visited.insert(reached_state).first.operator->(), path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, current_state->node_id);
            results_found++;
            return true;
        }
    }

    // Enumerate
    while (open.size() > 0) {
        auto current_state = open.front();
        auto reached_final_state = expand_neighbors(current_state);

        // Enumerate reached solutions
        if (reached_final_state != nullptr) {
            auto path_id = path_manager.set_path(reached_final_state, path_var);
            parent_binding->add(path_var, path_id);
            parent_binding->add(end, reached_final_state->node_id);
            results_found++;
            return true;
        } else {
            // Pop and visit next state
            assert(iter->at_end());
            open.pop();
        }
    }
    return false;
}


template <bool MULTIPLE_FINAL>
const SearchState* BFSEnum<MULTIPLE_FINAL>::expand_neighbors(const SearchState* current_state) {
    // Check if this is the first time that current_state is explored
    if (iter->at_end()) {
        current_transition = 0;
        // Check if automaton state has transitions
        if (automaton.from_to_connections[current_state->automaton_state].size() == 0) {
            return nullptr;
        }
        set_iter(current_state);
    }

    // Iterate over the remaining transitions of current_state
    // Don't start from the beginning, resume where it left thanks to current_transition and iter (pipeline)
    while (current_transition < automaton.from_to_connections[current_state->automaton_state].size()) {
        auto& transition = automaton.from_to_connections[current_state->automaton_state][current_transition];

        // Iterate over records until a final state is reached
        while (iter->next()) {
            SearchState next_state(transition.to,
                                   ObjectId(iter->get_node()),
                                   current_state,
                                   transition.inverse,
                                   transition.type_id);
            auto visited_state = visited.insert(next_state);

            // If next state was visited for the first time
            if (visited_state.second) {
                auto reached_state = visited_state.first;
                open.push(reached_state.operator->());

                // Check if new path is solution
                if (automaton.is_final_state[reached_state->automaton_state]) {
                    if (MULTIPLE_FINAL) {
                        auto node_reached_final = reached_final.find(reached_state->node_id.id);
                        if (node_reached_final == reached_final.end()) {
                            reached_final.insert(reached_state->node_id.id);
                            return reached_state.operator->();
                        }
                    } else {
                        return reached_state.operator->();
                    }
                }
            }
        }

        // Construct new iter with the next transition (if there exists one)
        current_transition++;
        if (current_transition < automaton.from_to_connections[current_state->automaton_state].size()) {
            set_iter(current_state);
        }
    }
    return nullptr;
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::set_iter(const SearchState* current_state) {
    // Get current transition object from automaton
    const auto& transition = automaton.from_to_connections[current_state->automaton_state][current_transition];

    // Get iterator from custom index
    iter = provider->get_iterator(transition.type_id.id, transition.inverse, current_state->node_id.id);
    idx_searches++;
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::reset() {
    // Empty open and visited
    queue<const SearchState*> empty;
    open.swap(empty);
    visited.clear();
    if (MULTIPLE_FINAL) {
        reached_final.clear();
    }
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id(std::holds_alternative<ObjectId>(start) ? std::get<ObjectId>(start)
                                                                     : (*parent_binding)[std::get<VarId>(start)]);
    auto state_inserted = visited.emplace(automaton.start_state,
                                          start_object_id,
                                          nullptr,
                                          true,
                                          ObjectId::get_null());
    open.push(state_inserted.first.operator->());
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::assign_nulls() {
    parent_binding->add(end, ObjectId::get_null());
}


template <bool MULTIPLE_FINAL>
void BFSEnum<MULTIPLE_FINAL>::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "Paths::Any::BFSEnum(idx_searches: " << idx_searches << ", found: " << results_found <<")";
}


template class Paths::AnyShortest::BFSEnum<true>;
template class Paths::AnyShortest::BFSEnum<false>;
