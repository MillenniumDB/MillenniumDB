#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprIRI : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;
    std::string                  base_iri;

    BindingExprIRI(std::unique_ptr<BindingExpr> expr, std::string base_iri) :
        expr(std::move(expr)), base_iri(base_iri) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        // IRIs remain unchanged
        if (expr_oid.is_iri()) {
            return expr_oid;
        }
        // Strings are converted to IRIs
        else if (expr_oid.get_sub_type() == ObjectId::MASK_STRING_SIMPLE) {
            std::string str = Conversions::unpack_string_simple(expr_oid);
            if (str.find(':') == std::string::npos) {
                // IRI is not absolute
                if (base_iri.empty())
                    return ObjectId::get_null();
                else
                    return Conversions::pack_iri(base_iri + str);
            } else {
                // IRI is absolute
                return Conversions::pack_iri(str);
            }
        }
        // Other types return null
        else
        {
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "IRI(" << *expr << ", " << base_iri << ")";
        return os;
    }
};
} // namespace SPARQL
