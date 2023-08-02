#pragma once

#include <memory>
#include <sstream>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "storage/unified_storage.h"

namespace SPARQL {
class BindingExprLang : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprLang(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.get_generic_type()) {
        case ObjectId::MASK_STRING: {
            if (expr_oid.get_sub_type() == ObjectId::MASK_STRING_LANG) {
                auto lang_id = (expr_oid.id & ObjectId::MASK_LITERAL_TAG) >> ObjectId::STR_LANG_INLINE_BYTES * 8;
                std::stringstream ss;
                UnifiedStorage::print_language(ss, lang_id);
                return Conversions::pack_string_simple(ss.str());
            } else {
                return ObjectId(ObjectId::STRING_SIMPLE_EMPTY);
            }
        }
        case ObjectId::MASK_NUMERIC:
        case ObjectId::MASK_DT:
        case ObjectId::MASK_BOOL: {
             return ObjectId(ObjectId::STRING_SIMPLE_EMPTY);
        }
        default: {
            return ObjectId::get_null();
        }
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "LANG(" << *expr << ")";
        return os;
    }
};
} // namespace SPARQL
