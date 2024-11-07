#include "tree.h"

#include <cassert>
#include <fstream>

#include "storage/index/tensor_store/lsh/metric.h"
#include "storage/index/tensor_store/serialization.h"
#include "storage/index/tensor_store/tensor_store.h"

using namespace LSH;

Tree::Tree(const TensorStore& tensor_store_, MetricType metric_type_, uint64_t max_bucket_size_, uint64_t max_depth_) :
    tensor_store(tensor_store_),
    metric_type(metric_type_),
    tensors_dim(tensor_store_.tensors_dim),
    max_bucket_size(max_bucket_size_),
    max_depth(max_depth_),
    root(nullptr) {
    // Bind metric functions
    switch (metric_type) {
    case MetricType::ANGULAR: {
        similarity_fn = &Metric::cosine_distance;
        break;
    }
    case MetricType::EUCLIDEAN: {
        similarity_fn = &Metric::euclidean_distance;
        break;
    }
    default: { // MetricType::MANHATTAN
        similarity_fn = &Metric::manhattan_distance;
        break;
    }
    }
    tensor_buffer.resize(tensors_dim);
}


Tree::Tree(std::fstream& fs, const TensorStore& tensor_store_) :
    tensor_store(tensor_store_) {
    deserialize(fs);
    tensor_buffer.resize(tensors_dim);
}


Tree::~Tree() {
    delete root;
}


std::vector<LeafNode*> Tree::descendants(TreeNode* node) {
    std::vector<LeafNode*> descendants;
    std::vector<TreeNode*> stack = { node };
    while (!stack.empty()) {
        TreeNode* current_node = stack.back();
        stack.pop_back();
        if (current_node->type() == TreeNodeType::LEAF) {
            auto leaf_node = reinterpret_cast<LeafNode*>(current_node);
            descendants.push_back(leaf_node);
        } else {
            stack.push_back(current_node->children[0]);
            stack.push_back(current_node->children[1]);
        }
    }
    return descendants;
}


void Tree::build() {
    if (root != nullptr)
        throw std::runtime_error("Unexpected Tree::build() call: Tree is already built!");

    // Fill the initial node with all the objects in the tensor store
    std::vector<uint64_t> object_ids;
    for (auto&& [object_id, _] : tensor_store.object_id2tensor_offset)
        object_ids.emplace_back(object_id);
    auto tmp = new LeafNode(nullptr, std::move(object_ids));

    // Bind the root
    root = tmp;

    // Create splits while possible
    std::vector<std::pair<LeafNode*, uint_fast32_t>> node_depth_stack = { { tmp, 0 } };
    while (node_depth_stack.size() > 0) {
        auto [current_leaf, current_depth] = node_depth_stack.back();
        node_depth_stack.pop_back();

        if (current_depth < max_depth && current_leaf->object_ids.size() > max_bucket_size) {
            std::vector<uint64_t> left_object_ids;
            std::vector<uint64_t> right_object_ids;
            TreeNode*             new_parent = nullptr;

            // Try to split the node with the sufficient imbalance threshold
            for (auto i = 0u; i < SPLIT_ATTEMPS; ++i) {
                new_parent = split_nodes(current_leaf->object_ids, left_object_ids, right_object_ids);

                double imbalance = double(left_object_ids.size()) / double(left_object_ids.size() + right_object_ids.size());
                imbalance = fmax(imbalance, 1.0l - imbalance);

                if (imbalance < IMBALANCE_THRESHOLD) {
                    break;
                }

                delete new_parent;
                new_parent = nullptr;
            }


            if (new_parent == nullptr) {
                // The split failed, just use a random split
                left_object_ids.clear();
                right_object_ids.clear();

                new_parent = new RandomSplitNode(nullptr);

                for (const auto& object_id : current_leaf->object_ids) {
                    // No need to call new_parent->side(), method can be inlined
                    const bool side = (rand() % 2) == 0;
                    if (side == false) {
                        left_object_ids.emplace_back(object_id);
                    } else {
                        right_object_ids.emplace_back(object_id);
                    }
                }
            }

            // Update previous parent
            if (current_leaf->parent == nullptr) {
                // There were just one leaf node
                root = new_parent;
            } else {
                // Replace the leaf node with the new parent
                new_parent->parent = current_leaf->parent;
                // Get which child was current node
                const bool side = current_leaf->parent->children[1] == current_leaf;
                // Update its parent
                current_leaf->parent->children[side] = new_parent;
            }

            // Connect the new parent with its children. Reuse the leaf node to optimize memory allocation
            current_leaf->parent     = new_parent;
            current_leaf->object_ids = std::move(left_object_ids);

            new_parent->children[0] = current_leaf;

            LeafNode* right_child = new LeafNode(new_parent, std::move(right_object_ids));
            new_parent->children[1] = right_child;

            node_depth_stack.emplace_back(right_child, current_depth + 1);
            node_depth_stack.emplace_back(current_leaf, current_depth + 1);
        }
    }
}


