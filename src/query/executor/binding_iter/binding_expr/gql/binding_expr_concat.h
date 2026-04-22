#pragma once

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>

namespace GQL {

enum class ConcatStatus {
    UNSET,
    LANG,
    XSD,
    SIMPLE
};

class BindingExprConcat : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprConcat(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        std::string res;

        ConcatStatus status = ConcatStatus::UNSET;

        auto expr_oid = lhs->eval(binding);
        switch (expr_oid.subtype()) {
        case ObjectSubType::String: {
            auto str = GQL::Conversions::unpack_string(expr_oid);
            res += str;
            break;
        }
        default:
            return ObjectId::get_null();
        }

        expr_oid = rhs->eval(binding);
        switch (expr_oid.subtype()) {
        case ObjectSubType::String: {
            auto str = GQL::Conversions::unpack_string(expr_oid);
            res += str;
            break;
        }
        default:
            return ObjectId::get_null();
        }

        switch (status) {
        case ConcatStatus::UNSET:
        case ConcatStatus::SIMPLE:
            return GQL::Conversions::pack_string(res);
        default:
            assert(false);
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << '(';
        lhs->print(os, ops);
        os << " CONCAT ";
        rhs->print(os, ops);
        os << ')';
    }
};
} // namespace GQL
