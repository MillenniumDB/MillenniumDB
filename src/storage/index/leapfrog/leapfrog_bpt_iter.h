#pragma once

#include "storage/index/leapfrog/leapfrog_iter.h"

template <std::size_t N>
class LeapfrogBptIter : public LeapfrogIter {
public:
    LeapfrogBptIter(bool*                                   interruption_requested,
                    const BPlusTree<N>&                     btree,
                    std::vector<std::unique_ptr<ScanRange>> initial_ranges,
                    std::vector<VarId>                      intersection_vars,
                    std::vector<VarId>                      enumeration_vars);

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

    void begin_enumeration() override;
    void reset_enumeration() override;
    bool next_enumeration(BindingId&) override;

    // returns true if the terms and parent_binding were found
    bool open_terms(BindingId& input_binding) override;

private:
    std::unique_ptr<Record<N>> current_tuple;

    std::unique_ptr<BPlusTreeLeaf<N>> current_leaf;

    std::unique_ptr<BptIter<N>> enum_bpt_iter;

    uint32_t current_pos_in_leaf;

    std::stack<std::unique_ptr<BPlusTreeDir<N>>> directory_stack;

    // search a record in the interval [min, max]
    bool internal_search(const Record<N>& min, const Record<N>& max);
};
