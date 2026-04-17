#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace SPARQL {
class BindingExprIsBlank : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprIsBlank(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.subtype()) {
        case ObjectSubType::Anon:
            return Conversions::pack_bool(true);
        case ObjectSubType::Null:
            return ObjectId::get_null();
        default:
            return Conversions::pack_bool(false);
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "isBLANK(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
