#pragma once

#include <array>
#include <memory>
#include <stack>
#include <variant>

#include "base/binding/binding_id_iter.h"
#include "base/thread/thread_info.h"
#include "parser/query/paths/path_automaton.h"
#include "execution/binding_id_iter/paths/any_shortest/search_state.h"
#include "execution/binding_id_iter/scan_ranges/scan_range.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "third_party/robin_hood/robin_hood.h"

/*
DFSEnum enumerates paths from/to a specifc node using DFS algorithm.

Open memory usage is linear, but shortest path is not guaranteed.
This can ralentize the search due to long paths that must be constructed
*/
namespace Paths { namespace Any {

struct DFSSearchState {
    const uint32_t state;
    const ObjectId object_id;

    uint32_t current_transition = 0;

    std::unique_ptr<BptIter<4>> iter = nullptr;

    DFSSearchState(uint32_t state, ObjectId object_id) : state(state), object_id(object_id) { }
};


class DFSEnum : public BindingIdIter {
private:
    // Attributes determined in the constuctor
    ThreadInfo*   thread_info;
    BPlusTree<1>& nodes;
    BPlusTree<4>& type_from_to_edge; // Used to search foward
    BPlusTree<4>& to_type_from_edge; // Used to search backward
    VarId         path_var;
    Id            start;
    VarId         end;
    PathAutomaton automaton;

    // Attributes determined in begin
    BindingId* parent_binding;
    bool       first_next = true;

    // Ranges to search in BPT. They are not local variables because some positions are reused.
    std::array<uint64_t, 4> min_ids;
    std::array<uint64_t, 4> max_ids;

    // Structs for BFS
    robin_hood::unordered_node_set<Paths::AnyShortest::SearchState> visited;

    std::stack<DFSSearchState> open;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t bpt_searches  = 0;

    robin_hood::unordered_node_set<Paths::AnyShortest::SearchState>::iterator
      current_state_has_next(DFSSearchState& current_state);

    void set_iter(DFSSearchState& current_state);

public:
    DFSEnum(ThreadInfo*   thread_info,
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
}} // namespace Paths::Any
