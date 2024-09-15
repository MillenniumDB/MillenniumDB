#pragma once

#include <cassert>
#include <cstdint>
#include <fstream>
#include <vector>

#include "storage/index/tensor_store/serialization.h"

namespace LSH {

enum class TreeNodeType {
    LEAF,
    ANGULAR_SPLIT,
    MINKOWSKI_SPLIT,
};

// TODO: RandomSplitNode

class TreeNode {
public:
    // Neighborhood references
    TreeNode* parent;
    TreeNode* children[2];

    TreeNode(TreeNode* parent) : parent(parent), children { nullptr, nullptr } { }

    TreeNode* sibling() {
        assert(parent != nullptr);
        return (parent->children[0] == this) ? parent->children[1] : parent->children[0];
    }

    virtual ~TreeNode() {
        delete children[0];
        delete children[1];
    }

    // Get the index of the most similar child for a given tensor (false -> left, true -> right)
    virtual bool side(const std::vector<float>&) = 0;

    virtual TreeNodeType type() const = 0;

    virtual void serialize(std::fstream& fs) const = 0;

    virtual void deserialize(std::fstream& fs) = 0;
};


class AngularSplitNode : public TreeNode {
public:
    std::vector<float> plane_normal;

    AngularSplitNode(TreeNode* parent, std::vector<float>&& plane_normal_) :
        TreeNode(parent), plane_normal(std::move(plane_normal_)) { }

    AngularSplitNode(std::fstream& fs) : TreeNode(nullptr) {
        deserialize(fs);
    }

    bool side(const std::vector<float>& tensor) override {
        assert(tensor.size() == plane_normal.size());
        float res = 0.0f;
        #ifdef _OPENMP
        #pragma omp simd reduction(+ : res)
        #endif
        for (uint_fast32_t i = 0; i < tensor.size(); ++i)
            res += plane_normal[i] * tensor[i];
        return res > 0.0f;
    }

    TreeNodeType type() const override {
        return TreeNodeType::ANGULAR_SPLIT;
    }

    void serialize(std::fstream& fs) const override {
        Serialization::write_uint64(fs, plane_normal.size());
        Serialization::write_float_vec(fs, plane_normal);
    }

    void deserialize(std::fstream& fs) override {
        auto plane_normal_size = Serialization::read_uint64(fs);
        plane_normal           = Serialization::read_float_vec(fs, plane_normal_size);
    }
};


class MinkowskiSplitNode : public TreeNode {
public:
    std::vector<float> plane_normal;
    float              plane_offset;

    MinkowskiSplitNode(TreeNode* parent, std::vector<float>&& plane_normal, float plane_offset_) :
        TreeNode(parent), plane_normal(std::move(plane_normal)), plane_offset(plane_offset_) { }

    MinkowskiSplitNode(std::fstream& fs) : TreeNode(nullptr) {
        deserialize(fs);
    }

    bool side(const std::vector<float>& tensor) override {
        assert(tensor.size() == plane_normal.size());
        float res = plane_offset;
        #ifdef _OPENMP
        #pragma omp simd reduction(+ : res)
        #endif
        for (uint_fast32_t i = 0; i < tensor.size(); ++i)
            res += plane_normal[i] * tensor[i];
        return res > 0.0f;
    }

    TreeNodeType type() const override {
        return TreeNodeType::MINKOWSKI_SPLIT;
    }

    void serialize(std::fstream& fs) const override {
        Serialization::write_uint64(fs, plane_normal.size());
        Serialization::write_float_vec(fs, plane_normal);
        Serialization::write_float(fs, plane_offset);
    }

    void deserialize(std::fstream& fs) override {
        const auto plane_normal_size = Serialization::read_uint64(fs);
        plane_normal           = Serialization::read_float_vec(fs, plane_normal_size);
        plane_offset           = Serialization::read_float(fs);
    }
};


class LeafNode : public TreeNode {
public:
    std::vector<uint64_t> object_ids;

    LeafNode(TreeNode* parent) : TreeNode(parent) { }

    LeafNode(TreeNode* parent, std::vector<uint64_t>&& object_ids_) : TreeNode(parent), object_ids(std::move(object_ids_)) { }

    LeafNode(std::fstream& fs) : TreeNode(nullptr) {
        deserialize(fs);
    }

    bool side(const std::vector<float>&) override {
        assert(false && "Leaf nodes should never call the side() method");
        return false;
    }

    TreeNodeType type() const override {
        return TreeNodeType::LEAF;
    }

    void serialize(std::fstream& fs) const override {
        Serialization::write_uint64(fs, object_ids.size());
        Serialization::write_uint64_vec(fs, object_ids);
    }

    void deserialize(std::fstream& fs) override {
        auto object_ids_size = Serialization::read_uint64(fs);
        object_ids           = Serialization::read_uint64_vec(fs, object_ids_size);
    }
};
} // namespace LSH