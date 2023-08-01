#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprCoalesce : public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> exprs;

    BindingExprCoalesce(std::vector<std::unique_ptr<BindingExpr>> exprs) :
        exprs(std::move(exprs)) { }

    ObjectId eval(const Binding& binding) override {
        for (auto& expr : exprs) {
            auto expr_oid = expr->eval(binding);
            if (!expr_oid.is_null())
                return expr_oid;
        }
        return ObjectId::get_null();
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "COALESCE(";

        for (size_t i = 0; i < exprs.size(); i++) {
            if (i != 0) {
                os << ", ";
            }
            os << *exprs[i];
        }
        os << ")";
        return os;
    }
};
} // namespace SPARQL
