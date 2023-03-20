#pragma once

#include <map>
#include <memory>
#include <vector>

#include "base/query/query_element.h"
#include "base/ids/object_id.h"
#include "base/query/var.h"
#include "parser/query/expr/expr_visitor.h"
#include "execution/binding_iter/binding_expr/binding_expr.h"

// This visitor returns nullptr if condition is pushed outside
class Expr2BindingExpr : public ExprVisitor {
public:
    const std::map<Var, VarId>& var2var_ids;

    std::unique_ptr<BindingExpr> current_binding_expr;

    std::vector<std::tuple<Var, std::string, QueryElement>> properties;

    // This is a variable that may change when visiting an Expr, but the visit method
    // must restore the original value before returning
    bool can_push_outside = true;

    Expr2BindingExpr(const std::map<Var, VarId>& var2var_ids);

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
