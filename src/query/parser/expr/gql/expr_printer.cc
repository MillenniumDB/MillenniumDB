#include "expr_printer.h"

#include "query/parser/expr/gql/exprs.h"

namespace GQL {

void ExprPrinter::visit(ExprNot& expr)
{
    os << "NOT(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprAnd& expr)
{
    os << "AND(";
    expr.exprs[0]->accept_visitor(*this);
    for (size_t i = 1; i < expr.exprs.size(); ++i) {
        os << ", ";
        expr.exprs[i]->accept_visitor(*this);
    }
    os << ')';
}

void ExprPrinter::visit(ExprOr& expr)
{
    os << "OR(";
    expr.exprs[0]->accept_visitor(*this);
    for (size_t i = 1; i < expr.exprs.size(); ++i) {
        os << ", ";
        expr.exprs[i]->accept_visitor(*this);
    }
    os << ')';
}

void ExprPrinter::visit(ExprHasNodeLabel& expr)
{
    os << "HasNodeLabel(";
    os << expr.node_id << ", " << expr.label_id;
    os << ')';
}

void ExprPrinter::visit(ExprHasEdgeLabel& expr)
{
    os << "HasEdgeLabel(";
    os << expr.edge_id << ", " << expr.label_id;
    os << ')';
}

void ExprPrinter::visit(ExprWildcardLabel& expr)
{
    os << "WildcardLabel(" << expr.var_id << ")";
}

void ExprPrinter::visit(ExprEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " = ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprNotEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " != ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprXor& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " XOR ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprConcat& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " || ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprGreaterOrEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " >= ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprGreater& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " > ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprLess& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " < ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprLessOrEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " < ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprIs& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " IS ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprAddition& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " + ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprSubtraction& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " - ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprMultiplication& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " * ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprDivision& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " / ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprAbs& expr)
{
    os << "ABS(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprModulo& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " % ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprSin& expr)
{
    os << "SIN(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprCos& expr)
{
    os << "COS(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprTan& expr)
{
    os << "TAN(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprCot& expr)
{
    os << "COT(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprSinh& expr)
{
    os << "SINH(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprCosh& expr)
{
    os << "COSH(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprTanh& expr)
{
    os << "TANH(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprAsin& expr)
{
    os << "ASIN(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprAcos& expr)
{
    os << "ACOS(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprAtan& expr)
{
    os << "ATAN(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprDegrees& expr)
{
    os << "DEGREES(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprRadians& expr)
{
    os << "RADIANS(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprLog& expr)
{
    os << "LOG(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprLog10& expr)
{
    os << "LOG10(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprLn& expr)
{
    os << "LN(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprExp& expr)
{
    os << "EXP(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprPower& expr)
{
    os << "POWER(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprSqrt& expr)
{
    os << "SQRT(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprFloor& expr)
{
    os << "FLOOR(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprCeil& expr)
{
    os << "CEIL(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprLength& expr)
{
    os << "CHAR_LENGTH(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprSubStr& expr)
{
    if (expr.left){
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

void ExprPrinter::visit(ExprFold& expr)
{
    if (expr.upper){
        os << "UPPER";
    } else {
        os << "LOWER";
    }
    os << "(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprSingleTrim& expr)
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

void ExprPrinter::visit(ExprMultiTrim& expr)
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

void ExprPrinter::visit(ExprNormalize& expr)
{
    os << "NORMALIZE(";
    expr.expr->accept_visitor(*this);
    os << ",";
    os << expr.form;
    os << ")";
}

void ExprPrinter::visit(ExprNullIf& expr)
{
    os << "NULLIF(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprCoalesce& expr) {
    os << "COALESCE(";
    for (const auto& expression : expr.exprs) {
        if (expression != nullptr) {
            expression->accept_visitor(*this);
        } else {
            os << "NULL";
        }
        os << ", ";
    }
    os << ')';
}

void ExprPrinter::visit(ExprSimpleCase& expr) {
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

void ExprPrinter::visit(ExprSearchedCase& expr) {
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

void ExprPrinter::visit(ExprCast& expr) {
    os << "CAST(";
    expr.expr->accept_visitor(*this);
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

void ExprPrinter::visit(ExprAggCountAll&) {
    os << "COUNT(*)";
}

void ExprPrinter::visit(ExprAggCount& expr) {
    os << "COUNT(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggAvg& expr) {
    os << "AVG(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggMax& expr) {
    os << "MAX(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';

}


void ExprPrinter::visit(ExprAggMin& expr) {
    os << "MIN(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggSum& expr) {
    os << "SUM(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggStddevPop& expr) {
    os << "STDDEV_POP(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggStddevSamp& expr) {
    os << "STDDEV_SAMP(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggCollect& expr) {
    os << "COLLECT(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggPercentileCont& expr) {
    os << "PERCENTILE_CONT(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ',';
    expr.percentile->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggPercentileDisc& expr) {
    os << "PERCENTILE_DISC(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ',';
    expr.percentile->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprVar& expr)
{
    os << expr.id;
}

void ExprPrinter::visit(ExprProperty& expr)
{
    os << expr.object << "." << expr.key;
}

void ExprPrinter::visit(ExprTerm& expr)
{
    os << expr.term;
}

} // namespace GQL
