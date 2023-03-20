/*
PropertyPathBFSCheck is a linear iterator used to evaluate property paths
in which both the start and the end node are already assigned.

This is the boolean version of PropertyPathBFSSimpleEnum iterator. Namely,
this iterator is used to evaluate a property path query in which both the
start and the end node are assigned. Notice that this can happen in various
different ways:
    1. In a query (Q1)=[regex]=>(Q2) this is specified in the query itself.
    2. In (Q1)=[regex]=>(?x), or (?x)=[regex]=>(Q1), the variable ?x might
    have been assigned in another iterator which piped its results to
    PropertyPathBFSCheck.
    3. In (?x)=[regex]=>(?y), both ?x and ?y might have been assigned in
    another iterator which piped its results to PropertyPathBFSCheck.

All of the class variables are the same as for PropertyPathBFSSimpleEnum,
except:
    - end:
        which is now an ID (either object or a variable), and not variable ID
    - end_object_id:
        This is the node we are looking for in the search, represented as
        object ID. In the case 1 above, this would be (the ID of) Q2, in
        cases 2 and 3 this might be constructed based on the value already
        stored in the variable start or end. Gets assigned in begin()
*/
#pragma once

#include <memory>
#include <queue>

#include "base/binding/binding_id_iter.h"
#include "base/ids/id.h"
#include "base/thread/thread_info.h"
#include "execution/binding_id_iter/paths/any_shortest/search_state.h"
#include "execution/binding_id_iter/paths/index_provider/path_index/path_index.h"
#include "parser/query/paths/automaton/rpq_automaton.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AnyShortest {

/*
BFSCheck checks if there's a path between two fixed nodes, using BFS.
*/
class BFSCheck : public BindingIdIter {
private:
    // Attributes determined in the constructor
    ThreadInfo*   thread_info;
    VarId         path_var;
    Id            start;
    Id            end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // Attributes determined in begin
    BindingId* parent_binding;
    ObjectId end_object_id;
    bool first_next = true;  // true in the first call of next

    // Structs for BFS

    // Set of visited SearchStates
    robin_hood::unordered_node_set<SearchState> visited;

    // Queue of pointers to a SearchState from visited
    std::queue<const SearchState*> open;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t idx_searches = 0;

    // Set iterator for current node + transition
    void set_iter(const RPQ_DFA::Transition& transition, const SearchState* current_state);

    // Iterator for index results
    std::unique_ptr<IndexIterator> iter;

public:
    BFSCheck(ThreadInfo* thread_info,
             VarId       path_var,
             Id          start,
             Id          end,
             RPQ_DFA     automaton,
             std::unique_ptr<IndexProvider> provider);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    void reset() override;
    void assign_nulls() override { };
    bool next() override;
};
}} // namespace Paths::AnyShortest
