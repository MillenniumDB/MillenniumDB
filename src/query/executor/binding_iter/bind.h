#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/var_id.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

class Bind : public BindingIter {
public:
    Bind(
        std::unique_ptr<BindingIter> child_iter,
        std::unique_ptr<BindingExpr> expr,
        VarId                          var
    ) :
        child_iter    (std::move(child_iter)),
        expr          (std::move(expr)),
        var           (var) { }

    void begin(Binding& parent_binding) override;

    void reset() override;

    bool next() override;

    void assign_nulls() override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> child_iter;
    std::unique_ptr<BindingExpr> expr;
    VarId                          var;

    Binding* parent_binding;
};
