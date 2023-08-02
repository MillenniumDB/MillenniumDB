#include "path_manager.h"

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
        std::vector<robin_hood::unordered_node_set<Paths::AnyShortest::SearchState>> materialized_path_states;
        std::vector<const void*> path_vector;
        std::vector<bool> begin_at_left_vector;

        // Fill structures
        // paths_materialized.push_back(false);
        paths.push_back(path_vector);
        begin_at_left.push_back(begin_at_left_vector);
        // states_materialized.push_back(materialized_path_states);
        available_index.push(i);
    }
}


void PathManager::init(uint_fast32_t max_threads) {
    new (&path_manager) PathManager(max_threads); // placement new
}


void PathManager::begin(size_t binding_size, std::vector<bool>&& _begin_at_left, bool /*materialize*/) {
    // Get next available index
    std::thread::id thread_id = std::this_thread::get_id();
    uint_fast32_t index;
    {
        // Avoid synchronizations problems with extractions and insertions
        std::lock_guard<std::mutex> lck(lock_mutex);
        if (available_index.size() < 1) {
            // TODO: wait for other threads to finish?
            throw QueryExecutionException("PathManger: available_index is empty");
        }
        index = available_index.front();
        available_index.pop();

        // Assign index to thread_id
        thread_paths.insert({thread_id, index});
    }

    // Set if path will be materialized
    // paths_materialized[index] = materialize;

    // Assign a space to unordered sets to materialize
    // if (materialize) {
        // for (size_t i = 0; i < binding_size; i++) {
        //     states_materialized[index].emplace_back();
        // }
        // // Create vector to save materialized paths
        // paths[index] = std::vector<const void*>();
    // } else {
        // Assign space to save paths
        paths[index] = std::vector<const void*>(binding_size);
        begin_at_left[index] = std::move(_begin_at_left);
    // }
}


ObjectId PathManager::set_path(const Paths::AnyShortest::SearchState* visited_pointer, VarId path_var) {
    auto index = get_thread_index();
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::MASK_PATH | ANY_SHORTEST_WALKS_MASK | path_var.id);
}


ObjectId PathManager::set_path(const Paths::AnyShortest::SearchStateDijkstra* visited_pointer, VarId path_var) {
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
                        void (*print_node) (std::ostream& os, ObjectId),
                        void (*print_edge) (std::ostream& os, ObjectId, bool inverse)) const
{
    std::thread::id thread_id = std::this_thread::get_id();
    auto index = thread_paths.find(thread_id)->second;

    // Recover state removing mask of path_id
    auto path_type = (path_id & PATH_TYPE_MASK);

    auto decoded_id = path_id & PATH_INDEX_MASK;

    using namespace Paths;

    switch (path_type) {
    case ANY_SHORTEST_WALKS_MASK: {
        auto state = reinterpret_cast<const AnyShortest::SearchState*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case ALL_SHORTEST_WALKS_MASK: {
        auto state = reinterpret_cast<const AllShortest::SearchState*>(paths[index][decoded_id]);
        state->print(os, print_node, print_edge, begin_at_left[index][decoded_id]);
        break;
    }
    case DIJKSTRA_MASK: {
        auto state = reinterpret_cast<const Paths::AnyShortest::SearchStateDijkstra*>(paths[index][decoded_id]);
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
    default:
        break;
    }
}

void PathManager::clear() {
    std::thread::id thread_id = std::this_thread::get_id();
    {
        // Avoid synchronization problems with deletions
        std::lock_guard<std::mutex> lck(lock_mutex);
        auto index = thread_paths.find(thread_id)->second;
        // Clean structures
        thread_paths.erase(thread_id);
        paths[index].clear();
        begin_at_left[index].clear();
        // states_materialized[index].clear();
        // Paths materialized deletion is not necessary

        // Add new index to available
        available_index.push(index);
    }
}
