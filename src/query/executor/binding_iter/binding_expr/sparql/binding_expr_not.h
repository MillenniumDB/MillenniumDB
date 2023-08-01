#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprNot : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprNot(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        ObjectId expr_bool = Conversions::to_boolean(expr_oid);

        if (expr_bool.is_true()) {
            return ObjectId(ObjectId::BOOL_FALSE);
        } else if (expr_bool.is_false()) {
            return ObjectId(ObjectId::BOOL_TRUE);
        } else {
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << '!' << *expr;
        return os;
    }
};
} // namespace SPARQL
