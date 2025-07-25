#pragma once

#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/query_context.h"

class BindingExprVar : public BindingExpr {
public:
    VarId var;

    BindingExprVar(VarId var) :
        var(var)
    { }

    ObjectId eval(const Binding& binding) override
    {
        return binding[var];
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>) const override
    {
        os << var;
    }
};
