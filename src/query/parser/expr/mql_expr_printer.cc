#include "mql_expr_printer.h"

#include "query/parser/expr/mql_exprs.h"

using namespace MQL;

void ExprPrinter::visit(ExprVar& expr) {
    os << '?' << get_query_ctx().get_var_name(expr.var);
}


void ExprPrinter::visit(ExprVarProperty& expr) {
    os << '?' << get_query_ctx().get_var_name(expr.var_with_property);
}


void ExprPrinter::visit(ExprConstant& expr) {
    os  << expr.value;
}


void ExprPrinter::visit(ExprAddition& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " + ";
    expr.rhs->accept_visitor(*this);
    os << ')' ;
}


void ExprPrinter::visit(ExprDivision& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " / ";
    expr.rhs->accept_visitor(*this);
    os << ')' ;
}


void ExprPrinter::visit(ExprModulo& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " % ";
    expr.rhs->accept_visitor(*this);
    os << ')' ;
}


void ExprPrinter::visit(ExprMultiplication& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " * ";
    expr.rhs->accept_visitor(*this);
    os << ')' ;
}


void ExprPrinter::visit(ExprSubtraction& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " - ";
    expr.rhs->accept_visitor(*this);
    os << ')' ;
}


void ExprPrinter::visit(ExprUnaryMinus& expr) {
    os << '-';
    expr.expr->accept_visitor(*this);
}


void ExprPrinter::visit(ExprUnaryPlus& expr) {
    os << '+';
    expr.expr->accept_visitor(*this);
}


void ExprPrinter::visit(ExprEquals& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " == ";
    expr.rhs->accept_visitor(*this);
    os << ')' ;
}


void ExprPrinter::visit(ExprGreaterOrEquals& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " >= ";
    expr.rhs->accept_visitor(*this);
    os << ')' ;
}


void ExprPrinter::visit(ExprGreater& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " > ";
    expr.rhs->accept_visitor(*this);
    os << ')' ;
}


void ExprPrinter::visit(ExprIs& expr) {
    os << '(';
    expr.expr->accept_visitor(*this);
    os << " IS " << (expr.negation ? "NOT " : "");
    os << expr.get_type_name() <<  ')';
}


void ExprPrinter::visit(ExprLessOrEquals& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " <= ";
    expr.rhs->accept_visitor(*this);
    os << ')' ;
}


void ExprPrinter::visit(ExprLess& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " < ";
    expr.rhs->accept_visitor(*this);
    os << ')' ;
}


void ExprPrinter::visit(ExprNotEquals& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " != ";
    expr.rhs->accept_visitor(*this);
    os << ')' ;
}


void ExprPrinter::visit(ExprAnd& expr) {
    os << '(';
    expr.and_list[0]->accept_visitor(*this);
    for (size_t i = 1; i < expr.and_list.size(); i++) {
        os << " AND ";
        expr.and_list[i]->accept_visitor(*this);
    }
    os << ')' ;
}


void ExprPrinter::visit(ExprNot& expr) {
    os << '!';
    expr.expr->accept_visitor(*this);
}


void ExprPrinter::visit(ExprOr& expr) {
    os << '(';
    expr.or_list[0]->accept_visitor(*this);
    for (size_t i = 1; i < expr.or_list.size(); i++) {
        os << " OR ";
        expr.or_list[i]->accept_visitor(*this);
    }
    os << ')' ;
}


void ExprPrinter::visit(ExprRegex& expr) {
    os << "REGEX(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    if (expr.expr3 != nullptr) {
        os << ", ";
        expr.expr3->accept_visitor(*this);
    }
    os << ')';
}


void ExprPrinter::visit(MQL::ExprAggAvg& expr) {
    os << "AVG(";
    expr.expr->accept_visitor(*this);
    os << ")";
}


void ExprPrinter::visit(MQL::ExprAggCountAll& expr) {
    os << "COUNT(";
    if (expr.distinct) {
        os << "DISTINCT ";
    }
    os << "*)";
}


void ExprPrinter::visit(MQL::ExprAggCount& expr) {
    os << "COUNT(";
    if (expr.distinct) {
        os << "DISTINCT ";
    }
    expr.expr->accept_visitor(*this);
    os << ")";
}


void ExprPrinter::visit(MQL::ExprAggMax& expr) {
    os << "MAX(";
    expr.expr->accept_visitor(*this);
    os << ")";
}


void ExprPrinter::visit(MQL::ExprAggMin& expr) {
    os << "MIN(";
    expr.expr->accept_visitor(*this);
    os << ")";
}


void ExprPrinter::visit(MQL::ExprAggSum& expr) {
    os << "SUM(";
    expr.expr->accept_visitor(*this);
    os << ")";
}
