#pragma once

#include <array>
#include <memory>
#include <queue>
#include <variant>

#include "base/binding/binding_id_iter.h"
#include "base/thread/thread_info.h"
#include "parser/query/paths/path_automaton.h"
#include "execution/binding_id_iter/paths/any_shortest/search_state.h"
#include "execution/binding_id_iter/scan_ranges/scan_range.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AnyShortest {

/*
BFSSimpleEnum implements an iterator for evaluating a path which has a
fixed start point. It enumerates all nodes that can be reached from it using
the BFS algorithm adapted to work as a pipeline.

The starting point may be a constant from the query or a variable that is
supposed to be setted before executing this iterator (i.e before calling the
methods begin and reset).
*/
class BFSSimpleEnum : public BindingIdIter {
private:
    // Attributes determined in the constuctor
    ThreadInfo*   thread_info;
    BPlusTree<1>& nodes;
    BPlusTree<4>& type_from_to_edge; // Used to search foward
    BPlusTree<4>& to_type_from_edge; // Used to search backwards
    VarId         path_var;
    Id            start;
    VarId         end;
    PathAutomaton automaton;

    // Attributes determined in begin
    BindingId* parent_binding;

    bool is_first;

    // Ranges to search in BPT. They are not local variables because some positions are reused.
    std::array<uint64_t, 4> min_ids;
    std::array<uint64_t, 4> max_ids;

    // Structs for BFS
    robin_hood::unordered_node_set<SearchState> visited;

    std::queue<SearchState> open;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t bpt_searches  = 0;

    // Constructs iter according to transition
    std::unique_ptr<BptIter<4>> set_iter(const TransitionId& transition, const SearchState& current_state);

public:
    BFSSimpleEnum(ThreadInfo*   thread_info,
                  BPlusTree<1>& nodes,
                  BPlusTree<4>& type_from_to_edge,
                  BPlusTree<4>& to_type_from_edge,
                  VarId         path_var,
                  Id            start,
                  VarId         end,
                  PathAutomaton automaton);

    void analyze(std::ostream& os, int indent = 0) const override;

    void begin(BindingId& parent_binding) override;

    void reset() override;

    void assign_nulls() override;

    bool next() override;
};

}} // namespace Paths::AnyShortest
