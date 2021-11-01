#ifndef STORAGE__LEAPFROG_ITER_H_
#define STORAGE__LEAPFROG_ITER_H_

#include <stack>
#include <vector>

#include "base/graph/graph_model.h"
#include "relational_model/execution/binding_id_iter/scan_ranges/scan_range.h"
#include "storage/index/random_access_table/random_access_table.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/bplus_tree/bplus_tree_dir.h"
#include "storage/index/bplus_tree/bplus_tree_leaf.h"
#include "storage/index/tuple_buffer/tuple_buffer.h"

class LeapfrogIter {
public:
    virtual ~LeapfrogIter() = default;

    bool* const interruption_requested;

    virtual void up() { level--; }
    virtual void down() = 0;
    virtual bool open_terms(BindingId& input_binding) = 0;
    virtual bool next() = 0;
    virtual bool seek(uint64_t key) = 0;
    virtual uint64_t get_key() const = 0;

    inline const std::vector<VarId>& get_intersection_vars() { return intersection_vars; }
    inline const std::vector<VarId>& get_enumeration_vars()  { return enumeration_vars; }

    // will consume all tuples and write them into the buffer. Invalidates the current_leaf
    virtual void enum_no_intersection(TupleBuffer& buffer) = 0;

protected:
    const std::vector<std::unique_ptr<ScanRange>> initial_ranges;
    const std::vector<VarId> intersection_vars;
    const std::vector<VarId> enumeration_vars;

    int_fast32_t level = -1; // can go from -1 to N-1

    LeapfrogIter(bool*                                   _interruption_requested,
                 std::vector<std::unique_ptr<ScanRange>> _initial_ranges,
                 std::vector<VarId>                      _intersection_vars,
                 std::vector<VarId>                      _enumeration_vars) :
        interruption_requested (_interruption_requested),
        initial_ranges         (std::move(_initial_ranges)),
        intersection_vars      (std::move(_intersection_vars)),
        enumeration_vars       (std::move(_enumeration_vars)) { }
};


template <std::size_t N>
class LeapfrogBptIter : public LeapfrogIter {
public:
    LeapfrogBptIter(bool*                                   interruption_requested,
                    const BPlusTree<N>&                     btree,
                    std::vector<std::unique_ptr<ScanRange>> initial_ranges,
                    std::vector<VarId>                      intersection_vars,
                    std::vector<VarId>                      enumeration_vars);

    ~LeapfrogBptIter() = default;

    inline uint64_t get_key() const override { return (*current_tuple)[level]; }

    // Increases the level and sets the current_tuple
    void down() override;

    // Sets the current tuple with the next record at the current level
    // not the same concept of a the next in the B+Tree, because may skip many records
    // returns false if there is no next record
    bool next() override;

    // Sets the current tuple with a record that has a greater or equal key at the current level
    // returns false if there is no such record
    bool seek(uint64_t key) override;

    void enum_no_intersection(TupleBuffer& buffer) override;

    // returns true if the terms and parent_binding were found
    bool open_terms(BindingId& input_binding) override;

private:
    std::unique_ptr<Record<N>> current_tuple;

    std::unique_ptr<BPlusTreeLeaf<N>> current_leaf;

    uint32_t current_pos_in_leaf;

    std::stack<std::unique_ptr<BPlusTreeDir<N>>> directory_stack;

    // search a record in the interval [min, max]
    bool internal_search(const Record<N>& min, const Record<N>& max);
};


class LeapfrogEdgeTableIter : public LeapfrogIter {
public:
    LeapfrogEdgeTableIter(bool*                                   interruption_requested,
                          RandomAccessTable<3>&                   _edge_table,
                          std::vector<std::unique_ptr<ScanRange>> _initial_ranges,
                          std::vector<VarId>                      _intersection_vars,
                          std::vector<VarId>                      _enumeration_vars,
                          std::array<uint_fast32_t, 3>            _permutation) :
        LeapfrogIter (interruption_requested,
                      move(_initial_ranges),
                      move(_intersection_vars),
                      move(_enumeration_vars)),
        edge_table   (_edge_table),
        permutation  (_permutation) { }

    ~LeapfrogEdgeTableIter() = default;

    inline uint64_t get_key() const override { return current_tuple[level]; }

    // Increases the level and sets the current_tuple
    void down() override { level++; }

    // Sets the current tuple with the next record at the current level
    // not the same concept of a the next in the B+Tree, because may skip many records
    // returns false if there is no next record
    bool next() override { return false; }

    // Sets the current tuple with a record that has a greater or equal key at the current level
    // returns false if there is no such record
    bool seek(uint64_t key) override {
        return current_tuple[level] >= key;
    }

    void enum_no_intersection(TupleBuffer& buffer) override {
        buffer.reset();
        std::vector<ObjectId> tuple;
        for (size_t i = 0; i < enumeration_vars.size(); i++) {
            tuple.push_back( ObjectId(current_tuple[initial_ranges.size() + intersection_vars.size() + i]) );
        }
        buffer.append_tuple(tuple);
    }

    // returns true if the terms and parent_binding were found
    bool open_terms(BindingId& input_binding) override {
        // initial_ranges[0] is the edge
        auto obj = initial_ranges[0]->get_min(input_binding);

        // check if obj is an edge
        if ((obj & GraphModel::TYPE_MASK) != GraphModel::CONNECTION_MASK) {
            return false;
        }
        auto edge_value = obj & GraphModel::VALUE_MASK;
        auto record = edge_table[edge_value];
        if (record == nullptr) {
            return false;
        } else {
            current_tuple[0] = edge_value;
            current_tuple[1] = (*record)[permutation[0]];
            current_tuple[2] = (*record)[permutation[1]];
            current_tuple[3] = (*record)[permutation[2]];

            // check assigned_vars
            for (std::size_t i = 1; i < initial_ranges.size(); i++) { // we skip 0, because we already searched the edge
                if (current_tuple[i] != initial_ranges[i]->get_min(input_binding)) {
                    return false;
                }
            }
            level = initial_ranges.size() - 1;
            return true;
        }
    }

private:
    std::array<uint64_t, 4> current_tuple;
    RandomAccessTable<3>& edge_table;
    std::array<uint_fast32_t, 3> permutation;
};

#endif // STORAGE__LEAPFROG_ITER_H_
