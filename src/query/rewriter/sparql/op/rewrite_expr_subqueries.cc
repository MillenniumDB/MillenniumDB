#include "rewrite_expr_subqueries.h"

#include "query/parser/op/sparql/ops.h"
#include "query/parser/sparql_query_parser.h"

using namespace SPARQL;

void RewriteExprSubqueries::visit(OpSelect& op_select)
{
    op_select.op->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpDescribe& op_describe)
{
    if (op_describe.op) {
        op_describe.op->accept_visitor(*this);
    }
}

void RewriteExprSubqueries::visit(OpConstruct& op_construct)
{
    op_construct.op->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpAsk& op_ask)
{
    op_ask.op->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpEmpty& op_empty)
{
    if (op_empty.deleted_op.has_value()) {
        op_empty.deleted_op.value()->accept_visitor(*this);
    }
}

void RewriteExprSubqueries::visit(OpFilter& op_filter)
{
    op_filter.op->accept_visitor(*this);

    RewriteExprSubqueriesExpr rewriter;
    for (auto& filter : op_filter.filters) {
        filter->accept_visitor(rewriter);
    }
}

void RewriteExprSubqueries::visit(OpOptional& op_optional)
{
    op_optional.lhs->accept_visitor(*this);
    op_optional.rhs->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpFrom& op_from)
{
    op_from.op->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpGraph& op_graph)
{
    op_graph.op->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpHaving& op_having)
{
    op_having.op->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpProcedure& op_procedure)
{
    RewriteExprSubqueriesExpr rewriter;
    for (auto& expr : op_procedure.argument_exprs) {
        expr->accept_visitor(rewriter);
    }
}

void RewriteExprSubqueries::visit(OpService&) { }

void RewriteExprSubqueries::visit(OpJoin& op_join)
{
    op_join.lhs->accept_visitor(*this);
    op_join.rhs->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpSemiJoin& op_semi_join)
{
    op_semi_join.lhs->accept_visitor(*this);
    op_semi_join.rhs->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpMinus& op_minus)
{
    op_minus.lhs->accept_visitor(*this);
    op_minus.rhs->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpNotExists& op_not_exists)
{
    op_not_exists.lhs->accept_visitor(*this);
    op_not_exists.rhs->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpUnion& op_union)
{
    for (auto& child : op_union.unions) {
        child->accept_visitor(*this);
    }
}

void RewriteExprSubqueries::visit(OpSequence& op_sequence)
{
    for (auto& op : op_sequence.ops) {
        op->accept_visitor(*this);
    }
}

void RewriteExprSubqueries::visit(OpBind& op_bind)
{
    op_bind.op->accept_visitor(*this);
}

// +---------------------------------------------------------------------------+
// |                            ExprVisitor                                    |
// +---------------------------------------------------------------------------+

void RewriteExprSubqueriesExpr::visit(ExprVar&) { }

void RewriteExprSubqueriesExpr::visit(ExprTerm&) { }

void RewriteExprSubqueriesExpr::visit(ExprEqual& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprNotEqual& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprNot& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprUnaryMinus& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprUnaryPlus& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprMultiplication& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprDivision& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprAddition& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprSubtraction& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprAnd& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprOr& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprLess& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprGreater& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprLessOrEqual& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprGreaterOrEqual& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprIn& expr)
{
    expr.lhs_expr->accept_visitor(*this);
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(ExprNotIn& expr)
{
    expr.lhs_expr->accept_visitor(*this);
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(ExprExists& expr)
{
    expr.op = QueryParser::rewrite(std::move(expr.op));
}

void RewriteExprSubqueriesExpr::visit(ExprNotExists& expr)
{
    expr.op = QueryParser::rewrite(std::move(expr.op));
}

void RewriteExprSubqueriesExpr::visit(ExprAggAvg& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprAggCount& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprAggCountAll&) { }

void RewriteExprSubqueriesExpr::visit(ExprAggGroupConcat& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprAggMax& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprAggMin& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprAggSample& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprAggSum& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprAbs& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprBNode& expr)
{
    if (expr.expr) {
        expr.expr->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(ExprBound&) { }

void RewriteExprSubqueriesExpr::visit(ExprCeil& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprCoalesce& expr)
{
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(ExprConcat& expr)
{
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(ExprContains& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprDatatype& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprDay& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprEncodeForUri& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprFloor& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprHours& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprIf& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
    expr.expr3->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprIRI& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprIsBlank& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprIsIRI& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprIsLiteral& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprIsNumeric& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprIsURI& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprLang& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprLangMatches& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprLCase& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprMD5& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprMinutes& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprMonth& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprNow&) { }

void RewriteExprSubqueriesExpr::visit(ExprRand&) { }

void RewriteExprSubqueriesExpr::visit(ExprRegex& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
    if (expr.expr3) {
        expr.expr3->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(ExprReplace& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
    expr.expr3->accept_visitor(*this);
    if (expr.expr4) {
        expr.expr4->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(ExprRound& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprSameTerm& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprSeconds& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprSHA1& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprSHA256& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprSHA384& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprSHA512& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprStrAfter& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprStrBefore& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprStrDT& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprStrEnds& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprStrLang& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprStrLen& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprStrStarts& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprStrUUID&) { }

void RewriteExprSubqueriesExpr::visit(ExprStr& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprSubStr& expr)
{
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprTimezone& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprTZ& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprUCase& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprURI& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprUUID&) { }

void RewriteExprSubqueriesExpr::visit(ExprYear& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprCast& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprCosineDistance& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprCosineSimilarity& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprDot& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprEuclideanDistance& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprManhattanDistance& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprPow& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprSqrt& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(ExprSum& expr)
{
    expr.expr->accept_visitor(*this);
}
