#pragma once

#include <optional>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/parser/expr/gql/expr.h"
#include "query/parser/expr/gql/expr_visitor.h"
#include "query/var_id.h"

namespace GQL {

class PathBindingIterConstructor;

class ExprToBindingExpr : public ExprVisitor {
public:
    PathBindingIterConstructor* bic = nullptr;

    // In expressions like "<expressions> AS <var>", as_var is <var>
    const std::optional<VarId> as_var;

    // Where the result will be stored
    std::unique_ptr<BindingExpr> tmp;

    // Has to be true while visiting something inside of an aggregation
    // and be false otherwise
    bool inside_aggregation = false;

    bool at_root = true; // The visitor is currently at the root node

    // true if expression is from order by or return
    bool after_group = false;

    // This constructor is used to visit expressions that must not have aggregations or group variables.
    ExprToBindingExpr() :
        bic(nullptr)
    { }

    // This constructor is used to visit expressions that can have aggregations or group variables.
    ExprToBindingExpr(PathBindingIterConstructor* bic, std::optional<VarId> as_var, bool after_group) :
        bic(bic),
        as_var(as_var),
        after_group(after_group)
    { }

    template<typename AggType, class... Args>
    void check_and_make_aggregate(Expr*, Args&&... args);

    void visit(ExprOr&) override;
    void visit(ExprAnd&) override;
    void visit(ExprNot&) override;
    void visit(ExprHasNodeLabel&) override;
    void visit(ExprHasEdgeLabel&) override;
    void visit(ExprWildcardLabel&) override;

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
    void visit(ExprAbs&) override;
    void visit(ExprModulo&) override;
    void visit(ExprSin&) override;
    void visit(ExprCos&) override;
    void visit(ExprTan&) override;
    void visit(ExprCot&) override;
    void visit(ExprSinh&) override;
    void visit(ExprCosh&) override;
    void visit(ExprTanh&) override;
    void visit(ExprAsin&) override;
    void visit(ExprAcos&) override;
    void visit(ExprAtan&) override;
    void visit(ExprDegrees&) override;
    void visit(ExprRadians&) override;
    void visit(ExprLog&) override;
    void visit(ExprLog10&) override;
    void visit(ExprLn&) override;
    void visit(ExprExp&) override;
    void visit(ExprPower&) override;
    void visit(ExprSqrt&) override;
    void visit(ExprFloor&) override;
    void visit(ExprCeil&) override;
    void visit(ExprLength&) override;
    void visit(ExprSubStr&) override;
    void visit(ExprFold&) override;
    void visit(ExprSingleTrim&) override;
    void visit(ExprMultiTrim&) override;
    void visit(ExprNormalize&) override;
    void visit(ExprNullIf&) override;
    void visit(ExprCoalesce&) override;
    void visit(ExprSimpleCase&) override;
    void visit(ExprSearchedCase&) override;
    void visit(ExprCast&) override;

    void visit(ExprAggCountAll&) override;
    void visit(ExprAggCount&) override;
    void visit(ExprAggAvg&) override;
    void visit(ExprAggMax&) override;
    void visit(ExprAggMin&) override;
    void visit(ExprAggSum&) override;
    void visit(ExprAggStddevPop&) override;
    void visit(ExprAggStddevSamp&) override;
    void visit(ExprAggCollect&) override;
    void visit(ExprAggPercentileCont&) override;
    void visit(ExprAggPercentileDisc&) override;
};
} // namespace GQL
