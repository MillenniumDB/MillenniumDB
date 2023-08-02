#pragma once

#include <memory>

#include "query/executor/binding_iter.h"

class Slice : public BindingIter {
public:
    Slice(
        std::unique_ptr<BindingIter> child_id_iter,
        uint64_t                       offset,
        uint64_t                       limit
    ) :
        child_id_iter (std::move(child_id_iter)),
        offset        (offset),
        limit         (limit) { }

    void begin(Binding& parent_binding) override;

    void reset() override;

    bool next() override;

    void assign_nulls() override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> child_id_iter;

    Binding* parent_binding;

    uint64_t offset;
    uint64_t limit;
    uint64_t count = 0;
    uint64_t position = 0;

    uint64_t total_count = 0;
};
