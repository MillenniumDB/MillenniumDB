#pragma once

#include <memory>
#include <ostream>

#include "graph_models/object_id.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprIsLiteral : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprIsLiteral(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch(expr_oid.get_generic_type()) {
            case ObjectId::MASK_STRING:
            case ObjectId::MASK_NUMERIC:
            case ObjectId::MASK_BOOL:
            case ObjectId::MASK_DT:
                return ObjectId(ObjectId::BOOL_TRUE);
            case ObjectId::MASK_NULL:
                return ObjectId::get_null();
            default:
                return ObjectId(ObjectId::BOOL_FALSE);
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "isLITERAL(" << *expr << ")";
        return os;
    }
};
} // namespace SPARQL
