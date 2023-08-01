#pragma once

#include "query/query_context.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

class BindingExprVar : public BindingExpr {
public:
    VarId var;

    BindingExprVar(VarId var) :
        var (var){ }

    ObjectId eval(const Binding& binding) override {
        return binding[var];
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << '?' << get_query_ctx().get_var_name(var);
        return os;
    }
};
