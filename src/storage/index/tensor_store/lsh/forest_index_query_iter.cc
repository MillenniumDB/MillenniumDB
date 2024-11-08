#include "forest_index_query_iter.h"

#include "macros/likely.h"
#include "storage/index/tensor_store/lsh/forest_index.h"
#include "storage/index/tensor_store/lsh/tree.h"
#include "storage/index/tensor_store/lsh/tree_node.h"
#include "storage/index/tensor_store/tensor_store.h"
#include "query/exceptions.h"
#include "query/query_context.h"

using namespace LSH;

ForestIndexQueryIter::ForestIndexQueryIter(
    const std::vector<float>& query_tensor_,
    const ForestIndex& forest_index_
) :
    query_tensor(query_tensor_),
    forest_index(forest_index_)
{
    // Initialize containers with the right size
    const auto num_trees = forest_index.trees.size();
    nodes.resize(num_trees);
    depths.resize(num_trees);
    buckets.resize(num_trees);
}


void ForestIndexQueryIter::begin() {
    for (uint_fast32_t i = 0; i < forest_index.trees.size(); ++i) {
        auto&& [leaf, depth] = forest_index.trees[i]->descend(query_tensor);
        nodes[i]  = leaf;
        depths[i] = depth;
        buckets[i].insert(leaf->object_ids.begin(), leaf->object_ids.end());
    }
    current_maximum_depth = *std::max_element(depths.begin(), depths.end());
    current               = nullptr;
    intersect_buckets();
}


bool ForestIndexQueryIter::next() {
    while (!stack.empty() || current_maximum_depth > 0) {
        if (MDB_unlikely(get_query_ctx().thread_info.interruption_requested)) {
            throw InterruptedException();
        }

        // Return elements from the stack
        if (!stack.empty()) {
            if (current == nullptr) {
                current = &stack.back();
                return true;
            } else if (current != &stack.front()) {
                --current;
                return true;
            } else {
                // Reached the end of the stack
                current = nullptr;
                stack.clear();

                // No more elements to return
                if (current_maximum_depth == 0)
                    return false;
            }
        }

        // Try to get an intersection
        for (uint_fast32_t i = 0; i < nodes.size(); ++i) {
            if (depths[i] == current_maximum_depth) {
                const auto sibling                   = nodes[i]->sibling();
                const auto sibling_descendant_leaves = Tree::descendants(sibling);
                for (const auto& sibling_descendant_leaf : sibling_descendant_leaves) {
                    buckets[i].insert(sibling_descendant_leaf->object_ids.begin(),
                                      sibling_descendant_leaf->object_ids.end());
                }
                nodes[i] = nodes[i]->parent;
                --depths[i];
            }
        }
        --current_maximum_depth;
        intersect_buckets();
    }
    return false;
}


void ForestIndexQueryIter::reset() {
    stack.clear();
    for (auto& bucket : buckets)
        bucket.clear();
    begin();
}


void ForestIndexQueryIter::intersect_buckets() {
    if (std::any_of(buckets.begin(), buckets.end(), [](const auto& bucket) { return bucket.empty(); })) {
        // Fast path for empty intersection due to empty buckets
        return;
    }

    // Start with the first bucket
    robin_hood::unordered_flat_set<uint64_t> intersection_bucket = buckets[0];
    for (uint_fast32_t i = 1; i < buckets.size(); ++i) {
        // For each other bucket, remove the elements that are not present
        for (auto it = intersection_bucket.begin(); it != intersection_bucket.end();) {
            if (!buckets[i].contains(*it))
                it = intersection_bucket.erase(it);
            else
                ++it;
        }

        // Fast path for empty intersection
        if (intersection_bucket.empty()) {
            return;
        }
    }

    // Remove the intersection from all the current buckets to avoid duplicates
    for (auto& bucket : buckets) {
        for (const auto& object_id : intersection_bucket) {
            bucket.erase(object_id);
        }
    }

    // Fill the stack
    std::vector<float> tensor_buffer(forest_index.tensor_store.tensors_dim);
    for (const auto& object_id : intersection_bucket) {
        forest_index.tensor_store.get(object_id, tensor_buffer);
        const float similarity = forest_index.similarity_fn(query_tensor, tensor_buffer);
        stack.emplace_back(object_id, similarity);
    }

    // Sort by similarity (in descending order)
    std::sort(stack.begin(), stack.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
}
