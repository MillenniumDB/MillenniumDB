#pragma once

#include <memory>
#include <string>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprContains : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprContains(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)), rhs(std::move(rhs)) { }

    ObjectId eval(const Binding& binding) override {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto lhs_sub = lhs_oid.get_sub_type();
        auto rhs_sub = rhs_oid.get_sub_type();

        if (lhs_sub != ObjectId::MASK_STRING_SIMPLE
         && lhs_sub != ObjectId::MASK_STRING_XSD
         && lhs_sub != ObjectId::MASK_STRING_LANG) {
            return ObjectId::get_null();
        }

        if (rhs_sub != ObjectId::MASK_STRING_SIMPLE
         && rhs_sub != ObjectId::MASK_STRING_XSD
         && rhs_sub != ObjectId::MASK_STRING_LANG) {
            return ObjectId::get_null();
        }

        if (lhs_sub != ObjectId::MASK_STRING_LANG
         && rhs_sub == ObjectId::MASK_STRING_LANG) {
            return ObjectId::get_null();
        }

        std::string lhs_str;
        std::string rhs_str;

        if (lhs_sub == ObjectId::MASK_STRING_LANG && rhs_sub == ObjectId::MASK_STRING_LANG) {
            auto [lhs_l, lhs_s] = Conversions::unpack_string_lang(lhs_oid);
            auto [rhs_l, rhs_s] = Conversions::unpack_string_lang(rhs_oid);
            if (lhs_l != rhs_l) {
                return ObjectId::get_null();
            }
            lhs_str = std::move(lhs_s);
            rhs_str = std::move(rhs_s);
        } else {
            lhs_str = Conversions::to_lexical_str(lhs_oid);
            rhs_str = Conversions::to_lexical_str(rhs_oid);
        }

        if (rhs_str.size() == 0) {
            return ObjectId(ObjectId::BOOL_TRUE);
        }
        if (lhs_str.size() == 0) {
            return ObjectId(ObjectId::BOOL_FALSE);
        }

        auto it = lhs_str.find(rhs_str);
        return ObjectId(ObjectId::MASK_BOOL | (it != std::string::npos));
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "CONTAINS(" << *lhs << ", " << *rhs << ")";
        return os;
    }
};
} // namespace SPARQL
