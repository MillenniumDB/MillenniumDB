#pragma once

#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "graph_models/object_id.h"
#include "query/var_id.h"
#include "query/executor/binding_iter/paths/all_shortest_simple/search_state.h"
#include "query/executor/binding_iter/paths/all_shortest_trails/search_state.h"
#include "query/executor/binding_iter/paths/all_shortest_walks/search_state.h"
#include "query/executor/binding_iter/paths/all_simple/search_state.h"
#include "query/executor/binding_iter/paths/all_trails/search_state.h"
#include "query/executor/binding_iter/paths/any_simple/search_state.h"
#include "query/executor/binding_iter/paths/any_trails/search_state.h"
#include "query/executor/binding_iter/paths/any_walks/search_state.h"
#include "query/executor/binding_iter/paths/experimental/search_state_dijkstra.h"
#include "third_party/robin_hood/robin_hood.h"

/*
PathManager manages the conversion from Path to ObjectId and ObjectId to Path.
Each query will run in its own thread, so PathManager assigns a slot in `paths`
*/
class PathManager {
public:
    // Used to know the type of the path
    static constexpr uint64_t PATH_TYPE_MASK = 0x00'FF'000000000000UL;
    // Used to know the id of the path
    static constexpr uint64_t PATH_INDEX_MASK = 0x00'00'FFFFFFFFFFFFUL;

    static constexpr uint64_t ANY_SHORTEST_WALKS_MASK      = 0x00'00'000000000000UL;
    static constexpr uint64_t ALL_SHORTEST_WALKS_MASK      = 0x00'01'000000000000UL;
    static constexpr uint64_t ALL_SIMPLE_MASK              = 0x00'02'000000000000UL;
    static constexpr uint64_t ALL_SIMPLE_DFS_MASK          = 0x00'03'000000000000UL;
    static constexpr uint64_t ALL_TRAILS_MASK              = 0x00'04'000000000000UL;
    static constexpr uint64_t ALL_TRAILS_DFS_MASK          = 0x00'05'000000000000UL;
    static constexpr uint64_t ANY_SIMPLE_MASK              = 0x00'06'000000000000UL;
    static constexpr uint64_t ANY_SIMPLE_DFS_MASK          = 0x00'07'000000000000UL;
    static constexpr uint64_t ANY_TRAILS_MASK              = 0x00'08'000000000000UL;
    static constexpr uint64_t ANY_TRAILS_DFS_MASK          = 0x00'09'000000000000UL;
    static constexpr uint64_t ALL_SHORTEST_SIMPLE_MASK     = 0x00'10'000000000000UL;
    static constexpr uint64_t ALL_SHORTEST_TRAILS_MASK     = 0x00'11'000000000000UL;
    static constexpr uint64_t ANY_SHORTEST_WALKS_DIR_MASK  = 0x00'13'000000000000UL;
    // static constexpr uint64_t ANY_TRAILS_DIR_MASK          = 0x00'14'000000000000UL;

    // experimental
    static constexpr uint64_t DIJKSTRA_MASK            = 0x00'12'000000000000UL;

    static void init(uint_fast32_t max_threads);

    // Assign space to save pointers to recover path
    // Every query that uses paths, need to call this method
    void begin(std::vector<bool>&& begin_at_left);

    ObjectId set_path(const Paths::Any::SearchState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::Any::DirectionalSearchState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::Any::SearchStateDijkstra* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AnySimple::PathState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AnyTrails::PathState* visited_pointer, VarId path_var);
    // ObjectId set_path(const Paths::AnyTrails::DirectionalPathState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AllSimple::PathState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AllTrails::PathState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AllShortest::SearchState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AllShortestSimple::PathState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AllShortestTrails::PathState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AnyTrails::SearchStateDFS* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AllSimple::SearchStateDFS* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AllTrails::SearchStateDFS* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AnySimple::SearchStateDFS* visited_pointer, VarId path_var);

    void print(std::ostream& os,
               uint64_t path_id,
               std::function<void(std::ostream& os, ObjectId)> print_node,
               std::function<void(std::ostream& os, ObjectId, bool inverse)> print_edge) const;

private:
    PathManager(uint_fast32_t max_threads);

    // Saves the pointer to path object
    // We use void* because the class type depends on the path type
    std::vector<std::vector<const void*>> paths;

    std::vector<std::vector<bool>> begin_at_left;

    uint_fast32_t get_thread_index() const;
};

extern PathManager& path_manager; // global object
