#pragma once

#include <memory>

#include "query/parser/expr/mql/expr.h"

namespace MQL {

class ExprIs : public Expr {
public:
    enum class TypeName {
        INTEGER,
        FLOAT,
        STRING,
        BOOL,
        NULL_
    };

    bool negation;

    std::unique_ptr<Expr> expr;

    TypeName type;
    uint64_t type_bitmap;

    ExprIs(bool negation, std::unique_ptr<Expr> expr, TypeName type_name, uint64_t type_bitmap) :
        negation(negation),
        expr(std::move(expr)),
        type(type_name),
        type_bitmap(type_bitmap)
    { }

    virtual std::unique_ptr<Expr> clone() const override
    {
        return std::make_unique<ExprIs>(negation, expr->clone(), type, type_bitmap);
    }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        return expr->has_aggregation();
    }

    std::set<VarId> get_all_vars() const override
    {
        return expr->get_all_vars();
    }

    static TypeName parse_type(const std::string& str)
    {
        if (str == "null") {
            return TypeName::NULL_;
        } else if (str == "integer") {
            return TypeName::INTEGER;
        } else if (str == "float") {
            return TypeName::FLOAT;
        } else if (str == "string") {
            return TypeName::STRING;
        } else {
            return TypeName::BOOL;
        }
    }

    std::string get_type_name() const
    {
        switch (type) {
        case TypeName::NULL_:
            return "NULL";
        case TypeName::INTEGER:
            return "INTEGER";
        case TypeName::FLOAT:
            return "FLOAT";
        case TypeName::BOOL:
            return "BOOL";
        case TypeName::STRING:
            return "STRING";
        }
        return "";
    }
};
} // namespace MQL
