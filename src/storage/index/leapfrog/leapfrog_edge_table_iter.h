#pragma once

#include "storage/index/leapfrog/leapfrog_iter.h"

class LeapfrogEdgeTableIter : public LeapfrogIter {
public:
    LeapfrogEdgeTableIter(bool*                                   interruption_requested,
                          RandomAccessTable<3>&                   edge_table,
                          std::vector<std::unique_ptr<ScanRange>> initial_ranges,
                          std::vector<VarId>                      intersection_vars,
                          std::vector<VarId>                      enumeration_vars,
                          std::array<uint_fast32_t, 3>            permutation) :
        LeapfrogIter (interruption_requested,
                      move(initial_ranges),
                      move(intersection_vars),
                      move(enumeration_vars)),
        edge_table   (edge_table),
        permutation  (permutation) { }

    inline uint64_t get_key() const override { return current_tuple[level]; }

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

    bool next_enumeration(BindingId& binding) override;

    // returns true if the terms and parent_binding were found
    bool open_terms(BindingId& input_binding) override;

private:
    std::array<uint64_t, 4> current_tuple;
    RandomAccessTable<3>& edge_table;
    std::array<uint_fast32_t, 3> permutation;

    uint64_t saved_edge_enum;
    uint64_t current_edge_enum;
};
