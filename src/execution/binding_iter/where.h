#pragma once

#include <memory>

#include "base/binding/binding_iter.h"
#include "base/binding/binding_id_iter.h"
#include "execution/binding_iter/binding_expr/binding_expr.h"

class Where : public BindingIter {
public:
    Where(std::unique_ptr<BindingIter> iter,
          std::unique_ptr<BindingExpr> binding_expr);

    void begin(std::ostream&) override;

    bool next() override;

    GraphObject operator[](VarId var_id) const override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> child_iter;
    std::unique_ptr<BindingExpr> binding_expr;
    uint64_t checked = 0;
    uint64_t results = 0;
};
