#pragma once

#include <optional>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/parser/expr/expr.h"
#include "query/parser/expr/expr_visitor.h"

namespace MQL {

class BindingIterConstructor;

// This visitor returns nullptr if condition is pushed outside
class ExprToBindingExpr : public ExprVisitor {
private:
    BindingIterConstructor* bic;

    const std::optional<VarId> var;

    // Has to be true while visiting something inside of an aggregation
    // and be false otherwise
    bool inside_aggregation = false;

public:
    ExprToBindingExpr() :
        bic (nullptr) { }

    // For Aggregations in RETURN, ORDER BY
    // (for now the language only permits Agg Functions in them)
    ExprToBindingExpr(
        BindingIterConstructor* bic,
        VarId var
    ) :
        bic (bic),
        var (var) { }

    std::unique_ptr<BindingExpr> tmp;

    void visit(ExprAggAvg&) override;
    void visit(ExprAggCountAll&) override;
    void visit(ExprAggCount&) override;
    void visit(ExprAggMax&) override;
    void visit(ExprAggMin&) override;
    void visit(ExprAggSum&) override;

    void visit(ExprVar&) override;
    void visit(ExprVarProperty&) override;
    void visit(ExprConstant&) override;
    void visit(ExprAddition&) override;
    void visit(ExprDivision&) override;
    void visit(ExprModulo&) override;
    void visit(ExprMultiplication&) override;
    void visit(ExprSubtraction&) override;
    void visit(ExprUnaryMinus&) override;
    void visit(ExprUnaryPlus&) override;
    void visit(ExprEquals&) override;
    void visit(ExprGreaterOrEquals&) override;
    void visit(ExprGreater&) override;
    void visit(ExprIs&) override;
    void visit(ExprLessOrEquals&) override;
    void visit(ExprLess&) override;
    void visit(ExprNotEquals&) override;
    void visit(ExprAnd&) override;
    void visit(ExprNot&) override;
    void visit(ExprOr&) override;
};
} // namespace MQL
