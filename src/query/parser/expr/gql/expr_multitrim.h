#pragma once

#include <string>

#include "query/parser/expr/gql/expr.h"

namespace GQL {
class ExprMultiTrim : public Expr {
public:
    std::unique_ptr<Expr> trim_src;
    // delim_str may be nullptr
    std::unique_ptr<Expr> delim_str;
    std::string specification;

    ExprMultiTrim(std::unique_ptr<Expr> trim_src, std::unique_ptr<Expr> delim_str, std::string specification) :
        trim_src(std::move(trim_src)),
        delim_str(std::move(delim_str)),
        specification(std::move(specification))
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprMultiTrim>(trim_src->clone(), delim_str->clone(), specification);
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        return trim_src->has_aggregation();
    }

    std::set<VarId> get_all_vars() const override
    {
        return trim_src->get_all_vars();
    }
};
} // namespace GQL
