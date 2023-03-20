#pragma once

#include <array>
#include <cstdlib>

#include "third_party/robin_hood/robin_hood.h"

namespace Import {
template<size_t N>
class StatsProcessor {
public:
    ~StatsProcessor() = default;
    virtual void process_tuple(const std::array<uint64_t, N>& tuple) = 0;
};

template<size_t N>
class NoStat : public StatsProcessor<N> {
public:
    void process_tuple(const std::array<uint64_t, N>&) override { }
};

template<size_t N>
class DistinctStat : public StatsProcessor<N> {
    // computes how many different elements are in the first column, assuming it is ordered
public:
    size_t distinct = 0;
    size_t current  = 0;

    void process_tuple(const std::array<uint64_t, N>& tuple) override {
        if (tuple[0] != current) {
            ++distinct;
            current = tuple[0];
        }
    }
};

template<size_t N>
class DictCountStat : public StatsProcessor<N> {
    // computes how many elements have each one of the first column values, assuming it is ordered
public:
    size_t count   = 0;
    size_t current = 0;

    robin_hood::unordered_map<uint64_t, uint64_t> dict;

    void process_tuple(const std::array<uint64_t, N>& tuple) override {
        if (tuple[0] == current) {
            ++count;
            current = tuple[0];
        } else {
            if (current != 0) {
                dict.insert({ current, count });
            }
            current = tuple[0];
            count   = 1;
        }
    }

    void end() {
        if (current != 0) {
            dict.insert({ current, count });
        }
    }
};

class PropStat : public StatsProcessor<3> {
public:
    uint64_t current_value   = 0;
    uint64_t key_count       = 0;
    uint64_t distinct_values = 0;
    uint64_t current_key     = 0;

    robin_hood::unordered_map<uint64_t, uint64_t> map_key_count;
    robin_hood::unordered_map<uint64_t, uint64_t> map_distinct_values;

    void process_tuple(const std::array<uint64_t, 3>& tuple) override {
        // check same key
        if (tuple[0] == current_key) {
            ++key_count;
            // check if value changed
            if (tuple[1] != current_value) {
                ++distinct_values;
                current_value = tuple[1];
            }
        } else {
            // save stats from last key
            if (current_key != 0) {
                map_key_count.insert({ current_key, key_count });
                map_distinct_values.insert({ current_key, distinct_values });
            }
            current_key   = tuple[0];
            current_value = tuple[1];

            key_count       = 1;
            distinct_values = 1;
        }
    }

    void end() {
        if (current_key != 0) {
            map_key_count.insert({ current_key, key_count });
            map_distinct_values.insert({ current_key, distinct_values });
        }
    }
};

class LabelStat : public StatsProcessor<2> {
public:
    uint64_t                                      current_label = 0;
    uint64_t                                      label_count   = 0;
    robin_hood::unordered_map<uint64_t, uint64_t> map_label_count;

    void process_tuple(const std::array<uint64_t, 2>& tuple) override {
        // check same key
        if (tuple[0] == current_label) {
            ++label_count;
        } else {
            // save stats from last key
            if (current_label != 0) {
                map_label_count.insert({ current_label, label_count });
            }
            current_label = tuple[0];
            label_count   = 1;
        }
    }

    void end() {
        if (current_label != 0) {
            map_label_count.insert({ current_label, label_count });
        }
    }
};

class PredicateStat : public StatsProcessor<3> {
public:
    uint64_t                                      current_predicate = 0;
    uint64_t                                      predicate_count   = 0;
    uint64_t                                      distinct_values   = 0;
    robin_hood::unordered_map<uint64_t, uint64_t> map_predicate_count;

    void process_tuple(const std::array<uint64_t, 3>& tuple) override {
        if (tuple[0] == current_predicate) {
            ++predicate_count;
        } else {
            // save stats from last predicate
            if (current_predicate != 0) {
                map_predicate_count.insert({ current_predicate, predicate_count });
            }
            current_predicate = tuple[0];
            predicate_count   = 1;
            ++distinct_values;
        }
    }

    void end() {
        if (current_predicate != 0) {
            map_predicate_count.insert({ current_predicate, predicate_count });
        }
    }
};

} // namespace Import
