#include "path_manager.h"

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

        // Fill structures
        paths_materialized.push_back(false);
        paths.push_back(path_vector);
        states_materialized.push_back(materialized_path_states);
        available_index.push(i);
    }
}


void PathManager::init(uint_fast32_t max_threads) {
    new (&path_manager) PathManager(max_threads); // placement new
}


void PathManager::begin(size_t binding_size, bool materialize) {
    // Get next available index
    std::thread::id thread_id = std::this_thread::get_id();
    uint_fast32_t index;
    {
        // Avoid sychronizations problems with extractions and insertions
        std::lock_guard<std::mutex> lck(lock_mutex);
        index = available_index.front();
        available_index.pop();

        // Assign index to thread_id
        thread_paths.insert({thread_id, index});
    }

    // Set if path will be materialized
    paths_materialized[index] = materialize;

    // Assign a space to unordered sets to materialize
    if (materialize) {
        for (size_t i = 0; i < binding_size; i++) {
            states_materialized[index].emplace_back();
        }
        // Create vector to save materialized paths
        paths[index] = std::vector<const void*>();
    } else {
        // Assign space to save paths
        paths[index] = std::vector<const void*>(binding_size);
    }
}


ObjectId PathManager::set_path(const Paths::AnyShortest::SearchState* visited_pointer, VarId path_var) {
    std::thread::id thread_id = std::this_thread::get_id();
    uint_fast32_t index;
    {
        // Avoid to acces a not consistent pointer with find()
        std::lock_guard<std::mutex> lck(lock_mutex);
        index = thread_paths.find(thread_id)->second;
    }
    auto materialize = paths_materialized[index];
    if (materialize) { // visited_pointer will be not valid
        // Stores not added states
        std::stack<Paths::AnyShortest::SearchState*> missing_states;
        Paths::AnyShortest::SearchState* current_state = const_cast<Paths::AnyShortest::SearchState*>(visited_pointer);
        auto& states_set = states_materialized[index][path_var.id];
        Paths::AnyShortest::SearchState* previous = nullptr;
        // Get all the path
        while (current_state != nullptr) {
            // Check if state has been added
            if (states_set.find(*current_state) == states_set.end()) {
                missing_states.push(current_state);
                current_state = const_cast<Paths::AnyShortest::SearchState*>(current_state->previous);
            } else {
                // Previous allow connect this paths to the rest of states of a longer path
                previous = const_cast<Paths::AnyShortest::SearchState*>(states_set.find(*current_state).operator->());
                // Stop iterating when find a states that has been already added
                break;
            }
        }

        // Path's states will be saved in order in topological order
        current_state = missing_states.top();
        // Path first element is also the first element of the path to return
        while (!missing_states.empty()) {
            current_state = missing_states.top();
            auto new_state = Paths::AnyShortest::SearchState(
                current_state->automaton_state,
                current_state->node_id,
                previous,
                current_state->inverse_direction,
                current_state->type_id
            );
            missing_states.pop();
            // Previous points to path manager set
            previous = const_cast<Paths::AnyShortest::SearchState*>(
                states_set.insert(new_state).first.operator->());
        }
        // Points to last element of set
        uint64_t path_id = paths[index].size();
        paths[index].push_back(states_set.find(*visited_pointer).operator->());
        return ObjectId(ObjectId::VALUE_PATH_MASK | path_id);
    } else {
        // Save visited pointer directly, visited_pointer always is valid
        paths[index][path_var.id] = visited_pointer;
        return ObjectId(ObjectId::VALUE_PATH_MASK | path_var.id);
    }
}

ObjectId PathManager::set_path(const Paths::AnyShortest::SearchStateDijkstra* visited_pointer, VarId path_var) {
    std::thread::id thread_id = std::this_thread::get_id();
    uint_fast32_t index;
    {
        // Avoid to acces a not consistent pointer with find()
        std::lock_guard<std::mutex> lck(lock_mutex);
        index = thread_paths.find(thread_id)->second;
    }
    // Save visited pointer directly, visited_pointer always is valid
    paths[index][path_var.id] = visited_pointer;
    return ObjectId(ObjectId::VALUE_PATH_MASK | DIJKSTRA_MASK | path_var.id);
}

