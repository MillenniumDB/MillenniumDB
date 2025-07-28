#pragma once

#include <memory>
#include <vector>

#include "graph_models/object_id.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprCoalesce : public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> exprs;

    BindingExprCoalesce(std::vector<std::unique_ptr<BindingExpr>> exprs) :
        exprs(std::move(exprs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        for (auto& expr : exprs) {
            auto expr_oid = expr->eval(binding);
            if (!expr_oid.is_null())
                return expr_oid;
        }
        return ObjectId::get_null();
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "COALESCE(";
        auto first = true;
        for (auto& expr : exprs) {
            if (first)
                first = false;
            else
                os << ", ";
            expr->print(os, ops);
        }
        os << ')';
    }
};
} // namespace SPARQL
