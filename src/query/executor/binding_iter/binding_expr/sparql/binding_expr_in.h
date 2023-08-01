#pragma once

#include <memory>
#include <ostream>
#include <vector>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/comparisons.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprIn : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs_expr;
    std::vector<std::unique_ptr<BindingExpr>> exprs;

    BindingExprIn(std::unique_ptr<BindingExpr> lhs_expr, std::vector<std::unique_ptr<BindingExpr>> exprs) :
        lhs_expr(std::move(lhs_expr)),
        exprs(std::move(exprs)) { }

    ObjectId eval(const Binding& binding) override {
        bool error = false;

        auto lhs_oid = lhs_expr->eval(binding);

        if (lhs_oid.is_null()) {
            return lhs_oid;
        }

        for (auto& expr : exprs) {
            auto expr_oid = expr->eval(binding);
            if (expr_oid.is_null()) {
                error = true;
            } else if (SPARQL::Comparisons::compare(lhs_oid, expr_oid) == 0) {
                return ObjectId(ObjectId::BOOL_TRUE);
            }
        }
        if (error) {
            return ObjectId::get_null();
        } else {
            return ObjectId(ObjectId::BOOL_FALSE);
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "IN(";

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
