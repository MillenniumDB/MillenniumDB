#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace MQL {
enum class ExprTypeName {
    INTEGER,
    FLOAT,
    STRING,
    BOOL,
    NULL_
};

class ExprIs : public Expr {
public:
    bool negation;

    std::unique_ptr<Expr> expr;

    ExprTypeName type;

    ExprIs(bool negation, std::unique_ptr<Expr> expr, const std::string& type) :
        negation (negation),
        expr     (std::move(expr)),
        type     (parse_type(type)) { }

    ExprIs(bool negation, std::unique_ptr<Expr> expr, ExprTypeName type_name) :
        negation (negation),
        expr     (std::move(expr)),
        type     (type_name) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprIs>(negation, expr->clone(), type);
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool has_aggregation() const override {
        return expr->has_aggregation();
    }

    std::set<VarId> get_all_vars() const override {
        return expr->get_all_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(indent, ' ') << '(' << *expr << " IS " << (negation ? "NOT " : "") << get_type_name() <<  ')';
    }

private:
    static ExprTypeName parse_type(const std::string& str) {
        if (str == "null") {
            return ExprTypeName::NULL_;
        } else if (str == "integer") {
            return ExprTypeName::INTEGER;
        } else if (str == "float") {
            return ExprTypeName::FLOAT;
        } else if (str == "string") {
            return ExprTypeName::STRING;
        } else {
            return ExprTypeName::BOOL;
        }
    }

    std::string get_type_name() const {
        switch (type) {
            case ExprTypeName::NULL_:
                return "NULL";
            case ExprTypeName::INTEGER:
                return "INTEGER";
            case ExprTypeName::FLOAT:
                return "FLOAT";
            case ExprTypeName::BOOL:
                return "BOOL";
            case ExprTypeName::STRING:
                return "STRING";
        }
        return "";
    }
};
} // namespace MQL
