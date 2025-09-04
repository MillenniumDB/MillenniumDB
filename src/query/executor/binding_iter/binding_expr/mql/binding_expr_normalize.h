#pragma once

#include <memory>

#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "misc/transliterator.h"

namespace MQL {
class BindingExprNormalize : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprNormalize(std::unique_ptr<BindingExpr> expr) :
        expr(std::move(expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        const auto expr_oid = expr->eval(binding);

        if ((expr_oid.id & ObjectId::GENERIC_TYPE_MASK) != ObjectId::MASK_STRING_SIMPLE) {
            return ObjectId::get_null();
        }

        const std::string str = Conversions::unpack_string(expr_oid);
        const std::string normalized = Transliterator::get_instance()->nfkd_casefold(str);

        return MQL::Conversions::pack_string(normalized);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "NORMALIZE(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace MQL
