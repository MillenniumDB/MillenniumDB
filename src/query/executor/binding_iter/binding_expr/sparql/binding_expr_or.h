#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprOr : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprOr(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        // Evaluation according to the SPARQL 1.1 standard
        // https://www.w3.org/TR/sparql11-query/#evaluation
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        ObjectId lhs_bool = Conversions::to_boolean(lhs_oid);

        if (lhs_bool == Conversions::pack_bool(true)) {
            return Conversions::pack_bool(true);
        }

        ObjectId rhs_bool = Conversions::to_boolean(rhs_oid);

        if (rhs_bool == Conversions::pack_bool(true)) {
            return Conversions::pack_bool(true);
        } else if (lhs_bool == Conversions::pack_bool(false) && rhs_bool == Conversions::pack_bool(false)) {
            return Conversions::pack_bool(false);
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
        os << '(';
        lhs->print(os, ops);
        os << " OR ";
        rhs->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