ObjectId PathManager::set_path(const Paths::AllShortest::SearchState* visited_pointer, VarId path_var) {
    std::thread::id thread_id = std::this_thread::get_id();
    uint_fast32_t index;
    {
        // Avoid to acces a not consistent pointer with find()
        std::lock_guard<std::mutex> lck(lock_mutex);
        index = thread_paths.find(thread_id)->second;
    }
    auto materialize = paths_materialized[index];
    if (materialize) { // visited_pointer will be not valid
        return ObjectId::get_null();
        // Stores not added states
        //std::stack<Search::SearchState*> missing_states;
        //Search::SearchState* current_state = const_cast<Search::SearchState*>(visited_pointer);
        //auto& states_set = states_materialized[index][path_var.id];
        //Search::SearchState* previous = nullptr;
        //// Get all the path
        //while (current_state != nullptr) {
        //    // Check if state has been added
        //    if (states_set.find(*current_state) == states_set.end()) {
        //        missing_states.push(current_state);
        //        current_state = const_cast<Search::SearchState*>(current_state->previous);
        //    } else {
        //        // Previous allow connect this paths to the rest of states of a longer path
        //        previous = const_cast<Search::SearchState*>(states_set.find(*current_state).operator->());
        //        // Stop iterating when find a states that has been already added
        //        break;
        //    }
        //}

        //// Path's states will be saved in order in topological order
        //current_state = missing_states.top();
        //// Path first element is also the first element of the path to return
        //while (!missing_states.empty()) {
        //    current_state = missing_states.top();
        //    auto new_state = Search::SearchState(
        //        current_state->state,
        //        current_state->object_id,
        //        previous,
        //        current_state->direction,
        //        current_state->label_id
        //    );
        //    missing_states.pop();
        //    // Previous points to path manager set
        //    previous = const_cast<Search::SearchState*>(
        //        states_set.insert(new_state).first.operator->());
        //}
        //// Points to last element of set
        //uint64_t path_id = paths[index].size();
        //paths[index].push_back(states_set.find(*visited_pointer).operator->());
        //return ObjectId(GraphModel::VALUE_PATH_MASK | path_id);
    } else {
        // Save visited pointer directly, visited_pointer always is valid
        paths[index][path_var.id] = visited_pointer;
        return ObjectId(ObjectId::VALUE_PATH_MASK | ALL_STATE_MASK | path_var.id);
    }
}


void PathManager::print(std::ostream& os, uint64_t path_id) const {
    std::thread::id thread_id = std::this_thread::get_id();
    auto index = thread_paths.find(thread_id)->second;
    // Recover state removing mask of path_id
    auto path_type = (path_id & PATH_TYPE_MASK);

    switch (path_type) {
    case SEARCH_STATE_MASK: {
        auto current_state = reinterpret_cast<const Paths::AnyShortest::SearchState*>(paths[index][path_id & PATH_INDEX_MASK]);
        current_state->get_path(os);
        break;
    }
    case ALL_STATE_MASK: {
        auto current_state = reinterpret_cast<const Paths::AllShortest::SearchState*>(paths[index][path_id & PATH_INDEX_MASK]);
        current_state->path_iter.get_path(current_state->node_id, os);
        break;
    }
    case DIJKSTRA_MASK: {
        auto current_state = reinterpret_cast<const Paths::AnyShortest::SearchStateDijkstra*>(paths[index][path_id & PATH_INDEX_MASK]);
        current_state->get_path(os);
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
        states_materialized[index].clear();
        // Paths materialized deletion is not necesary

        // Add new index to available
        available_index.push(index);
    }
}
