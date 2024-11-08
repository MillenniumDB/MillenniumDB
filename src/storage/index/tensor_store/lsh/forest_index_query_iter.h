#include <cstdint>

#include <vector>

#include "third_party/robin_hood/robin_hood.h"

namespace LSH {
class ForestIndex;
class TreeNode;
/*
 * TODO: Description
 */
class ForestIndexQueryIter {
public:
    // Current value
    std::pair<uint64_t, float>* current;

    ForestIndexQueryIter(const std::vector<float>& query_tensor, const ForestIndex& forest_index);

    // Initialize the iterator
    void begin();

    // Returns true if a new element exists and was set into current, false otherwise
    bool next();

    // Reset the iterator
    void reset();

private:
    const std::vector<float> query_tensor;
    const ForestIndex&       forest_index;

    // Current state of each tree
    uint_fast32_t              current_maximum_depth;
    std::vector<TreeNode*>     nodes;
    std::vector<uint_fast32_t> depths;

    // Buckets of object ids for each tree
    std::vector<robin_hood::unordered_flat_set<uint64_t>> buckets;

    // Stack of object ids and similarities obtained from the intersection
    std::vector<std::pair<uint64_t, float>> stack;

    // Do an intersection between the buckets. If any, remove the intersection from each one and fill the stack
    void intersect_buckets();
};
} // namespace LSH