std::pair<LeafNode*, uint64_t> Tree::descend(const std::vector<float>& query_tensor) const {
    assert(query_tensor.size() == tensors_dim);
    TreeNode* current_node  = root;
    uint64_t  current_depth = 0;
    // Descend until reaching a leaf node
    while (current_node->type() != TreeNodeType::LEAF) {
        current_node = current_node->children[current_node->side(query_tensor)];
        ++current_depth;
    }
    return std::make_pair(reinterpret_cast<LeafNode*>(current_node), current_depth);
}


void Tree::serialize(std::fstream& fs) const {
    // Serialize sizes
    Serialization::write_uint64(fs, tensors_dim);
    Serialization::write_uint64(fs, max_bucket_size);
    Serialization::write_uint64(fs, max_depth);

    // Serialize metric
    Serialization::write_uint8(fs, static_cast<uint8_t>(metric_type));

    // Serialize the tree with pre-order traversal
    std::vector<TreeNode*> stack = { root };
    while (!stack.empty()) {
        TreeNode* current_node = stack.back();
        stack.pop_back();
        Serialization::write_uint8(fs, static_cast<uint8_t>(current_node->type()));
        current_node->serialize(fs);
        if (current_node->type() != TreeNodeType::LEAF) {
            stack.push_back(current_node->children[1]);
            stack.push_back(current_node->children[0]);
        }
    }
}

void Tree::deserialize(std::fstream& fs) {
    // Deserialize sizes
    tensors_dim     = Serialization::read_uint64(fs);
    max_bucket_size = Serialization::read_uint64(fs);
    max_depth       = Serialization::read_uint64(fs);

    // Deserialize metric and bind similarity and deserialize split node functions
    metric_type = static_cast<MetricType>(Serialization::read_uint8(fs));
    TreeNode* (*deserialize_split_node)(std::fstream&);
    switch (metric_type) {
    case MetricType::ANGULAR: {
        similarity_fn = &Metric::cosine_distance;
        deserialize_split_node = &deserialize_angular_split_node;
        break;
    }
    case MetricType::EUCLIDEAN: {
        similarity_fn = &Metric::euclidean_distance;
        deserialize_split_node = &deserialize_minkowski_split_node;
        break;
    }
    default: { // MetricType::MANHATTAN
        similarity_fn = &Metric::manhattan_distance;
        deserialize_split_node = &deserialize_minkowski_split_node;
        break;
    }
    }

    // Deserialize the pre-order traversal serialized tree
    const auto root_type = static_cast<TreeNodeType>(Serialization::read_uint8(fs));
    switch (root_type) {
        case TreeNodeType::LEAF: {
            // Base case when there is only one node
            root = new LeafNode(fs);
            return;
        }
        case TreeNodeType::RANDOM_SPLIT: {
            root = new RandomSplitNode(fs);
            break;
        }
        default: {
            root = deserialize_split_node(fs);
            break;
        }
    }

    std::vector<TreeNode*> stack = { root };
    while (!stack.empty()) {
        TreeNode* current_node;
        const auto current_type = static_cast<TreeNodeType>(Serialization::read_uint8(fs));
        switch (current_type) {
        case TreeNodeType::LEAF: {
            current_node = new LeafNode(fs);
            break;
        }
        case TreeNodeType::RANDOM_SPLIT: {
            current_node = new RandomSplitNode(fs);
            break;
        }
        default: {
            current_node = deserialize_split_node(fs);
            break;
        }
        };
        current_node->parent = stack.back();
        if (stack.back()->children[0] == nullptr) {
            stack.back()->children[0] = current_node;
        } else {
            stack.back()->children[1] = current_node;
            stack.pop_back();
        }
        if (current_node->type() != TreeNodeType::LEAF) {
            stack.push_back(current_node);
        }
    }
}

