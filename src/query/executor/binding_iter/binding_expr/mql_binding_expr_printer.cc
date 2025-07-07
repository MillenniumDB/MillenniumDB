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

void BindingExprPrinter::visit(BindingExprCosineSimilarity& expr)
{
    os << "COSINE_SIMILARITY(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprCosineDistance& expr) {
    os << "COSINE_DISTANCE(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprManhattanDistance& expr) {
    os << "MANHATTAN DISTANCE(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprEuclideanDistance& expr) {
    os << "EUCLIDEAN_DISTANCE(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ")";
}

} // namespace MQL
