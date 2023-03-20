#include "expr_to_binding_condition.h"

#include "execution/binding_iter/binding_expr/binding_exprs.h"
#include "parser/query/expr/exprs.h"

using namespace std;

Expr2BindingExpr::Expr2BindingExpr(const std::map<Var, VarId>& var2var_ids) :
    var2var_ids (var2var_ids) { }


void Expr2BindingExpr::visit(ExprConstant& expr_constant) {
    current_binding_expr = make_unique<BindingExprAtomConstant>(QueryElement::deduce(expr_constant.value));
}


void Expr2BindingExpr::visit(ExprVar& expr_var) {
    auto find_var_id = var2var_ids.find(expr_var.var);
    assert(find_var_id != var2var_ids.end()
            && "Variable names inside WHERE need to be checked before processing conditions");

    current_binding_expr = make_unique<BindingExprAtomVar>(find_var_id->second);
}


void Expr2BindingExpr::visit(ExprVarProperty& expr_var_property) {
    auto find_var_id = var2var_ids.find(expr_var_property.property_var);
    assert(find_var_id != var2var_ids.end()
            && "Variable names inside WHERE need to be checked before processing conditions");

    current_binding_expr = make_unique<BindingExprAtomVar>(find_var_id->second);
}


void Expr2BindingExpr::visit(ExprAddition& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(current_binding_expr);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(current_binding_expr);

    current_binding_expr = make_unique<BindingExprAddition>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}


void Expr2BindingExpr::visit(ExprDivision& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(current_binding_expr);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(current_binding_expr);

    current_binding_expr = make_unique<BindingExprDivision>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}


void Expr2BindingExpr::visit(ExprModulo& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(current_binding_expr);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(current_binding_expr);

    current_binding_expr = make_unique<BindingExprModulo>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}


void Expr2BindingExpr::visit(ExprMultiplication& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(current_binding_expr);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(current_binding_expr);

    current_binding_expr = make_unique<BindingExprMultiplication>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}


void Expr2BindingExpr::visit(ExprSubtraction& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(current_binding_expr);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(current_binding_expr);

    current_binding_expr = make_unique<BindingExprSubtraction>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}


void Expr2BindingExpr::visit(ExprUnaryMinus& expr) {
    expr.expr->accept_visitor(*this);
    current_binding_expr = make_unique<BindingExprUnaryMinus>(std::move(current_binding_expr));
}


void Expr2BindingExpr::visit(ExprUnaryPlus& expr) {
    expr.expr->accept_visitor(*this);
    current_binding_expr = make_unique<BindingExprUnaryPlus>(std::move(current_binding_expr));
}


void Expr2BindingExpr::visit(ExprEquals& expr) {
    if (can_push_outside) {
        auto var_prop_ptr = dynamic_cast<ExprVarProperty*>(expr.lhs.get());
        auto constant_ptr = dynamic_cast<ExprConstant*>(expr.rhs.get());

        if (var_prop_ptr == nullptr || constant_ptr == nullptr) {
            constant_ptr = dynamic_cast<ExprConstant*>(expr.lhs.get());
            var_prop_ptr = dynamic_cast<ExprVarProperty*>(expr.rhs.get());
        }
        if (var_prop_ptr != nullptr && constant_ptr != nullptr) {
            properties.push_back({var_prop_ptr->object_var,
                                  var_prop_ptr->property_key,
                                  QueryElement::deduce(constant_ptr->value)});
            return;
        }
    }

    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(current_binding_expr);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(current_binding_expr);

    current_binding_expr = make_unique<BindingExprEquals>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}


void Expr2BindingExpr::visit(ExprGreaterOrEquals& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(current_binding_expr);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(current_binding_expr);

    current_binding_expr = make_unique<BindingExprGreaterOrEquals>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}


void Expr2BindingExpr::visit(ExprGreater& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(current_binding_expr);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(current_binding_expr);

    current_binding_expr = make_unique<BindingExprGreater>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}


void Expr2BindingExpr::visit(ExprIs& expr) {
    expr.expr->accept_visitor(*this);
    current_binding_expr = make_unique<BindingExprIs>(std::move(current_binding_expr), expr.negation, expr.type);
}


void Expr2BindingExpr::visit(ExprLessOrEquals& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(current_binding_expr);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(current_binding_expr);

    current_binding_expr = make_unique<BindingExprLessOrEquals>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}


void Expr2BindingExpr::visit(ExprLess& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(current_binding_expr);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(current_binding_expr);

    current_binding_expr = make_unique<BindingExprLess>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}


void Expr2BindingExpr::visit(ExprNotEquals& expr) {
    expr.lhs->accept_visitor(*this);
    auto lhs_binding_expr = std::move(current_binding_expr);
    expr.rhs->accept_visitor(*this);
    auto rhs_binding_expr = std::move(current_binding_expr);

    current_binding_expr = make_unique<BindingExprNotEquals>(std::move(lhs_binding_expr), std::move(rhs_binding_expr));
}


void Expr2BindingExpr::visit(ExprAnd& expr) {
    std::vector<std::unique_ptr<BindingExpr>> and_list;
    for (auto& e : expr.and_list) {
        e->accept_visitor(*this);
        if (current_binding_expr != nullptr) {
            and_list.push_back(std::move(current_binding_expr));
        }
    }
    if (and_list.size() > 1) {
        current_binding_expr = make_unique<BindingExprAnd>(std::move(and_list));
    } else if (and_list.size() == 1) {
        current_binding_expr = std::move(and_list[0]);
    }
}


void Expr2BindingExpr::visit(ExprNot& expr) {
    auto original_can_push_outside = can_push_outside;
    can_push_outside = false;
    expr.expr->accept_visitor(*this);
    current_binding_expr = make_unique<BindingExprNot>(std::move(current_binding_expr));
    can_push_outside = original_can_push_outside;
}


void Expr2BindingExpr::visit(ExprOr& expr) {
    auto original_can_push_outside = can_push_outside;
    can_push_outside = false;
    std::vector<std::unique_ptr<BindingExpr>> or_list;
    for (auto& e : expr.or_list) {
        e->accept_visitor(*this);
        or_list.push_back(std::move(current_binding_expr));
    }
    current_binding_expr = make_unique<BindingExprOr>(std::move(or_list));
    can_push_outside = original_can_push_outside;
}
