#pragma once

#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "base/ids/object_id.h"
#include "base/ids/var_id.h"
#include "base/path_printer.h"
#include "execution/binding_id_iter/paths/all_shortest/search_state.h"
#include "execution/binding_id_iter/paths/all_simple/search_state.h"
#include "execution/binding_id_iter/paths/all_trails/search_state.h"
#include "execution/binding_id_iter/paths/any_shortest/search_state.h"
#include "execution/binding_id_iter/paths/any_shortest/experimental/search_state_dijkstra.h"
#include "execution/binding_id_iter/paths/any_simple/search_state.h"
#include "execution/binding_id_iter/paths/any_trails/search_state.h"
#include "third_party/robin_hood/robin_hood.h"

/*
PathManager manages the conversion from Path to ObjectId and ObjectId to Path.
Each query will run in its own thread, so PathManager assigns a slot in `paths`
*/
class PathManager : public PathPrinter {
public:
    // Usado para obtener el tipo de path
    static constexpr uint64_t PATH_TYPE_MASK = 0x00'FF'000000000000UL;
    // Usado para obtener el id del path
    static constexpr uint64_t PATH_INDEX_MASK = 0x00'00'FFFFFFFFFFFFUL;

    static constexpr uint64_t SEARCH_STATE_MASK   = 0x00'00'000000000000UL;
    static constexpr uint64_t ALL_STATE_MASK      = 0x00'01'000000000000UL;
    static constexpr uint64_t TWO_WAY_STATE_MASK  = 0x00'02'000000000000UL;
    static constexpr uint64_t DIJKSTRA_MASK       = 0x00'03'000000000000UL;
    static constexpr uint64_t ALL_SIMPLE_MASK     = 0x00'04'000000000000UL;
    static constexpr uint64_t ALL_SIMPLE_DFS_MASK = 0x00'05'000000000000UL;
    static constexpr uint64_t ALL_TRAILS_MASK     = 0x00'06'000000000000UL;
    static constexpr uint64_t ALL_TRAILS_DFS_MASK = 0x00'07'000000000000UL;
    static constexpr uint64_t ANY_SIMPLE_MASK     = 0x00'08'000000000000UL;
    static constexpr uint64_t ANY_SIMPLE_DFS_MASK = 0x00'09'000000000000UL;
    static constexpr uint64_t ANY_TRAILS_MASK     = 0x00'10'000000000000UL;
    static constexpr uint64_t ANY_TRAILS_DFS_MASK = 0x00'11'000000000000UL;

    static void init(uint_fast32_t max_threads);

    // Assign space to save pointers to recover path
    void begin(size_t binding_size, bool materialize);

    ObjectId set_path(const Paths::AnyShortest::SearchState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AnyShortest::SearchStateDijkstra* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AllShortest::SearchState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AnySimple::PathState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AnySimple::SearchStateDFS* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AnyTrails::PathState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AnyTrails::SearchStateDFS* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AllSimple::PathState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AllSimple::SearchStateDFS* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AllTrails::PathState* visited_pointer, VarId path_var);
    ObjectId set_path(const Paths::AllTrails::SearchStateDFS* visited_pointer, VarId path_var);

    void print(std::ostream& os, uint64_t path_id) const override;

    void print_shortest_path();
    void print_finding_path();

    void clear();

private:
    PathManager(uint_fast32_t max_threads);

    // Stores the available index to store a paths of a thread
    std::queue<uint_fast32_t> available_index;

    // Saves the index of paths of thread_id
    robin_hood::unordered_map<std::thread::id, uint_fast32_t> thread_paths;

    // Saves the pointer to path object
    // We use void* because the class type depends on the path type
    std::vector<std::vector<const void*>> paths;

    // Indicates which paths must be materialized
    std::vector<bool> paths_materialized;

    // Saves pointer when path must be materialized
    std::vector<std::vector<robin_hood::unordered_node_set<Paths::AnyShortest::SearchState>>> states_materialized;
    // TODO: materialize Paths::AllShortest::SearchState

    // To avoid synchronization problems
    std::mutex lock_mutex;
};

extern PathManager& path_manager; // global object
