#ifndef RELATIONAL_MODEL__PATH_MANAGER_H_
#define RELATIONAL_MODEL__PATH_MANAGER_H_

#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "base/ids/object_id.h"
#include "relational_model/execution/binding_id_iter/property_paths/search_state.h"
#include "relational_model/models/quad_model/quad_model.h"


/*
PathManager manages the conversion from path to ObjectId and
ObjectId to path. Each query will run in a thread, so PathManager
assigns a slot in 'paths' (to save a pointers to SearchState). The same
index will be used in 'materialized_state' to see if the path will be
materialized and 'states_mataterialized' to save a SearchState copy to
materialization
*/

class PathManager : public PathPrinter {
public:
    ~PathManager() = default;

    static void init(GraphModel& model, uint_fast32_t max_threads);

    // Assign space to save pointers to recover path
    void begin(size_t binding_size, bool materialize);

    ObjectId set_path(const SearchState* visited_pointer, VarId path_var);

    void print(std::ostream& os, uint64_t path_id) const override;

    void clear();

private:
    GraphModel& model;
    PathManager(GraphModel& model, uint_fast32_t max_threads);

    // Stores the available index to store a paths of a thread
    std::queue<uint_fast32_t> available_index;

    // Saves the index of paths of thread_id
    std::unordered_map<std::thread::id, uint_fast32_t> thread_paths;

    // Saves the pointer to path object
    std::vector<std::vector<const SearchState* >> paths;

    // Indicates with paths must be materialized
    std::vector<bool> paths_materialized;

    // Saves pointer when path must be materialized
    std::vector<std::vector<std::unordered_set<SearchState, SearchStateHasher>>> states_materialized;

    // To avoid synchronization problems
    std::mutex lock_mutex;

};

extern PathManager& path_manager; // global object

#endif // RELATIONAL_MODEL__PATH_MANAGER_H_
