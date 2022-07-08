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
        cases 2 and 3 this migh be constructed based on the value already
        stored in the variable start or end. Gets assigned in begin()
*/
#pragma once

#include <array>
#include <memory>
#include <queue>
#include <variant>

#include "base/binding/binding_id_iter.h"
#include "base/thread/thread_info.h"
#include "parser/query/paths/automaton/rpq_automaton.h"
#include "execution/binding_id_iter/paths/any_shortest/search_state.h"
#include "execution/binding_id_iter/scan_ranges/scan_range.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AnyShortest {

/*
BFSCheck will determine if there exists a path between two fixed
nodes using clasical BFS algorithm to explore the database.
*/
class BFSCheck : public BindingIdIter {
private:
    // Attributes determined in the constuctor
    ThreadInfo*  thread_info;
    VarId        path_var;
    Id           start;
    Id           end;
    RPQAutomaton automaton;

    // Attributes determined in begin
    BindingId* parent_binding;
    ObjectId end_object_id;
    bool is_first;  // true in the first call of next

    // Ranges to search in BPT. They are not local variables because some positions are reused.
    std::array<uint64_t, 4> min_ids;
    std::array<uint64_t, 4> max_ids;

    // Structs for BFS
    robin_hood::unordered_node_set<SearchState> visited;
    // open stores a pointer to a SearchState stored in visited
    // that allows to avoid use visited.find to get a pointer and
    // use the state extracted of the open directly.
    std::queue<const SearchState*> open;
    std::unique_ptr<BptIter<4>> iter;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t bpt_searches = 0;

    // Constructs iter according to transition
    void set_iter(const Transition& transition, const SearchState* current_state);

public:
    BFSCheck(ThreadInfo*  thread_info,
             VarId        path_var,
             Id           start,
             Id           end,
             RPQAutomaton automaton);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    void reset() override;
    inline void assign_nulls() override { };
    bool next() override;
};
}} // namespace Paths::AnyShortest
