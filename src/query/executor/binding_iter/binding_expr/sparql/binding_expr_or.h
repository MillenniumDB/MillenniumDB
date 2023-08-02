#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprOr : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprOr(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }

    ObjectId eval(const Binding& binding) override {
        // Evaluation according to the SPARQL 1.1 standard
        // https://www.w3.org/TR/sparql11-query/#evaluation
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        ObjectId lhs_bool = Conversions::to_boolean(lhs_oid);

        if (lhs_bool.is_true()) {
            return ObjectId(ObjectId::BOOL_TRUE);
        }

        ObjectId rhs_bool = Conversions::to_boolean(rhs_oid);

        if (rhs_bool.is_true()) {
            return ObjectId(ObjectId::BOOL_TRUE);
        } else if (lhs_bool.is_false() && rhs_bool.is_false()) {
            return ObjectId(ObjectId::BOOL_FALSE);
        } else {
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << '(' << *lhs << "||" << *rhs << ')';
        return os;
    }
};
} // namespace SPARQL
