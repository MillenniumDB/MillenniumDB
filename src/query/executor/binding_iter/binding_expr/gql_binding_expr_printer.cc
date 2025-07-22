#include "gql_binding_expr_printer.h"
#include "gql_binding_exprs.h"

namespace GQL {

void BindingExprPrinter::visit(GQL::BindingExprOr& expr)
{
    os << '(';
    auto first = true;
    for (auto& e : expr.or_list) {
        if (first) {
            first = false;
        } else {
            os << " OR ";
        }
        e->accept_visitor(*this);
    }
    os << ")";
}

void BindingExprPrinter::visit(BindingExprAnd& expr)
{
    os << '(';
    auto first = true;
    for (auto& e : expr.and_list) {
        if (first) {
            first = false;
        } else {
            os << " AND ";
        }
        e->accept_visitor(*this);
    }
    os << ")";
}

void BindingExprPrinter::visit(BindingExprNot& expr)
{
    os << "NOT(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(GQL::BindingExprHasNodeLabel& expr)
{
    os << "HasNodeLabel(";
    os << expr.node_id << ", " << expr.label_id;
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprHasEdgeLabel& expr)
{
    os << "HasEdgeLabel(";
    os << expr.edge_id << ", " << expr.label_id;
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprWildcardLabel& expr)
{
    os << "HasWildcardLabel(" << expr.var_id << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprEquals& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " = ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprConcat& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " CONCAT ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprGreaterOrEquals& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " >= ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprGreater& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " > ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprIs& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " IS ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprLessOrEquals& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " <= ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprLess& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " < ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprNotEquals& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " != ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprXor& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " XOR ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprAddition& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " + ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprSubtraction& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " - ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprMultiplication& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " * ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprDivision& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " / ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(BindingExprAbs& expr)
{
    os << "ABS(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(GQL::BindingExprModulo& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " % ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(BindingExprSin& expr)
{
    os << "SIN(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprCos& expr)
{
    os << "COS(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprTan& expr)
{
    os << "TAN(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprCot& expr)
{
    os << "COT(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprSinh& expr)
{
    os << "SINH(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprCosh& expr)
{
    os << "COSH(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprTanh& expr)
{
    os << "TANH(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprAsin& expr)
{
    os << "ASIN(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprAcos& expr)
{
    os << "ACOS(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprAtan& expr)
{
    os << "ATAN(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprDegrees& expr)
{
    os << "DEGREES(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprRadians& expr)
{
    os << "RADIANS(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(GQL::BindingExprLog& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " LOG ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(BindingExprLog10& expr)
{
    os << "LOG10(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprLn& expr)
{
    os << "LN(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprExp& expr)
{
    os << "EXP(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(GQL::BindingExprPower& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " POWER ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(BindingExprSqrt& expr)
{
    os << "SQRT(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprFloor& expr)
{
    os << "FLOOR(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprCeil& expr)
{
    os << "Ceil(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprLength& expr)
{
    os << "CHAR_LENGTH(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprSubStr& expr)
{
    if (expr.left) {
        os << "LEFT";
    } else {
        os << "RIGHT";
    }
    os << "(";
    expr.expr->accept_visitor(*this);
    os << ",";
    expr.str_len->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprFold& expr)
{
    if (expr.upper) {
        os << "UPPER";
    } else {
        os << "LOWER";
    }
    os << "(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprSingleTrim& expr)
{
    os << "TRIM(";
    os << expr.specification;
    if (expr.single_char == nullptr) {
        os << " ";
    } else {
        expr.single_char->accept_visitor(*this);
    }
    os << "FROM";
    expr.str->accept_visitor(*this);
    os << ")";
}

void BindingExprPrinter::visit(BindingExprMultiTrim& expr)
{
    os << expr.specification;
    os << "(";
    expr.trim_src->accept_visitor(*this);
    os << ",";
    if (expr.delim_str == nullptr) {
        os << " ";
    } else {
        expr.delim_str->accept_visitor(*this);
    }
    os << ")";
}

void BindingExprPrinter::visit(BindingExprNormalize& expr)
{
    os << "NORMALIZE(";
    expr.expr->accept_visitor(*this);
    os << ",";
    os << expr.form;
    os << ")";
}

void BindingExprPrinter::visit(GQL::BindingExprNullIf& expr)
{
    os << "NULLIF(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void BindingExprPrinter::visit(GQL::BindingExprCoalesce& expr)
{
    os << "COALESCE(";
    for (const auto& expression : expr.expressions) {
        expression->accept_visitor(*this);
        os << ", ";
    }
    os << ')';
}

void BindingExprPrinter::visit(BindingExprSimpleCase& expr)
{
    os << "CASE ";
    expr.case_operand->accept_visitor(*this);
    for (const auto& clauses : expr.when_clauses) {
        os << " WHEN ";
        for (const auto& clause : clauses.first.second) {
            os << clauses.first.first;
            clause->accept_visitor(*this);
            os << ", ";
        }
        os << " THEN ";
        clauses.second->accept_visitor(*this);
    }
    os << " ELSE ";
    if (expr.else_expr == nullptr) {
        os << "NULL";
    } else {
        expr.else_expr->accept_visitor(*this);
    }
}

void BindingExprPrinter::visit(BindingExprSearchedCase& expr)
{
    os << "CASE ";
    for (const auto& clause : expr.when_clauses) {
        os << " WHEN ";
        clause.first->accept_visitor(*this);
        os << " THEN ";
        clause.first->accept_visitor(*this);
    }
    os << " ELSE ";
    if (expr.else_clause == nullptr) {
        os << "NULL";
    } else {
        expr.else_clause->accept_visitor(*this);
    }
}

void BindingExprPrinter::visit(BindingExprCast& expr)
{
    os << "CAST(";
    expr.operand->accept_visitor(*this);
    os << " AS ";
    if (expr.targetType == GQL_OID::GenericType::BOOL) {
        os << "BOOL";
    } else if (expr.targetType == GQL_OID::GenericType::NUMERIC) {
        os << "NUMERIC";
    } else if (expr.targetType == GQL_OID::GenericType::DATE) {
        os << "DATE";
    } else if (expr.targetType == GQL_OID::GenericType::STRING) {
        os << "STRING";
    }
    os << ")";
}

} // namespace GQL
