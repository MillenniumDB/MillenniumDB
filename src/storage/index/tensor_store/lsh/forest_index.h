#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class TensorStore;

namespace LSH {
class ForestIndexQueryIter;
class Tree;
enum class MetricType;
/*
 * ForestIndex is the main class for creating an LSH Forest Index for a given TensorStore. After building the index, one
 * can iterate over all the indexed object_ids through a QueryIterator instance
 */
class ForestIndex {
    friend class ForestIndexQueryIter;

public:
    uint64_t num_trees;
    uint64_t max_bucket_size;
    uint64_t max_depth;

    // Initialize a new forest index
    ForestIndex(const TensorStore& tensor_store,
                MetricType         metric_type,
                uint64_t           num_trees,
                uint64_t           max_bucket_size,
                uint64_t           max_depth);

    // Load an existing forest index from a file
    ForestIndex(const std::string& path, const TensorStore& tensor_store);

    // Build the forest index from the TensorStore
    void build();

    // Get the top k most similar object_ids from the forest using the SynchAscend algorithm
    std::vector<std::pair<uint64_t, float>> query_top_k(const std::vector<float>& query_tensor, uint64_t k, int64_t search_k) const;

    // Get an iterator for all the object_ids in the forest
    std::unique_ptr<ForestIndexQueryIter> query_iter(const std::vector<float>& query_tensor) const;

    // Serialize the tree to a file
    void serialize(const std::string& path) const;

private:
    const TensorStore& tensor_store;
    MetricType         metric_type;

    float (*similarity_fn)(const std::vector<float>&, const std::vector<float>&);

    std::vector<std::unique_ptr<Tree>> trees;

    // Deserialize the tree from a file
    void deserialize(const std::string& path);
};
} // namespace LSH