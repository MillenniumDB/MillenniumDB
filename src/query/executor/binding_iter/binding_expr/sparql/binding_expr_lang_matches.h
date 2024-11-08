#pragma once

#include <algorithm>
#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprLangMatches : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprLangMatches(std::unique_ptr<BindingExpr> lhs,
                           std::unique_ptr<BindingExpr> rhs) :
        lhs (std::move(lhs)),
        rhs (std::move(rhs)) { }

    ObjectId eval(const Binding& binding) override {
        // SPEC: https://www.ietf.org/rfc/rfc4647.txt
        // basic filtering with basic language range

        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        if (RDF_OID::get_generic_sub_type(lhs_oid) != RDF_OID::GenericSubType::STRING_SIMPLE) {
            return ObjectId::get_null();
        }
        if (RDF_OID::get_generic_sub_type(rhs_oid) != RDF_OID::GenericSubType::STRING_SIMPLE) {
            return ObjectId::get_null();
        }

        auto lhs_str = Conversions::unpack_string(lhs_oid);
        auto rhs_str = Conversions::unpack_string(rhs_oid);

        auto lhs_size = lhs_str.size();
        auto rhs_size = rhs_str.size();

        if (rhs_str == "*") {
            return Conversions::pack_bool(lhs_size > 0);
        }

        std::transform(lhs_str.begin(), lhs_str.end(), lhs_str.begin(), ::tolower);
        std::transform(rhs_str.begin(), rhs_str.end(), rhs_str.begin(), ::tolower);

        if (lhs_size < rhs_size) {
            return Conversions::pack_bool(false);
        } else if (lhs_size == rhs_size) {
            return Conversions::pack_bool(lhs_str == rhs_str);
        }

        auto lhs_prefix = lhs_str.substr(0, rhs_str.size());

        if (lhs_str[lhs_prefix.size()] == '-') {
            return Conversions::pack_bool(lhs_prefix == rhs_str);
        } else {
            return Conversions::pack_bool(false);
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
