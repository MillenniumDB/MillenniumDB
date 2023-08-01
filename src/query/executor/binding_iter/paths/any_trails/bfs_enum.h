#pragma once

#include <memory>
#include <queue>

#include "query/executor/binding_iter.h"
#include "query/id.h"
#include "query/executor/binding_iter/paths/any_trails/search_state.h"
#include "query/parser/paths/automaton/rpq_automaton.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AnyTrails {

/*
BFSEnum returns a single Trail to all
reachable nodes from a starting node, using BFS.
*/
class BFSEnum : public BindingIter {
private:
    // Attributes determined in the constructor
    VarId         path_var;
    Id            start;
    VarId         end;
    const RPQ_DFA automaton;
    std::unique_ptr<IndexProvider> provider;

    // Attributes determined in begin
    Binding* parent_binding;
    bool first_next = true;

    // Array of all trails (based on prefix tree + linked list idea)
    Visited visited;

    // Queue of search states
    std::queue<SearchState> open;

    // The index of the transition being currently explored
    uint32_t current_transition = 0;

    // Set iterator for current node + transition
    void set_iter(const SearchState& current_state);

   // Iterator for current node expansion
    std::unique_ptr<EdgeIter> iter;

    // Set of nodes reached with a final state
    robin_hood::unordered_set<uint64_t> reached_final;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t idx_searches = 0;

public:
    BFSEnum(
        VarId                          path_var,
        Id                             start,
        VarId                          end,
        RPQ_DFA                        automaton,
        std::unique_ptr<IndexProvider> provider
    ) :
        path_var      (path_var),
        start         (start),
        end           (end),
        automaton     (automaton),
        provider      (std::move(provider)) { }

    // Expand neighbors from current state
    const SearchState* expand_neighbors(const SearchState& current_state);

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    void reset() override;
    bool next() override;

    void assign_nulls() override {
        parent_binding->add(end, ObjectId::get_null());
        parent_binding->add(path_var, ObjectId::get_null());
    }
};
}} // namespace Paths::AnyTrails
