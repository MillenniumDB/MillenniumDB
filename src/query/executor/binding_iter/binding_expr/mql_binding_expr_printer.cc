#include "mql_binding_expr_printer.h"


namespace MQL {


void BindingExprPrinter::visit(BindingExprAnd& expr) {
    os << '(';
    auto first = true;
    for (auto& expr : expr.and_list) {
        if (first) first = false; else os << " AND ";
        expr->accept_visitor(*this);
    }
    os << ')';
}


void BindingExprPrinter::visit(BindingExprEquals& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " = ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprIs& expr) {
    os << '(';
    expr.expr->accept_visitor(*this);
    os << (expr.negation ? " IS NOT " : " IS ");
    os << expr.get_type_name(expr.type) << ')';

}


void BindingExprPrinter::visit(BindingExprLessOrEquals& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " <= ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprLess& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << "<";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprModulo& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " % ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprNotEquals& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " != ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprNot& expr) {
    os << "!";
    expr.expr->accept_visitor(*this);
}


void BindingExprPrinter::visit(BindingExprOr& expr) {
    os << '(';
    auto first = true;
    for (auto& expr : expr.or_list) {
        if (first) first = false; else os << " OR ";
        expr->accept_visitor(*this);
    }
    os << ')';
}


} // namespace MQL