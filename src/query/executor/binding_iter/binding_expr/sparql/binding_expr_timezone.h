#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace SPARQL {
class BindingExprTimezone : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprTimezone(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.generic_type()) {
        case ObjectGenType::TemporalLiteral: {
            bool error;
            auto str = DateTime(expr_oid).get_timezone(&error);
            if (error) {
                return ObjectId::get_null();
            }
            std::string datatype = "http://www.w3.org/2001/XMLSchema#dayTimeDuration";
            return Conversions::pack_string_datatype(datatype, str);
        }
        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "TIMEZONE(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace SPARQL
