#pragma once

#include <memory>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/empty_binding_iter.h"

class IndexNestedLoopJoin : public BindingIter {
public:
    IndexNestedLoopJoin(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs
    ) :
        lhs           (std::move(lhs)),
        original_rhs  (std::move(rhs)) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:
    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> original_rhs;

    BindingIter* rhs; // will point to original_rhs or a EmptyBindingIter

    Binding* parent_binding;

    uint64_t result_count = 0;
    uint64_t executions = 0;

    EmptyBindingIter empty_iter;
};
