#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"

class Union : public BindingIter {
public:
    Union(
        std::vector<std::unique_ptr<BindingIter>>&& iters
    ) :
        iters         (std::move(iters)) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    std::vector<std::unique_ptr<BindingIter>> iters;
    uint_fast32_t current_iter = 0;
    Binding* parent_binding;
};
