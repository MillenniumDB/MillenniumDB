#include "path_manager.h"
#include "query/query_context.h"

#include <cassert>
#include <new>         // placement new
#include <type_traits> // aligned_storage
#include <stack>

// memory for the object
static typename std::aligned_storage<sizeof(PathManager), alignof(PathManager)>::type path_manager_buf;
// global object
PathManager& path_manager = reinterpret_cast<PathManager&>(path_manager_buf);

PathManager::PathManager(uint_fast32_t max_threads) {
    for (uint64_t i = 0; i < max_threads; i++) {
        std::vector<robin_hood::unordered_node_set<Paths::Any::SearchState>> materialized_path_states;
        std::vector<const void*> path_vector;
        std::vector<bool> begin_at_left_vector;

        // Fill structures
        paths.push_back(path_vector);
        begin_at_left.push_back(begin_at_left_vector);
    }
}


void PathManager::init(uint_fast32_t max_threads) {
    new (&path_manager) PathManager(max_threads); // placement new
}


void PathManager::begin(std::vector<bool>&& _begin_at_left) {
    auto binding_size = get_query_ctx().get_var_size();
    auto index = get_thread_index();
    // Assign space to save paths
    paths[index].resize(binding_size);
    begin_at_left[index] = std::move(_begin_at_left);
}


ObjectId PathManager::set_path(const Paths::Any::SearchState* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ANY_SHORTEST_WALKS_MASK | path_var.id);
}

ObjectId PathManager::set_path(const Paths::Any::MultiSourceSearchState* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ANY_SHORTEST_WALKS_MULTIPLE_STARTS_MASK | path_var.id);
}

ObjectId PathManager::set_path(const Paths::Any::DirectionalSearchState* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ANY_SHORTEST_WALKS_DIR_MASK | path_var.id);
}

ObjectId PathManager::set_path(const Paths::Any::SearchStateDijkstra* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | DIJKSTRA_MASK | path_var.id);
}


ObjectId PathManager::set_path(const Paths::AllShortest::SearchState* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ALL_SHORTEST_WALKS_MASK | path_var.id);
}


ObjectId PathManager::set_path(const Paths::AnySimple::PathState* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ANY_SIMPLE_MASK | path_var.id);
}


ObjectId PathManager::set_path(const Paths::AnySimple::SearchStateDFS* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ANY_SIMPLE_DFS_MASK | path_var.id);
}


ObjectId PathManager::set_path(const Paths::AllSimple::PathState* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ALL_SIMPLE_MASK | path_var.id);
}


ObjectId PathManager::set_path(const Paths::AllSimple::SearchStateDFS* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ALL_SIMPLE_DFS_MASK | path_var.id);
}


ObjectId PathManager::set_path(const Paths::AllShortestSimple::PathState* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ALL_SHORTEST_SIMPLE_MASK | path_var.id);
}


ObjectId PathManager::set_path(const Paths::AnyTrails::PathState* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ANY_TRAILS_MASK | path_var.id);
}


// ObjectId PathManager::set_path(const Paths::AnyTrails::DirectionalPathState* visited_pointer, VarId path_var) {
//     auto index = get_thread_index();
//     paths[index][path_var.id] = visited_pointer;
//     return ObjectId(ObjectId::MASK_PATH | ANY_TRAILS_DIR_MASK | path_var.id);
// }


ObjectId PathManager::set_path(const Paths::AnyTrails::SearchStateDFS* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ANY_TRAILS_DFS_MASK | path_var.id);
}


ObjectId PathManager::set_path(const Paths::AllTrails::PathState* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ALL_TRAILS_MASK | path_var.id);
}


ObjectId PathManager::set_path(const Paths::AllTrails::SearchStateDFS* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ALL_TRAILS_DFS_MASK | path_var.id);
}


ObjectId PathManager::set_path(const Paths::AllShortestTrails::PathState* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ALL_SHORTEST_TRAILS_MASK | path_var.id);
}


void PathManager::print(std::ostream& os,
                        uint64_t path_id,
                        std::function<void(std::ostream&, ObjectId)> print_node,
                        std::function<void(std::ostream&, ObjectId, bool)> print_edge) const
{
    auto index = get_thread_index();

    // Recover state removing mask of path_id
    auto path_type = (path_id & PATH_TYPE_MASK);

    auto decoded_id = path_id & PATH_INDEX_MASK;

    using namespace Paths;

    switch (path_type) {
    case ANY_SHORTEST_WALKS_MASK: {
        auto state = reinterpret_cast<const Any::SearchState*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ANY_SHORTEST_WALKS_DIR_MASK: {
        auto state = reinterpret_cast<const Any::DirectionalSearchState*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ALL_SHORTEST_WALKS_MASK: {
        auto state = reinterpret_cast<const AllShortest::SearchState*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case DIJKSTRA_MASK: {
        auto state = reinterpret_cast<const Paths::Any::SearchStateDijkstra*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ANY_SIMPLE_MASK: {
        auto state = reinterpret_cast<const Paths::AnySimple::PathState*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ANY_SIMPLE_DFS_MASK: {
        auto state = reinterpret_cast<const Paths::AnySimple::SearchStateDFS*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ALL_SIMPLE_MASK: {
        auto state = reinterpret_cast<const Paths::AllSimple::PathState*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ALL_SIMPLE_DFS_MASK: {
        auto state = reinterpret_cast<const Paths::AllSimple::SearchStateDFS*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ALL_SHORTEST_SIMPLE_MASK: {
        auto state = reinterpret_cast<const Paths::AllShortestSimple::PathState*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ANY_TRAILS_MASK: {
        auto state = reinterpret_cast<const Paths::AnyTrails::PathState*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    // case ANY_TRAILS_DIR_MASK: {
    //     auto state = reinterpret_cast<const Paths::AnyTrails::DirectionalPathState*>(paths[index][decoded_id]);
    //     state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
    //     break;
    // }
    case ANY_TRAILS_DFS_MASK: {
        auto state = reinterpret_cast<const Paths::AnyTrails::SearchStateDFS*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ALL_TRAILS_MASK: {
        auto state = reinterpret_cast<const Paths::AllTrails::PathState*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ALL_TRAILS_DFS_MASK: {
        auto state = reinterpret_cast<const Paths::AllTrails::SearchStateDFS*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ALL_SHORTEST_TRAILS_MASK: {
        auto state = reinterpret_cast<const Paths::AllShortestTrails::PathState*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ANY_SHORTEST_WALKS_MULTIPLE_STARTS_MASK: {
        auto state = reinterpret_cast<const Paths::Any::MultiSourceSearchState*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    default:
        break;
    }
}


uint_fast32_t PathManager::get_thread_index() const {
    return get_query_ctx().thread_info.worker_index;
}


// void PathManager::clear() {
//     auto index = get_thread_index();
//     paths[index].clear();
//     begin_at_left[index].clear();
// }
