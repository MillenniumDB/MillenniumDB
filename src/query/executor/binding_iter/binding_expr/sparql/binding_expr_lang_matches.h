#pragma once

#include <algorithm>
#include <memory>
#include <sstream>
#include <string>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprLangMatches : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprLangMatches(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)), rhs(std::move(rhs)) { }

    ObjectId eval(const Binding& binding) override {
        // SPEC: https://www.ietf.org/rfc/rfc4647.txt
        // basic filtering with basic language range

        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        if (lhs_oid.get_sub_type() != ObjectId::MASK_STRING_SIMPLE) {
            return ObjectId::get_null();
        }
        if (rhs_oid.get_sub_type() != ObjectId::MASK_STRING_SIMPLE) {
            return ObjectId::get_null();
        }

        auto lhs_str = Conversions::unpack_string_simple(lhs_oid);
        auto rhs_str = Conversions::unpack_string_simple(rhs_oid);

        auto lhs_size = lhs_str.size();
        auto rhs_size = rhs_str.size();

        if (rhs_str == "*") {
            return ObjectId(ObjectId::MASK_BOOL | (lhs_size > 0));
        }

        std::transform(lhs_str.begin(), lhs_str.end(), lhs_str.begin(), [](unsigned char c){ return std::tolower(c); });
        std::transform(rhs_str.begin(), rhs_str.end(), rhs_str.begin(), [](unsigned char c){ return std::tolower(c); });

        if (lhs_size < rhs_size) {
            return ObjectId(ObjectId::BOOL_FALSE);
        } else if (lhs_size == rhs_size) {
            return ObjectId(ObjectId::MASK_BOOL | (lhs_str == rhs_str));
        }

        auto lhs_prefix = lhs_str.substr(0, rhs_str.size());

        if (lhs_str[lhs_prefix.size()] == '-') {
            return ObjectId(ObjectId::MASK_BOOL | (lhs_prefix == rhs_str));
        } else {
            return ObjectId(ObjectId::BOOL_FALSE);
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "langMATCHES(" << *lhs << ", " << *rhs << ")";
        return os;
    }
};
} // namespace SPARQL
