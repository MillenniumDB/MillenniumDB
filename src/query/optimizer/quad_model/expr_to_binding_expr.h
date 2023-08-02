#pragma once

#include <chrono>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <unordered_map>

#include <boost/uuid/uuid_generators.hpp>

#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/query_context.h"
#include "query/var_id.h"
// #include "query/executor/binding_iter/projection_order_exprs.h"
#include "query/parser/expr/expr.h"
#include "query/parser/expr/expr_visitor.h"

namespace MQL {

// This visitor returns nullptr if condition is pushed outside
class ExprToBindingExpr : public ExprVisitor {
public:
    std::unique_ptr<BindingExpr> tmp;

    // TODO:
    // std::vector<std::tuple<VarId, std::string, QueryElement>> properties;

    // This is a variable that may change when visiting an Expr, but the visit method
    // must restore the original value before returning
    bool can_push_outside = true;

    void visit(MQL::ExprVar&) override;
    void visit(MQL::ExprConstant&) override;
    void visit(MQL::ExprAddition&) override;
    void visit(MQL::ExprDivision&) override;
    void visit(MQL::ExprModulo&) override;
    void visit(MQL::ExprMultiplication&) override;
    void visit(MQL::ExprSubtraction&) override;
    void visit(MQL::ExprUnaryMinus&) override;
    void visit(MQL::ExprUnaryPlus&) override;
    void visit(MQL::ExprEquals&) override;
    void visit(MQL::ExprGreaterOrEquals&) override;
    void visit(MQL::ExprGreater&) override;
    void visit(MQL::ExprIs&) override;
    void visit(MQL::ExprLessOrEquals&) override;
    void visit(MQL::ExprLess&) override;
    void visit(MQL::ExprNotEquals&) override;
    void visit(MQL::ExprAnd&) override;
    void visit(MQL::ExprNot&) override;
    void visit(MQL::ExprOr&) override;
};
} // namespace MQL
