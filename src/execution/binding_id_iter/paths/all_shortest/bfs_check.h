#pragma once

#include <array>
#include <memory>
#include <queue>

#include "base/binding/binding_id_iter.h"
#include "base/ids/id.h"
#include "base/thread/thread_info.h"
#include "execution/binding_id_iter/paths/all_shortest/search_state.h"
#include "parser/query/paths/path_automaton.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "third_party/robin_hood/robin_hood.h"

namespace Paths { namespace AllShortest {

// returns all the shortest paths between two fixed nodes
class BFSCheck : public BindingIdIter {
private:
    // Attributes determined in the constuctor
    ThreadInfo*   thread_info;
    BPlusTree<1>& nodes;
    BPlusTree<4>& type_from_to_edge; // Used to search foward
    BPlusTree<4>& to_type_from_edge; // Used to search backward
    VarId         path_var;
    Id            start;
    Id            end;
    PathAutomaton automaton;

    // Attributes determined in begin
    BindingId* parent_binding;
    ObjectId   end_object_id;
    bool       is_first; // true in the first call of next

    // Ranges to search in BPT. They are not local variables because some positions are reused.
    std::array<uint64_t, 4> min_ids;
    std::array<uint64_t, 4> max_ids;

    // Structs for BFS
    robin_hood::unordered_node_set<SearchState, SearchStateHasher> visited;
    // open stores a pointer to a Paths::All::SearchState stored in visited
    // that allows to avoid use visited.find to get a pointer and
    // use the state extracted of the open directly.
    std::queue<const SearchState*> open;

    // Stores the children of state in expansion
    std::unique_ptr<BptIter<4>> iter;
    // The index of the transition that set_iter method uses to
    // construct iter attribute.
    uint32_t current_transition = 0;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t bpt_searches  = 0;

    // Shortest path finding
    uint32_t min_distance = UINT32_MAX;

    // Constructs iter according to transition
    void set_iter(const SearchState* current_state);

    std::pair<robin_hood::unordered_node_set<SearchState, SearchStateHasher>::iterator, bool>
      current_state_has_next(const SearchState* current_state);

public:
    BFSCheck(ThreadInfo*   thread_info,
             BPlusTree<1>& nodes,
             BPlusTree<4>& type_from_to_edge,
             BPlusTree<4>& to_type_from_edge,
             VarId         path_var,
             Id            start,
             Id            end,
             PathAutomaton automaton);

    void analyze(std::ostream& os, int indent = 0) const override;

    void begin(BindingId& parent_binding) override;

    void reset() override;

    bool next() override;

    void assign_nulls() override { }
};
}} // namespace Paths::AllShortest
