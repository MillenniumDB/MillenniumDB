#pragma once

#include <vector>

#include "query/executor/binding_iter/scan_ranges/scan_range.h"

class LeapfrogIter {
    friend class LeapfrogJoin;

public:
    virtual ~LeapfrogIter() = default;

    bool* const interruption_requested;

    virtual void up()
    {
        level--;
    }

    virtual void down() = 0;
    virtual bool open_terms(Binding& input_binding) = 0;
    virtual bool next() = 0;
    virtual bool seek(uint64_t key) = 0;
    virtual uint64_t get_key() const = 0;

    inline const std::vector<VarId>& get_intersection_vars()
    {
        return intersection_vars;
    }

    inline const std::vector<VarId>& get_enumeration_vars()
    {
        return enumeration_vars;
    }

    virtual void begin_enumeration() = 0;
    virtual void reset_enumeration() = 0;
    virtual bool next_enumeration(Binding&) = 0;

    const std::vector<std::unique_ptr<ScanRange>> initial_ranges;

    // returns the cardinality of the relation (initial_ranges + intersection + enumeration)
    size_t get_relation_size()
    {
        return initial_ranges.size() + intersection_vars.size() + enumeration_vars.size();
    }

    virtual std::string get_iter_name() const = 0;

    // returns false if estimation cannot be done
    virtual bool try_estimate(
        std::vector<double>& initial_estimations,
        std::vector<double>& after_estimations
    ) const = 0;

protected:
    const std::vector<VarId> intersection_vars;
    const std::vector<VarId> enumeration_vars;

    int_fast32_t level = -1; // can go from -1 to N-1

    LeapfrogIter(
        bool* interruption_requested,
        std::vector<std::unique_ptr<ScanRange>> initial_ranges,
        std::vector<VarId> intersection_vars,
        std::vector<VarId> enumeration_vars
    ) :
        interruption_requested(interruption_requested),
        initial_ranges(std::move(initial_ranges)),
        intersection_vars(std::move(intersection_vars)),
        enumeration_vars(std::move(enumeration_vars))
    { }
};
