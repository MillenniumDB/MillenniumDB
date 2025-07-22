#pragma once

#include <memory>
#include <stdexcept>

#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/parser/expr/mql/bool_expr/expr_comaprision/expr_is.h"

namespace MQL {
class BindingExprIs : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    bool negation;

    ExprIs::TypeName type;

    BindingExprIs(std::unique_ptr<BindingExpr> expr, bool negation, ExprIs::TypeName type) :
        expr(std::move(expr)),
        negation(negation),
        type(type)
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto oid = expr->eval(binding);
        bool res = false;
        switch (type) {
        case ExprIs::TypeName::NULL_:
            res = (oid.id & ObjectId::TYPE_MASK) == ObjectId::MASK_NULL;
            break;
        case ExprIs::TypeName::INTEGER:
            res = (oid.id & ObjectId::SUB_TYPE_MASK) == ObjectId::MASK_INT;
            break;
        case ExprIs::TypeName::FLOAT:
            res = (oid.id & ObjectId::TYPE_MASK) == ObjectId::MASK_FLOAT;
            break;
        case ExprIs::TypeName::BOOL:
            res = (oid.id & ObjectId::TYPE_MASK) == ObjectId::MASK_BOOL;
            break;
        case ExprIs::TypeName::STRING:
            res = (oid.id & ObjectId::SUB_TYPE_MASK) == ObjectId::MASK_STRING_SIMPLE;
            break;
        default:
            throw std::logic_error("ExprIs::TypeName case not implemented in BindingExprIs");
        }

        return Conversions::pack_bool(res ^= negation);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::string get_type_name(ExprIs::TypeName type) const
    {
        switch (type) {
        case ExprIs::TypeName::NULL_:
            return "NULL";
        case ExprIs::TypeName::INTEGER:
            return "INTEGER";
        case ExprIs::TypeName::FLOAT:
            return "FLOAT";
        case ExprIs::TypeName::BOOL:
            return "BOOL";
        case ExprIs::TypeName::STRING:
            return "STRING";
        }
        return "";
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << '(';
        expr->print(os, ops);
        os << (negation ? " IS NOT " : " IS ");
        os << get_type_name(type) << ')';
    }
};
} //namespace MQL
