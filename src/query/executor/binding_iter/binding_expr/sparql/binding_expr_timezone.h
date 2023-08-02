#pragma once

#include <memory>
#include <sstream>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/datatypes/datetime.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "storage/unified_storage.h"

namespace SPARQL {
class BindingExprTimezone : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprTimezone(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        if (expr_oid.get_generic_type() != ObjectId::MASK_DT) {
            return ObjectId::get_null();
        }

        bool error;
        auto str = DateTime(expr_oid).get_timezone(&error);
        if (error) {
            return ObjectId::get_null();
        }
        auto datatype = UnifiedStorage::get_datatype_id("http://www.w3.org/2001/XMLSchema#dayTimeDuration");
        return Conversions::pack_string_datatype(datatype, std::move(str));
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "TIMEZONE(" << *expr << ")";
        return os;
    }
};
} // namespace SPARQL
