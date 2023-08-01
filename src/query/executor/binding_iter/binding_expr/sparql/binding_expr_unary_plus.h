#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprUnaryPlus : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprUnaryPlus(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch(expr_oid.get_generic_type()) {
            case ObjectId::MASK_NUMERIC:
                return expr_oid;
            default:
                return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << '+' << *expr;
        return os;
    }
};
} // namespace SPARQL
