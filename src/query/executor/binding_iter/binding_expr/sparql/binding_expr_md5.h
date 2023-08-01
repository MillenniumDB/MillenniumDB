#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "third_party/hashes/md5.h"

namespace SPARQL {
class BindingExprMD5 : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprMD5(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        switch (expr_oid.get_sub_type()) {
        case ObjectId::MASK_STRING_SIMPLE: {
            MD5 md5;
            std::string str  = Conversions::unpack_string_simple(expr_oid);
            std::string hash = md5(str);
            return Conversions::pack_string_simple(hash);
        }
        default:
            return ObjectId::get_null();
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "MD5(" << *expr << ")";
        return os;
    }
};
} // namespace SPARQL
