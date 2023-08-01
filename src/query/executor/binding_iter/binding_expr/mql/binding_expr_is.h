#pragma once

#include <memory>
#include <stdexcept>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/parser/expr/mql/bool_expr/expr_comaprision/expr_is.h"

namespace MQL {
class BindingExprIs : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    bool negation;

    ExprTypeName type;

    BindingExprIs(std::unique_ptr<BindingExpr> expr, bool negation, ExprTypeName type) :
        expr     (std::move(expr)),
        negation (negation),
        type     (type) { }

    ObjectId eval(const Binding& binding) override {
        auto oid = expr->eval(binding);
        bool res = false;
        switch (type) {
        case ExprTypeName::NULL_:
            res = (oid.id & ObjectId::TYPE_MASK) == ObjectId::MASK_NULL;
            break;
        case ExprTypeName::INTEGER:
            res = (oid.id & ObjectId::SUB_TYPE_MASK) == ObjectId::MASK_INT;
            break;
        case ExprTypeName::FLOAT:
            res = (oid.id & ObjectId::TYPE_MASK) == ObjectId::MASK_FLOAT;
            break;
        case ExprTypeName::BOOL:
            res = (oid.id & ObjectId::TYPE_MASK) == ObjectId::MASK_BOOL;
            break;
        case ExprTypeName::STRING:
            res = (oid.id & ObjectId::SUB_TYPE_MASK) == ObjectId::MASK_STRING_SIMPLE;
            break;
        default:
            throw std::logic_error("ExprTypeName case not implemented in BindingExprIs");
        }
        if (res ^ negation) {
            return ObjectId(ObjectId::BOOL_TRUE);
        } else {
            return ObjectId(ObjectId::BOOL_FALSE);
        }
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << '(' << *expr << (negation ? " IS NOT " : " IS ") << get_type_name(type) << ')';
        return os;
    }

private:
    std::string get_type_name(ExprTypeName type) const {
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
} //namespace MQL
