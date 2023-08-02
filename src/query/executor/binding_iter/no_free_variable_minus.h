#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"

class NoFreeVariableMinus : public BindingIter {
public:
    NoFreeVariableMinus(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs
    ) :
        lhs           (std::move(lhs)),
        rhs           (std::move(rhs)) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;
    bool has_result = false;
};
