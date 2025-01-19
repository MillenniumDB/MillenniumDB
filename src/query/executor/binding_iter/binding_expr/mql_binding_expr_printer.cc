#include "mql_binding_expr_printer.h"


namespace MQL {


void BindingExprPrinter::visit(BindingExprAddition& expr){
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " + ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(BindingExprAnd& expr) {
    os << '(';
    auto first = true;
    for (auto& expr : expr.and_list) {
        if (first) first = false; else os << " AND ";
        expr->accept_visitor(*this);
    }
    os << ')';
}

void BindingExprPrinter::visit(BindingExprDivision& expr){
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " / ";
    expr.rhs->accept_visitor(*this);
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

void BindingExprPrinter::visit(BindingExprMultiplication& expr){
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " * ";
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

void BindingExprPrinter::visit(BindingExprSubtraction& expr){
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " - ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(BindingExprUnaryMinus& expr){
    os << "-(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(BindingExprUnaryPlus& expr){
    os << "+(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(BindingExprRegex& expr) {
    os << "REGEX(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    if (expr.expr3 != nullptr) {
        os << ", ";
        expr.expr3->accept_visitor(*this);
    }
    os << ")";
}

void BindingExprPrinter::visit(BindingExprTensorDistanceFromExpr& expr) {
    os << "TENSOR_DISTANCE_FROM_EXPR(";
    os << expr.tensor_store->name() ;
    os << ", ";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    os << ", ";
    os << expr.metric_type;
    os << ")";
}

void BindingExprPrinter::visit(BindingExprTensorDistanceFromTensor& expr) {
    os << "TENSOR_DISTANCE_FROM_EXPR(";
    os << expr.tensor_store->name() ;
    os << ", ";
    expr.expr1->accept_visitor(*this);
    os << ", [";
    os << expr.tensor[0];
    for (std::size_t i = 1; i < expr.tensor.size(); ++i) {
        os << ", ";
        os << expr.tensor[i];
    }
    os << "], ";
    os << expr.metric_type;
    os << ")";
}


} // namespace MQL