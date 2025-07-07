#pragma once

#include <optional>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/optimizer/quad_model/binding_iter_constructor.h"
#include "query/parser/expr/mql/expr.h"
#include "query/parser/expr/mql/expr_visitor.h"

namespace MQL {

class BindingIterConstructor;

// This visitor returns nullptr if condition is pushed outside
class ExprToBindingExpr : public ExprVisitor {
public:
    BindingIterConstructor* bic;
    std::vector<PropertyTypeConstraint> fixed_types_properties;

    // In expressions like "<expressions> AS <var>", as_var is <var>
    const std::optional<VarId> as_var;

    // Where result will be stored
    std::unique_ptr<BindingExpr> tmp;

    // Has to be true while visiting something inside of an aggregation
    // and be false otherwise
    bool inside_aggregation = false;

    // true if expression is from order by or return
    bool after_group;

    // For Aggregations in RETURN, ORDER BY
    // (for now the language only permits Agg Functions in them)
    ExprToBindingExpr(BindingIterConstructor* _bic, std::optional<VarId> as_var, bool after_group) :
        bic(_bic),
        fixed_types_properties(_bic->fixed_types_properties),
        as_var(as_var),
        after_group(after_group)
    { }

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
    void visit(ExprRegex&) override;
    void visit(ExprCosineSimilarity&) override;
    void visit(ExprCosineDistance&) override;
    void visit(ExprManhattanDistance&) override;
    void visit(ExprEuclideanDistance&) override;

private:

    template<typename AggType, class ... Args>
    void check_and_make_aggregate(Expr*, Args&&... args);
};
} // namespace MQL
