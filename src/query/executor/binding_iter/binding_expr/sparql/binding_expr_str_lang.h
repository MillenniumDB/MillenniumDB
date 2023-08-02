#pragma once

#include <memory>

#include "graph_models/inliner.h"
#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "storage/unified_storage.h"

namespace SPARQL {
class BindingExprStrLang : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr1;
    std::unique_ptr<BindingExpr> expr2;

    BindingExprStrLang(std::unique_ptr<BindingExpr> expr1, std::unique_ptr<BindingExpr> expr2) :
        expr1 (std::move(expr1)),
        expr2 (std::move(expr2)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr1_oid = expr1->eval(binding);
        auto expr2_oid = expr2->eval(binding);

        auto expr1_subtype = expr1_oid.get_sub_type();
        if (expr1_subtype != ObjectId::MASK_STRING_SIMPLE &&
            expr1_subtype != ObjectId::MASK_STRING_XSD) {
            return ObjectId::get_null();
        }

        if (expr2_oid.get_generic_type() != ObjectId::MASK_STRING_SIMPLE) {
            return ObjectId::get_null();
        }

        auto str = Conversions::to_lexical_str(expr1_oid);
        uint64_t str_id;

        if (str.size() <= ObjectId::STR_LANG_INLINE_BYTES) {
            str_id = ObjectId::MASK_STRING_LANG_INLINED | Inliner::inline_string5(str.c_str());
        } else {
            str_id = UnifiedStorage::get_str_id(str, ObjectId::MASK_STRING_LANG);
        }

        auto lang = Conversions::unpack_string_simple(expr2_oid);
        uint64_t lang_id = UnifiedStorage::get_language_id(lang);

        return ObjectId(str_id | lang_id << ObjectId::STR_LANG_INLINE_BYTES * 8);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "strLANG(" << *expr1 << ", " << *expr2 << ")";
        return os;
    }
};
} // namespace SPARQL
