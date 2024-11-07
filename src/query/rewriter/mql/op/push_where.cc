#include "push_where.h"

#include "query/parser/op/mql/ops.h"
#include "query/parser/expr/mql_exprs.h"

using namespace MQL;

void PushWhere::visit(OpMatch& op_match) {
    op_match.fixed_properties = std::move(fixed_properties);
}


void PushWhere::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);
    if (must_delete_where) {
        must_delete_where = false;
        op_order_by.op = std::move(where_child);
    }
}


void PushWhere::visit(OpProjectSimilarity& op_project_similarity) {
    op_project_similarity.op->accept_visitor(*this);
    if (must_delete_where) {
        must_delete_where = false;
        op_project_similarity.op = std::move(where_child);
    }
}


void PushWhere::visit(OpBruteSimilaritySearch& op_brute_similarity_search) {
    op_brute_similarity_search.op->accept_visitor(*this);
    if (must_delete_where) {
        must_delete_where = false;
        op_brute_similarity_search.op = std::move(where_child);
    }
}


void PushWhere::visit(OpReturn& op_return) {
    op_return.op->accept_visitor(*this);
    if (must_delete_where) {
        must_delete_where = false;
        op_return.op = std::move(where_child);
    }
}


void PushWhere::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);
    if (must_delete_where) {
        must_delete_where = false;
        op_group_by.op = std::move(where_child);
    }
}


void PushWhere::visit(OpWhere& op_where) {
    PushWhereExpr expr_visitor(fixed_properties);
    op_where.expr->accept_visitor(expr_visitor);
    op_where.op->accept_visitor(*this);

    if (expr_visitor.can_remove) {
        must_delete_where = true;
        where_child = std::move(op_where.op);
    } else if (expr_visitor.remaining_expr != nullptr) {
        op_where.expr = std::move(expr_visitor.remaining_expr);
    }
}


void PushWhere::visit(OpSet& op_set) {
    op_set.op->accept_visitor(*this);
}


//////////////////////////////////////////////////////////////
void PushWhereExpr::visit(ExprAnd& expr_and) {
    std::vector<std::unique_ptr<Expr>> remaining_and;

    for (auto& expr : expr_and.and_list) {
        can_remove = true;
        expr->accept_visitor(*this);
        if (can_remove) {

        } else {
            if (remaining_expr != nullptr) {
                remaining_and.push_back(std::move(remaining_expr));
            } else {
                remaining_and.push_back(std::move(expr));
            }
        }
    }

    if (remaining_and.size() == 0) {
        can_remove = true;
    } else if (remaining_and.size() == 1) {
        remaining_expr = std::move(remaining_and[0]);
        can_remove = false;
    } else {
        expr_and.and_list = std::move(remaining_and);
        can_remove = false;
    }
}


void PushWhereExpr::visit(ExprEquals& expr_equals) {
    ExprConstant* expr_constant;
    ExprVarProperty* expr_var_prop;
    if (   dynamic_cast<ExprConstant*>(expr_equals.lhs.get()) != nullptr
        && dynamic_cast<ExprVarProperty*>(expr_equals.rhs.get()) != nullptr)
    {
        expr_constant = dynamic_cast<ExprConstant*>(expr_equals.lhs.get());
        expr_var_prop = dynamic_cast<ExprVarProperty*>(expr_equals.rhs.get());
    }

    else if (dynamic_cast<ExprVarProperty*>(expr_equals.lhs.get()) != nullptr
          && dynamic_cast<ExprConstant*>(expr_equals.rhs.get()) != nullptr)
    {
        expr_constant = dynamic_cast<ExprConstant*>(expr_equals.rhs.get());
        expr_var_prop = dynamic_cast<ExprVarProperty*>(expr_equals.lhs.get());
    }
    else {
        can_remove = false;
        return;
    }

    fixed_properties.push_back({
        expr_var_prop->var_without_property,
        expr_var_prop->key,
        expr_constant->value,
        expr_var_prop->var_with_property
    });
    can_remove = true;
}

void PushWhereExpr::visit(ExprAddition&)        { can_remove = false; }
void PushWhereExpr::visit(ExprDivision&)        { can_remove = false; }
void PushWhereExpr::visit(ExprModulo&)          { can_remove = false; }
void PushWhereExpr::visit(ExprMultiplication&)  { can_remove = false; }
void PushWhereExpr::visit(ExprSubtraction&)     { can_remove = false; }
void PushWhereExpr::visit(ExprUnaryMinus&)      { can_remove = false; }
void PushWhereExpr::visit(ExprUnaryPlus&)       { can_remove = false; }
void PushWhereExpr::visit(ExprGreaterOrEquals&) { can_remove = false; }
void PushWhereExpr::visit(ExprGreater&)         { can_remove = false; }
void PushWhereExpr::visit(ExprIs&)              { can_remove = false; }
void PushWhereExpr::visit(ExprLessOrEquals&)    { can_remove = false; }
void PushWhereExpr::visit(ExprLess&)            { can_remove = false; }
void PushWhereExpr::visit(ExprNotEquals&)       { can_remove = false; }
void PushWhereExpr::visit(ExprNot&)             { can_remove = false; }
void PushWhereExpr::visit(ExprOr&)              { can_remove = false; }
void PushWhereExpr::visit(ExprRegex&)           { can_remove = false; }