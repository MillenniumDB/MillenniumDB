#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/comparisons.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprLess : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprLess(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }

    ObjectId eval(const Binding& binding) override {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        bool error;
        auto res = SPARQL::Comparisons::strict_compare(lhs_oid, rhs_oid, &error) < 0;
        if (error) {
            return ObjectId::get_null();
        }
        return ObjectId(ObjectId::MASK_BOOL | res);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << '(' << *lhs << '<' << *rhs << ')';
        return os;
    }
};
} // namespace SPARQL
