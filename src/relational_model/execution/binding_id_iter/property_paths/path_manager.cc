#include "path_manager.h"

#include <new>         // placement new
#include <type_traits> // aligned_storage
#include <stack>
#include <iostream>

// memory for the object
static typename std::aligned_storage<sizeof(PathManager), alignof(PathManager)>::type path_manager_buf;
// global object
PathManager& path_manager = reinterpret_cast<PathManager&>(path_manager_buf);

PathManager::PathManager(GraphModel& model, uint_fast32_t max_threads) :
    model (model)
{
    for (uint64_t i = 0; i < max_threads; i++) {
        std::vector<std::unordered_set<SearchState, SearchStateHasher>> materialized_path_states;
        std::vector<const SearchState*> path_vector;

        // Fill structures
        paths_materialized.push_back(false);
        paths.push_back(path_vector);
        states_materialized.push_back(materialized_path_states);
        available_index.push(i);
    }
}


void PathManager::init(GraphModel& model, uint_fast32_t max_threads) {
    new (&path_manager) PathManager(model, max_threads); // placement new
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
        paths[index] = std::vector<const SearchState*>();
    } else {
        // Assign space to save paths
        paths[index] = std::vector<const SearchState*>(binding_size);
    }
}


ObjectId PathManager::set_path(const SearchState* visited_pointer, VarId path_var) {
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
        std::stack<SearchState*> missing_states;
        SearchState* current_state = const_cast<SearchState*>(visited_pointer);
        auto& states_set = states_materialized[index][path_var.id];
        SearchState* previous = nullptr;
        // Get all the path
        while (current_state != nullptr) {
            // Check if state has been added
            if (states_set.find(*current_state) == states_set.end()) {
                missing_states.push(current_state);
                current_state = const_cast<SearchState*>(current_state->previous);
            } else {
                // Previous allow connect this paths to the rest of states of a longer path
                previous = const_cast<SearchState*>(states_set.find(*current_state).operator->());
                // Stop iterating when find a states that has been already added
                break;
            }
        }

        // Path's states will be saved in order in topological order
        current_state = missing_states.top();
        // Path first element is also the first element of the path to return
        while (!missing_states.empty()) {
            current_state = missing_states.top();
            auto new_state = SearchState(
                current_state->state,
                current_state->object_id,
                previous,
                current_state->direction,
                current_state->label_id
            );
            missing_states.pop();
            // Previous points to path manager set
            previous = const_cast<SearchState*>(
                states_set.insert(new_state).first.operator->());
        }
        // Points to last element of set
        uint64_t path_id = paths[index].size();
        paths[index].push_back(states_set.find(*visited_pointer).operator->());
        return ObjectId(GraphModel::VALUE_PATH_MASK | path_id);
    } else {
        // Save visited pointer directly, visited_pointer always is valid
        paths[index][path_var.id] = visited_pointer;
        return ObjectId(GraphModel::VALUE_PATH_MASK | path_var.id);
    }
}


void PathManager::print(std::ostream& os, uint64_t path_id) const {
    std::thread::id thread_id = std::this_thread::get_id();
    auto index = thread_paths.find(thread_id)->second;
    // Recover state removing mask of path_id
    auto current_state = paths[index][path_id & GraphModel::VALUE_MASK];

    std::vector<std::string> path_string;
    std::vector<bool> directions;
    std::vector<ObjectId> labels;
    while (current_state != nullptr) {
       std::stringstream ss;
       ss << model.get_graph_object(current_state->object_id);
       path_string.push_back(ss.str());
       directions.push_back(current_state->direction);
       labels.push_back(current_state->label_id);
       current_state = const_cast<SearchState*>(current_state->previous);
    }
    os << "(" << path_string[path_string.size() - 1] << ")";

    for (int i = path_string.size() - 2; i >= 0; i--) {
        if (directions[i]) {
            os << "<=[" << model.get_graph_object(labels[i]) << "]=";
        } else {
            os << "=[" << model.get_graph_object(labels[i]) << "]=>";
        }
        os << "(" << path_string[i] << ")";
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
