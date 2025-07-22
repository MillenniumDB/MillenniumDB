#pragma once

#include <string>

#include "query/parser/expr/gql/expr.h"

namespace GQL {
class ExprSingleTrim : public Expr {
public:
    std::unique_ptr<Expr> str;
    // single_char may be nullptr
    std::unique_ptr<Expr> single_char;
    std::string specification;

    ExprSingleTrim(std::unique_ptr<Expr> str, std::unique_ptr<Expr> single_char, std::string specification) :
        str(std::move(str)),
        single_char(std::move(single_char)),
        specification(std::move(specification))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprSingleTrim>(str->clone(), single_char->clone(), specification);
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        return str->has_aggregation();
    }

    std::set<VarId> get_all_vars() const override
    {
        return str->get_all_vars();
    }
};
} // namespace GQL
