#pragma once

#include <memory>
#include <stack>

#include "base/binding/binding_id_iter.h"
#include "base/ids/id.h"
#include "base/thread/thread_info.h"
#include "execution/binding_id_iter/paths/any_trails/search_state.h"
#include "parser/query/paths/automaton/rpq_automaton.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AnyTrails {

/*
DFSEnum returns a single Trail to all
reachable nodes from a starting node, using DFS.
*/
class DFSEnum : public BindingIdIter {
private:
    // Attributes determined in the constructor
    ThreadInfo*   thread_info;
    VarId         path_var;
    Id            start;
    VarId         end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // Attributes determined in begin
    BindingId* parent_binding;
    bool first_next = true;

    // Stack for search states + paths
    std::stack<SearchStateDFS> open;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t idx_searches = 0;

    // Get next state of interest
    SearchStateDFS* current_state_has_next(SearchStateDFS& current_state);

    // Set iterator for current node + transition
    void set_iter(SearchStateDFS& current_state);

    // Reconstruct and check trail
    bool is_trail(const SearchStateDFS& path_state, ObjectId new_edge);

    // Set of nodes reached with a final state
    robin_hood::unordered_set<uint64_t> reached_final;

public:
    DFSEnum(ThreadInfo* thread_info,
            VarId       path_var,
            Id          start,
            VarId       end,
            RPQ_DFA     automaton,
            std::unique_ptr<IndexProvider> provider);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    void reset() override;
    void assign_nulls() override;
    bool next() override;
};
}} // namespace Paths::AnyTrails