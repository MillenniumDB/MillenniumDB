#pragma once

#include <memory>
#include <string>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprStrBefore : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprStrBefore(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
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

        bool return_lang = false;
        uint16_t lhs_lang;

        if (lhs_sub == ObjectId::MASK_STRING_LANG && rhs_sub == ObjectId::MASK_STRING_LANG) {
            auto [lhs_l, lhs_s] = Conversions::unpack_string_lang(lhs_oid);
            auto [rhs_l, rhs_s] = Conversions::unpack_string_lang(rhs_oid);
            if (lhs_l != rhs_l) {
                return ObjectId::get_null();
            }
            lhs_lang = lhs_l;
            return_lang = true;
            lhs_str = std::move(lhs_s);
            rhs_str = std::move(rhs_s);
        } else if (lhs_sub == ObjectId::MASK_STRING_SIMPLE
                || lhs_sub == ObjectId::MASK_STRING_XSD) {
            lhs_str = Conversions::to_lexical_str(lhs_oid);
            rhs_str = Conversions::to_lexical_str(rhs_oid);
        } else {
            auto [lhs_l, lhs_s] = Conversions::unpack_string_lang(lhs_oid);
            lhs_str = lhs_s;
            lhs_lang = lhs_l;
            return_lang = true;
            rhs_str = Conversions::to_lexical_str(rhs_oid);
        }

        if (rhs_str.size() == 0) {
            if (return_lang) {
                return ObjectId(ObjectId::MASK_STRING_LANG_INLINED | (static_cast<uint64_t>(lhs_lang) << ObjectId::STR_LANG_INLINE_BYTES * 8));
            } else if (lhs_sub == ObjectId::MASK_STRING_XSD){
                return ObjectId(ObjectId::STRING_XSD_EMPTY);
            } else {
                return ObjectId(ObjectId::STRING_SIMPLE_EMPTY);
            }
        }
        if (lhs_str.size() == 0) {
            return ObjectId(ObjectId::STRING_SIMPLE_EMPTY);
        }

        auto it = lhs_str.find(rhs_str);
        if (it != std::string::npos) {
            auto substr = lhs_str.substr(0, it);
            if (return_lang) {
                return Conversions::pack_string_lang(lhs_lang, substr);
            } else if (lhs_sub == ObjectId::MASK_STRING_XSD) {
                return Conversions::pack_string_xsd(substr);
            } else {
                return Conversions::pack_string_simple(substr);
            }
        }
        return ObjectId(ObjectId::STRING_SIMPLE_EMPTY);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "strBEFORE(" << *lhs << ", " << *rhs << ")";
        return os;
    }
};
} // namespace SPARQL
