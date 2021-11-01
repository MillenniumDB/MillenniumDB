#ifndef RELATIONAL_MODEL__PROPERTY_PATH_DFS_ITER_ENUM_H_
#define RELATIONAL_MODEL__PROPERTY_PATH_DFS_ITER_ENUM_H_

#include <array>
#include <memory>
#include <unordered_set>
#include <stack>
#include <variant>

#include "base/binding/binding_id_iter.h"
#include "base/parser/logical_plan/op/property_paths/path_automaton.h"
#include "relational_model/models/quad_model/quad_model.h"
#include "relational_model/execution/binding_id_iter/property_paths/search_state.h"
#include "relational_model/execution/binding_id_iter/scan_ranges/scan_range.h"
#include "storage/index/bplus_tree/bplus_tree.h"

/*
PropertyPathDFSIterEnum enumerates paths from/to a specifc node using DFS algorithm.

Open memory usage is linear, but shortest path is not guaranteed.
This can ralentize the search due to long paths that must be constructed
*/


namespace DFSIterEnum {
struct State {
    const uint32_t state;
    const ObjectId object_id;
    uint32_t current_transition = 0;
    std::unique_ptr<BptIter<4>> iter = nullptr;

    State(uint32_t state, ObjectId object_id) :
        state     (state),
        object_id (object_id) { }

};
}


class PropertyPathDFSIterEnum : public BindingIdIter {
    using Id = std::variant<VarId, ObjectId>;

private:
    // Attributes determined in the constuctor
    ThreadInfo*   thread_info;
    BPlusTree<1>& nodes;
    BPlusTree<4>& type_from_to_edge;  // Used to search foward
    BPlusTree<4>& to_type_from_edge;  // Used to search backward
    VarId         path_var;
    Id            start;
    VarId         end;
    PathAutomaton automaton;

    // Attributes determined in begin
    BindingId* parent_binding;
    bool first_next = true;

    // Ranges to search in BPT. They are not local variables because some positions are reused.
     std::array<uint64_t, 4> min_ids;
     std::array<uint64_t, 4> max_ids;

    // Structs for BFS
    std::unordered_set<SearchState, SearchStateHasher> visited;
    std::stack<DFSIterEnum::State> open;

    // Statistics
    uint_fast32_t results_found = 0;
    uint_fast32_t bpt_searches = 0;

    std::unordered_set<SearchState, SearchStateHasher>::iterator
        current_state_has_next(DFSIterEnum::State& current_state);

    void set_iter(DFSIterEnum::State& current_state);

public:
    PropertyPathDFSIterEnum(ThreadInfo*   thread_info,
                            BPlusTree<1>& nodes,
                            BPlusTree<4>& type_from_to_edge,
                            BPlusTree<4>& to_type_from_edge,
                            VarId         path_var,
                            Id            start,
                            VarId         end,
                            PathAutomaton automaton);
    ~PropertyPathDFSIterEnum() = default;

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(BindingId& parent_binding) override;
    void reset() override;
    void assign_nulls() override;
    bool next() override;
};

#endif // RELATIONAL_MODEL__PROPERTY_PATH_DFS_ITER_ENUM_H_