TreeNode* Tree::split_nodes(const std::vector<uint64_t>& source,
                            std::vector<uint64_t>&       left,
                            std::vector<uint64_t>&       right) {
    TreeNode* split_node;
    left.clear();
    right.clear();

    switch (metric_type) {
    case MetricType::ANGULAR: {
        auto&& [plane_normal, _] = generate_plane(source);
        split_node = new AngularSplitNode(nullptr, std::move(plane_normal));
        break;
    }
    default: { // MetricType::EUCLIDEAN or MetricType::MANHATTAN
        auto&& [plane_normal, plane_offset] = generate_plane(source);
        split_node = new MinkowskiSplitNode(nullptr, std::move(plane_normal), plane_offset);
        break;
    }
    }

    for (const auto& object_id : source) {
        tensor_store.get(object_id, tensor_buffer);
        if (split_node->side(tensor_buffer))
            right.emplace_back(object_id);
        else
            left.emplace_back(object_id);
    }

    return split_node;
}


std::pair<std::vector<float>, float> Tree::generate_plane(const std::vector<uint64_t>& object_ids) {
    assert(object_ids.size() > 1);
    uint64_t centroid_index_a = get_uniform_uint64(0, object_ids.size() - 1);
    uint64_t centroid_index_b = get_uniform_uint64(0, object_ids.size() - 2);
    if (centroid_index_b == centroid_index_a) ++centroid_index_b; // prevents centroid_a == centroid_b

    std::vector<float> centroid_a(tensor_store.tensors_dim);
    std::vector<float> centroid_b(tensor_store.tensors_dim);
    tensor_store.get(object_ids[centroid_index_a], centroid_a);
    tensor_store.get(object_ids[centroid_index_b], centroid_b);

    uint_fast32_t num_points_centroid_a = 1;
    uint_fast32_t num_points_centroid_b = 1;

    for (uint_fast32_t epochs = 0; epochs < GENERATE_PLANE_EPOCHS; ++epochs) {
        const auto k = get_uniform_uint64(0, object_ids.size() - 1);
        tensor_store.get(object_ids[k], tensor_buffer);

        const float similarity_ak = similarity_fn(centroid_a, tensor_buffer);
        const float similarity_bk = similarity_fn(centroid_b, tensor_buffer);
        if (similarity_ak < similarity_bk) {
            // Centroid A is more similar, update it
            #ifdef _OPENMP
            #pragma omp simd
            #endif
            for (uint_fast32_t i = 0; i < tensor_store.tensors_dim; ++i)
                centroid_a[i] = (num_points_centroid_a * centroid_a[i] + tensor_buffer[i]) / (num_points_centroid_a + 1);
            ++num_points_centroid_a;
        } else {
            // Centroid B is more similar, update it
            #ifdef _OPENMP
            #pragma omp simd
            #endif
            for (uint_fast32_t i = 0; i < tensor_store.tensors_dim; ++i)
                centroid_b[i] = (num_points_centroid_b * centroid_b[i] + tensor_buffer[i]) / (num_points_centroid_b + 1);
            ++num_points_centroid_b;
        }
    }

    // Compute the normal of the plane between centroid_a and centroid_b
    std::vector<float> plane_normal(tensors_dim);
    #ifdef _OPENMP
    #pragma omp simd
    #endif
    for (uint_fast32_t i = 0; i < tensors_dim; ++i)
        plane_normal[i] = centroid_a[i] - centroid_b[i];

    // Compute the offset of the plane
    float plane_offset = 0.0f;
    #ifdef _OPENMP
    #pragma omp simd reduction(+ : plane_offset)
    #endif
    for (uint_fast32_t i = 0; i < tensors_dim; ++i)
        plane_offset += -plane_normal[i] * (centroid_a[i] + centroid_b[i]) / 2.0f;

    return std::make_pair(plane_normal, plane_offset);
}
