#include "rewrite_filter_subqueries.h"

#include <cassert>
#include <set>

#include "query/exceptions.h"
#include "query/query_context.h"
#include "query/rewriter/sparql/op/change_join_to_sequence.h"
#include "query/rewriter/sparql/op/check_scoped_blank_nodes.h"
#include "query/rewriter/sparql/op/check_var_names.h"
#include "query/rewriter/sparql/op/replace_unscoped_variables.h"
#include "query/parser/op/sparql/ops.h"
#include "query/parser/sparql_query_parser.h"

using namespace SPARQL;

void RewriteFilterSubqueries::visit(OpSelect& op_select) {
    op_select.op->accept_visitor(*this);
}


void RewriteFilterSubqueries::visit(OpDescribe& op_describe) {
    if (op_describe.op) {
        op_describe.op->accept_visitor(*this);
    }
}


void RewriteFilterSubqueries::visit(OpConstruct& op_construct) {
    op_construct.op->accept_visitor(*this);
}


void RewriteFilterSubqueries::visit(OpAsk& op_ask) {
    op_ask.op->accept_visitor(*this);
}

void RewriteFilterSubqueries::visit(OpEmpty& op_empty) {
    if (op_empty.deleted_op.has_value()) {
        op_empty.deleted_op.value()->accept_visitor(*this);
    }
}

void RewriteFilterSubqueries::visit(OpBasicGraphPattern&) { }

void RewriteFilterSubqueries::visit(OpFilter& op_filter) {
    op_filter.op->accept_visitor(*this);

    RewriteFilterSubqueriesExpr rewriter;
    for (auto& filter: op_filter.filters) {
        filter->accept_visitor(rewriter);
    }
}


void RewriteFilterSubqueries::visit(OpOptional& op_optional) {
    op_optional.lhs->accept_visitor(*this);
    op_optional.rhs->accept_visitor(*this);
}


void RewriteFilterSubqueries::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);
}

void RewriteFilterSubqueries::visit(OpFrom& op_from)
{
    op_from.op->accept_visitor(*this);
}

void RewriteFilterSubqueries::visit(OpGraph& op_graph)
{
    op_graph.op->accept_visitor(*this);
}

void RewriteFilterSubqueries::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);
}

void RewriteFilterSubqueries::visit(OpHaving& op_having) {
    op_having.op->accept_visitor(*this);
}


void RewriteFilterSubqueries::visit(OpService&) { }

void RewriteFilterSubqueries::visit(OpJoin& op_join) {
    op_join.lhs->accept_visitor(*this);
    op_join.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueries::visit(OpSemiJoin& op_semi_join) {
    op_semi_join.lhs->accept_visitor(*this);
    op_semi_join.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueries::visit(OpMinus& op_minus) {
    op_minus.lhs->accept_visitor(*this);
    op_minus.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueries::visit(OpNotExists& op_not_exists) {
    op_not_exists.lhs->accept_visitor(*this);
    op_not_exists.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueries::visit(OpUnion& op_union) {
    for (auto& child : op_union.unions) {
        child->accept_visitor(*this);
    }
}

void RewriteFilterSubqueries::visit(OpSequence& op_sequence) {
    for (auto& op : op_sequence.ops) {
        op->accept_visitor(*this);
    }
}

void RewriteFilterSubqueries::visit(OpBind& op_bind) {
    op_bind.op->accept_visitor(*this);
}

void RewriteFilterSubqueries::visit(OpUnitTable&) { }

void RewriteFilterSubqueries::visit(OpValues&) { }

void RewriteFilterSubqueries::visit(OpShow&) { }

// +---------------------------------------------------------------------------+
// |                            ExprVisitor                                    |
// +---------------------------------------------------------------------------+

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprVar&) { }

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprTerm&) { }

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprEqual& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprNotEqual& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprNot& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprUnaryMinus& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprUnaryPlus& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprMultiplication& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprDivision& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprAddition& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprSubtraction& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprAnd& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprOr& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprLess& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprGreater& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprLessOrEqual& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprGreaterOrEqual& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprIn& expr) {
    expr.lhs_expr->accept_visitor(*this);
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprNotIn& expr) {
    expr.lhs_expr->accept_visitor(*this);
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprExists& expr) {
    expr.op = QueryParser::rewrite(std::move(expr.op));
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprNotExists& expr) {
    expr.op = QueryParser::rewrite(std::move(expr.op));
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprAggAvg& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprAggCount& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprAggCountAll&) { }

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprAggGroupConcat& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprAggMax& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprAggMin& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprAggSample& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprAggSum& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprAbs& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprBNode& expr) {
    if (expr.expr) {
        expr.expr->accept_visitor(*this);
    }
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprBound&) { }

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprCeil& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprCoalesce& expr) {
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprConcat& expr) {
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprContains& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprDatatype& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprDay& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprEncodeForUri& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprFloor& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprHours& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprIf& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
    expr.expr3->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprIRI& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprIsBlank& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprIsIRI& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprIsLiteral& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprIsNumeric& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprIsURI& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprLang& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprLangMatches& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprLCase& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprMD5& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprMinutes& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprMonth& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprNow&) { }

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprRand&) { }

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprRegex& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
    if (expr.expr3) {
        expr.expr3->accept_visitor(*this);
    }
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprReplace& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
    expr.expr3->accept_visitor(*this);
    if (expr.expr4) {
        expr.expr4->accept_visitor(*this);
    }
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprRound& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprSameTerm& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprSeconds& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprSHA1& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprSHA256& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprSHA384& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprSHA512& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprStrAfter& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprStrBefore& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprStrDT& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprStrEnds& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprStrLang& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprStrLen& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprStrStarts& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprStrUUID&) { }

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprStr& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprSubStr& expr) {
    expr.expr1->accept_visitor(*this);
    expr.expr2->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprTimezone& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprTZ& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprUCase& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprURI& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprUUID&) { }

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprYear& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprCast& expr) {
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprCosineSimilarity& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprDot& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprEuclideanDistance& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprManhattanDistance& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprPow& expr)
{
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprSqrt& expr)
{
    expr.expr->accept_visitor(*this);
}

void RewriteFilterSubqueriesExpr::visit(SPARQL::ExprSum& expr)
{
    expr.expr->accept_visitor(*this);
}