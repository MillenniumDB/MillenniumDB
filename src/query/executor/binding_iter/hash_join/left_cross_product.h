#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"

class LeftCrossProduct : public BindingIter {
public:
    LeftCrossProduct(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs,
        std::vector<VarId>&&           rhs_vars
    ) :
        lhs               (std::move(lhs)),
        rhs               (std::move(rhs)),
        rhs_vars          (std::move(rhs_vars)) { }

    void analyze(std::ostream& os, int indent = 0) const override;
    void begin(Binding& parent_binding) override;
    bool next() override;
    void reset() override;
    void assign_nulls() override;

private:

    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;
    BindingIter* rhs_iter;

    std::vector<VarId> rhs_vars;

    Binding* parent_binding;

    uint64_t found = 0;
};
