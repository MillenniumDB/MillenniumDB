#include "expr_printer.h"

#include "query/parser/expr/sparql/exprs.h"

namespace SPARQL {

void ExprPrinter::visit(ExprVar& expr)
{
    os << '?' << get_query_ctx().get_var_name(expr.var);
}

void ExprPrinter::visit(ExprNot& expr)
{
    os << "NOT(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprUnaryMinus& expr)
{
    os << "-(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprUnaryPlus& expr)
{
    os << "+(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprMultiplication& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " * ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprDivision& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " / ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAddition& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " + ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprSubtraction& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " - ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAnd& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " AND ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprOr& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " OR ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprLess& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " < ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprGreater& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " > ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprEqual& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " = ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprNotEqual& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " != ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprLessOrEqual& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " <= ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprGreaterOrEqual& expr)
{
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " >= ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprTerm& expr)
{
    os << expr.term;
}

void ExprPrinter::visit(ExprIn& expr)
{
    os << '(';
    expr.lhs_expr->accept_visitor(*this);
    os << " IN (";
    auto first = true;
    for (auto& e : expr.exprs) {
        if (first)
            first = false;
        else
            os << ", ";
        e->accept_visitor(*this);
    }
    os << "))";
}

void ExprPrinter::visit(ExprNotIn& expr)
{
    os << '(';
    expr.lhs_expr->accept_visitor(*this);
    os << " NOT IN (";
    auto first = true;
    for (auto& e : expr.exprs) {
        if (first)
            first = false;
        else
            os << ", ";
        e->accept_visitor(*this);
    }
    os << "))";
}

void ExprPrinter::visit(ExprAggAvg& expr)
{
    os << "AVG(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggCount& expr)
{
    os << "COUNT(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggCountAll& expr)
{
    os << "COUNT(" << (expr.distinct ? "DISTINCT " : "");
    os << "*)";
}

void ExprPrinter::visit(ExprAggGroupConcat& expr)
{
    os << "GROUP_CONCAT(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ", separator=" << expr.separator << ")";
}

void ExprPrinter::visit(ExprAggMax& expr)
{
    os << "MAX(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggMin& expr)
{
    os << "MIN(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggSample& expr)
{
    os << "SAMPLE(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAggSum& expr)
{
    os << "SUM(" << (expr.distinct ? "DISTINCT " : "");
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprAbs& expr)
{
    os << "ABS(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprBNode& expr)
{
    os << "BNODE(";
    if (expr.expr != nullptr) {
        expr.expr->accept_visitor(*this);
    }
    os << ')';
}

void ExprPrinter::visit(ExprBound& expr)
{
    os << "BOUND(?" << get_query_ctx().get_var_name(expr.var) << ')';
}

void ExprPrinter::visit(ExprCeil& expr)
{
    os << "CEIL(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprCoalesce& expr)
{
    os << "COALESCE(";
    auto first = true;
    for (auto& expr : expr.exprs) {
        if (first)
            first = false;
        else
            os << ", ";
        expr->accept_visitor(*this);
    }
    os << ')';
}

void ExprPrinter::visit(ExprConcat& expr)
{
    os << "CONCAT(";
    bool first = true;
    for (auto& expr : expr.exprs) {
        if (first)
            first = false;
        else
            os << ", ";
        expr->accept_visitor(*this);
    }
    os << ')';
}

void ExprPrinter::visit(ExprContains& expr)
{
    os << "CONTAINS(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprDatatype& expr)
{
    os << "DATATYPE(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprDay& expr)
{
    os << "DAY(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprEncodeForUri& expr)
{
    os << "ENCODE_FOR_URI(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprExists& expr)
{
    os << "EXISTS(_Op_" << ops.size() << "_)";
    ops.push_back(expr.op.get());
}

void ExprPrinter::visit(ExprFloor& expr)
{
    os << "FLOOR(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprHours& expr)
{
    os << "HOURS(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprIf& expr)
{
    os << "IF(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    os << ", ";
    expr.expr3->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprIRI& expr)
{
    os << "IRI(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprIsBlank& expr)
{
    os << "isBLANK(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprIsIRI& expr)
{
    os << "isIRI(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprIsLiteral& expr)
{
    os << "isLITERAL(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprIsNumeric& expr)
{
    os << "isNUMERIC(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprIsURI& expr)
{
    os << "isURI(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprLang& expr)
{
    os << "LANG(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprLangMatches& expr)
{
    os << "LANGMATCHES(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprLCase& expr)
{
    os << "LCASE(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprMD5& expr)
{
    os << "MD5(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprMinutes& expr)
{
    os << "MINUTES(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprMonth& expr)
{
    os << "MONTH(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprNotExists& expr)
{
    os << "NOT EXISTS(_Op_" << ops.size() << "_)";
    ops.push_back(expr.op.get());
}

void ExprPrinter::visit(ExprNow&)
{
    os << "NOW()";
}

void ExprPrinter::visit(ExprRand&)
{
    os << "RAND()";
}

void ExprPrinter::visit(ExprRegex& expr)
{
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

void ExprPrinter::visit(ExprReplace& expr)
{
    os << "REPLACE(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    os << ", ";
    expr.expr3->accept_visitor(*this);
    if (expr.expr4 != nullptr) {
        os << ", ";
        expr.expr4->accept_visitor(*this);
    }
    os << ')';
}

void ExprPrinter::visit(ExprRound& expr)
{
    os << "ROUND(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprSameTerm& expr)
{
    os << "sameTERM(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprSeconds& expr)
{
    os << "SECONDS(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprSHA1& expr)
{
    os << "SHA1(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprSHA256& expr)
{
    os << "SHA256(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprSHA384& expr)
{
    os << "SHA384(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprSHA512& expr)
{
    os << "SHA521(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprStrAfter& expr)
{
    os << "STRAFTER(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprStrBefore& expr)
{
    os << "STRBEFORE(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprStrDT& expr)
{
    os << "STRDT(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprStrEnds& expr)
{
    os << "STRENDS(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprStrLang& expr)
{
    os << "STRLANG(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprStrLen& expr)
{
    os << "STRLEN(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprStrStarts& expr)
{
    os << "STRSTARTS(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprStrUUID&)
{
    os << "STRUUID()";
}

void ExprPrinter::visit(ExprStr& expr)
{
    os << "STR(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprSubStr& expr)
{
    os << "SUBSTR(";
    expr.expr1->accept_visitor(*this);
    os << ", ";
    expr.expr2->accept_visitor(*this);
    if (expr.expr3 != nullptr) {
        os << ", ";
        expr.expr3->accept_visitor(*this);
    }
    os << ')';
}

void ExprPrinter::visit(ExprTimezone& expr)
{
    os << "TIMEZONE(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprTZ& expr)
{
    os << "TZ(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprUCase& expr)
{
    os << "UCASE(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprURI& expr)
{
    os << "URI(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprUUID&)
{
    os << "UUID()";
}

void ExprPrinter::visit(ExprYear& expr)
{
    os << "YEAR(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprCast& expr)
{
    os << cast_type_to_string(expr.cast_type) << '(';
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprCosineDistance& expr)
{
    os << "mdbfn:cosineDistance(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprCosineSimilarity& expr)
{
    os << "mdbfn:cosineSimilarity(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprDot& expr)
{
    os << "mdbfn:dot(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprEuclideanDistance& expr)
{
    os << "mdbfn:euclideanDistance(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprManhattanDistance& expr)
{
    os << "mdbfn:manhattanDistance(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprPow& expr)
{
    os << "mdbfn:pow(";
    expr.lhs->accept_visitor(*this);
    os << ", ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprSqrt& expr)
{
    os << "mdbfn:sqrt(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

void ExprPrinter::visit(ExprSum& expr)
{
    os << "mdbfn:sum(";
    expr.expr->accept_visitor(*this);
    os << ')';
}

} // namespace SPARQL
