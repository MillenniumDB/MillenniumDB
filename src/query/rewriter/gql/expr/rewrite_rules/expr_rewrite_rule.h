#pragma once

#include "query/parser/expr/expr.h"
#include "query/parser/op/op.h"
#include <memory>

namespace GQL {

class ExprRewriteRule {
public:
    virtual ~ExprRewriteRule() = default;

    virtual bool is_possible_to_regroup(std::unique_ptr<Expr>&) = 0;
    virtual std::unique_ptr<Expr> regroup(std::unique_ptr<Expr>) = 0;

protected:
    template<typename T>
    bool is_castable_to(std::unique_ptr<Expr>& unknown_expr)
    {
        if (dynamic_cast<T*>(unknown_expr.get()) != nullptr) {
            return true;
        }
        return false;
    }

    template<typename T>
    std::unique_ptr<T> cast_to(std::unique_ptr<Expr> unknown_expr)
    {
        std::unique_ptr<T> casted_optional(dynamic_cast<T*>(unknown_expr.release()));
        return casted_optional;
    }

    template<typename T>
    bool is_castable_to(std::unique_ptr<Op>& unknown_op)
    {
        if (dynamic_cast<T*>(unknown_op.get()) != nullptr) {
            return true;
        }
        return false;
    }

    template<typename T>
    std::unique_ptr<T> cast_to(std::unique_ptr<Op> unknown_op)
    {
        std::unique_ptr<T> casted_optional(dynamic_cast<T*>(unknown_op.release()));
        return casted_optional;
    }
};
} // namespace GQL
