#pragma once

#include <memory>
#include <string>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprStrStarts : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprStrStarts(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
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
        } else if (lhs_sub == ObjectId::MASK_STRING_SIMPLE
                || lhs_sub == ObjectId::MASK_STRING_XSD) {
            lhs_str = Conversions::to_lexical_str(lhs_oid);
            rhs_str = Conversions::to_lexical_str(rhs_oid);
        } else {
            auto [lhs_l, lhs_s] = Conversions::unpack_string_lang(lhs_oid);
            lhs_str = lhs_s;
            rhs_str = Conversions::to_lexical_str(rhs_oid);
        }

        auto lhs_size = lhs_str.size();
        auto rhs_size = rhs_str.size();

        if (rhs_size == 0) {
            return ObjectId(ObjectId::BOOL_TRUE);
        }
        if (lhs_size == 0) {
            return ObjectId(ObjectId::BOOL_FALSE);
        }
        if (lhs_size < rhs_size) {
            return ObjectId(ObjectId::BOOL_FALSE);
        }

        auto prefix = lhs_str.substr(0, rhs_size);
        return ObjectId(ObjectId::MASK_BOOL | (prefix == rhs_str));
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "strSTARTS(" << *lhs << ", " << *rhs << ")";
        return os;
    }
};
} // namespace SPARQL
