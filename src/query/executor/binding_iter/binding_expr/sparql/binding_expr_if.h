#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprIf : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr_cond;
    std::unique_ptr<BindingExpr> expr_then;
    std::unique_ptr<BindingExpr> expr_else;

    BindingExprIf(
        std::unique_ptr<BindingExpr> expr_cond,
        std::unique_ptr<BindingExpr> expr_then,
        std::unique_ptr<BindingExpr> expr_else
    ) :
        expr_cond(std::move(expr_cond)),
        expr_then(std::move(expr_then)),
        expr_else(std::move(expr_else))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto cond_oid = expr_cond->eval(binding);

        ObjectId cond_bool = Conversions::to_boolean(cond_oid);

        if (cond_bool == Conversions::pack_bool(true)) {
            return expr_then->eval(binding);
        } else if (cond_bool == Conversions::pack_bool(false)) {
            return expr_else->eval(binding);
        } else {
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "IF(";
        expr_cond->print(os, ops);
        os << ", ";
        expr_then->print(os, ops);
        os << ", ";
        expr_else->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
