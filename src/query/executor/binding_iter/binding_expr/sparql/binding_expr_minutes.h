#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprMinutes : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprMinutes(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);
        if (expr_oid.get_generic_type() != ObjectId::MASK_DT) {
            return ObjectId::get_null();
        }
        bool error;
        auto res = DateTime(expr_oid).get_minute(&error);
        if (error) {
            return ObjectId::get_null();
        }
        return Conversions::pack_int(res);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "MINUTES(" << *expr << ")";
        return os;
    }
};
} // namespace SPARQL
