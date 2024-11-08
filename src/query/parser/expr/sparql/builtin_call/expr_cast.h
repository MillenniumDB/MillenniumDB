#pragma once

#include <memory>

#include "query/parser/expr/expr.h"

namespace SPARQL {

enum class CastType {
    xsd_boolean,
    xsd_double,
    xsd_float,
    xsd_decimal,
    xsd_integer,
    xsd_dateTime,
    xsd_string,
};

inline std::string cast_type_to_string(CastType cast_type) {
    switch (cast_type) {
        case CastType::xsd_boolean:  return "xsd:boolean";
        case CastType::xsd_double:   return "xsd:double";
        case CastType::xsd_float:    return "xsd:float";
        case CastType::xsd_decimal:  return "xsd:decimal";
        case CastType::xsd_integer:  return "xsd:integer";
        case CastType::xsd_dateTime: return "xsd:dateTime";
        case CastType::xsd_string:   return "xsd:string";
        default: return "Invalid CastType";
    }
}

class ExprCast : public Expr {
public:
    CastType cast_type;
    std::unique_ptr<Expr> expr;

    ExprCast(CastType cast_type, std::unique_ptr<Expr> expr) :
        cast_type(cast_type),
        expr     (std::move(expr)) { }

    virtual std::unique_ptr<Expr> clone() const override {
        return std::make_unique<ExprCast>(cast_type, expr->clone());
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return expr->get_all_vars();
    }

    bool has_aggregation() const override {
        return expr->has_aggregation();
    }
};
} // namespace SPARQL
