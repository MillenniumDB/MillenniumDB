#include "rewrite_expr_subqueries.h"

#include "query/parser/op/sparql/ops.h"
#include "query/parser/sparql_query_parser.h"

using namespace SPARQL;

void RewriteExprSubqueries::visit(OpSelect& op_select) {
    op_select.op->accept_visitor(*this);
}


void RewriteExprSubqueries::visit(OpDescribe& op_describe) {
    if (op_describe.op) {
        op_describe.op->accept_visitor(*this);
    }
}


void RewriteExprSubqueries::visit(OpConstruct& op_construct) {
    op_construct.op->accept_visitor(*this);
}


void RewriteExprSubqueries::visit(OpAsk& op_ask) {
    op_ask.op->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpEmpty& op_empty) {
    if (op_empty.deleted_op.has_value()) {
        op_empty.deleted_op.value()->accept_visitor(*this);
    }
}

void RewriteExprSubqueries::visit(OpBasicGraphPattern&) { }

void RewriteExprSubqueries::visit(OpFilter& op_filter) {
    op_filter.op->accept_visitor(*this);

    RewriteExprSubqueriesExpr rewriter;
    for (auto& filter: op_filter.filters) {
        filter->accept_visitor(rewriter);
    }
}


void RewriteExprSubqueries::visit(OpOptional& op_optional) {
    op_optional.lhs->accept_visitor(*this);
    op_optional.rhs->accept_visitor(*this);
}


void RewriteExprSubqueries::visit(OpOrderBy& op_order_by) {
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

void RewriteExprSubqueries::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpHaving& op_having) {
    op_having.op->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpProcedure& op_procedure) {
    RewriteExprSubqueriesExpr rewriter;
    for (auto& expr : op_procedure.argument_exprs) {
        expr->accept_visitor(rewriter);
    }
}

void RewriteExprSubqueries::visit(OpService&) { }

void RewriteExprSubqueries::visit(OpJoin& op_join) {
    op_join.lhs->accept_visitor(*this);
    op_join.rhs->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpSemiJoin& op_semi_join) {
    op_semi_join.lhs->accept_visitor(*this);
    op_semi_join.rhs->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpMinus& op_minus) {
    op_minus.lhs->accept_visitor(*this);
    op_minus.rhs->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpNotExists& op_not_exists) {
    op_not_exists.lhs->accept_visitor(*this);
    op_not_exists.rhs->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpUnion& op_union) {
    for (auto& child : op_union.unions) {
        child->accept_visitor(*this);
    }
}

void RewriteExprSubqueries::visit(OpSequence& op_sequence) {
    for (auto& op : op_sequence.ops) {
        op->accept_visitor(*this);
    }
}

void RewriteExprSubqueries::visit(OpBind& op_bind) {
    op_bind.op->accept_visitor(*this);
}

void RewriteExprSubqueries::visit(OpUnitTable&) { }

void RewriteExprSubqueries::visit(OpValues&) { }

void RewriteExprSubqueries::visit(OpShow&) { }

// +---------------------------------------------------------------------------+
// |                            ExprVisitor                                    |
// +---------------------------------------------------------------------------+

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprVar&) { }

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprTerm&) { }

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprEqual& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprNotEqual& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprNot& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprUnaryMinus& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprUnaryPlus& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprMultiplication& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprDivision& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprAddition& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprSubtraction& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprAnd& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprOr& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprLess& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprGreater& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprLessOrEqual& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprGreaterOrEqual& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprIn& expr) {
    expr.lhs_expr->accept_visitor(*this);
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprNotIn& expr) {
    expr.lhs_expr->accept_visitor(*this);
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprExists& expr) {
    expr.op = QueryParser::rewrite(std::move(expr.op));
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprNotExists& expr) {
    expr.op = QueryParser::rewrite(std::move(expr.op));
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprAggAvg& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprAggCount& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprAggCountAll&) { }

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprAggGroupConcat& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprAggMax& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprAggMin& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprAggSample& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprAggSum& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprAbs& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprBNode& expr) {
    if (expr.expr) {
        expr.expr->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprBound&) { }

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprCeil& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprCoalesce& expr) {
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprConcat& expr) {
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprContains& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprDatatype& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprDay& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprEncodeForUri& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprFloor& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprHours& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprIf& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
    expr.expr3->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprIRI& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprIsBlank& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprIsIRI& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprIsLiteral& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprIsNumeric& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprIsURI& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprLang& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprLangMatches& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprLCase& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprMD5& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprMinutes& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprMonth& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprNow&) { }

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprRand&) { }

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprRegex& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
    if (expr.expr3) {
        expr.expr3->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprReplace& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
    expr.expr3->accept_visitor(*this);
    if (expr.expr4) {
        expr.expr4->accept_visitor(*this);
    }
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprRound& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprSameTerm& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprSeconds& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprSHA1& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprSHA256& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprSHA384& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprSHA512& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprStrAfter& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprStrBefore& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprStrDT& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprStrEnds& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprStrLang& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprStrLen& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprStrStarts& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprStrUUID&) { }

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprStr& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprSubStr& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprTimezone& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprTZ& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprUCase& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprURI& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprUUID&) { }

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprYear& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprCast& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprCosineDistance& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprCosineSimilarity& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprDot& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprEuclideanDistance& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprManhattanDistance& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprPow& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprSqrt& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteExprSubqueriesExpr::visit(SPARQL::ExprSum& expr)
{
    expr.expr->accept_visitor(*this);
}
