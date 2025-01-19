#pragma once

#include <optional>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/parser/expr/expr.h"
#include "query/parser/expr/expr_visitor.h"
#include "query/var_id.h"

namespace GQL {

class BindingIterConstructor;

class ExprToBindingExpr : public ExprVisitor {
public:
    BindingIterConstructor* bic = nullptr;

    // In expressions like "<expressions> AS <var>", as_var is <var>
    const std::optional<VarId> as_var;

    // Where the result will be stored
    std::unique_ptr<BindingExpr> tmp;

    // This constructor is used to visit expressions that must not have aggregations or group variables.
    ExprToBindingExpr() :
        bic(nullptr)
    { }

    // This constructor is used to visit expressions that can have aggregations or group variables.
    ExprToBindingExpr(BindingIterConstructor* bic, std::optional<VarId> as_var) :
        bic(bic),
        as_var(as_var)
    { }

    template<typename AggType, class... Args>
    void check_and_make_aggregate(Expr*, Args&&... args);

    void visit(ExprOr&) override;
    void visit(ExprAnd&) override;
    void visit(ExprNot&) override;
    void visit(ExprHasNodeLabel&) override;
    void visit(ExprHasEdgeLabel&) override;

    void visit(ExprEquals&) override;
    void visit(ExprVar&) override;
    void visit(ExprProperty&) override;
    void visit(ExprConcat&) override;
    void visit(ExprGreaterOrEquals&) override;
    void visit(ExprGreater&) override;
    void visit(ExprIs&) override;
    void visit(ExprLessOrEquals&) override;
    void visit(ExprLess&) override;
    void visit(ExprNotEquals&) override;
    void visit(ExprXor&) override;
    void visit(ExprTerm&) override;
    void visit(ExprAddition&) override;
    void visit(ExprSubtraction&) override;
    void visit(ExprMultiplication&) override;
    void visit(ExprDivision&) override;
};
} // namespace GQL
