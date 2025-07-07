#include "replace_unscoped_variables.h"

#include <cassert>

#include "graph_models/object_id.h"
#include "misc/set_operations.h"
#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;
using namespace misc;

void ReplaceUnscopedVariables::visit(OpDescribe& op_describe)
{
    if (op_describe.op) {
        op_describe.op->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariables::visit(OpConstruct& op_construct)
{
    op_construct.op->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpAsk& op_ask)
{
    op_ask.op->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpEmpty& op_empty)
{
    if (op_empty.deleted_op.has_value()) {
        op_empty.deleted_op.value()->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariables::visit(OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpFrom& op_from)
{
    op_from.op->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpGraph& op_graph)
{
    op_graph.op->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpHaving& op_having)
{
    op_having.op->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpFilter& op_filter)
{
    op_filter.op->accept_visitor(*this);

    auto scope_vars = set_union(op_filter.op->get_scope_vars(), parent_vars_in_scope);
    ReplaceUnscopedVariablesExpr visitor(scope_vars);
    for (auto& filter : op_filter.filters) {
        visitor.visit_and_replace_if_necessary(filter);
    }
}

void ReplaceUnscopedVariables::visit(OpSelect& op_select)
{
    auto original_parent_vars_in_scope = parent_vars_in_scope;
    parent_vars_in_scope = {};
    op_select.op->accept_visitor(*this);
    parent_vars_in_scope = original_parent_vars_in_scope;
}

void ReplaceUnscopedVariables::visit(OpJoin& op_join)
{
    op_join.lhs->accept_visitor(*this);
    op_join.rhs->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpOptional& op_optional)
{
    auto original_parent_vars_in_scope = parent_vars_in_scope;
    op_optional.lhs->accept_visitor(*this);

    parent_vars_in_scope = set_union(parent_vars_in_scope, op_optional.lhs->get_scope_vars());
    op_optional.rhs->accept_visitor(*this);

    parent_vars_in_scope = original_parent_vars_in_scope;
}

void ReplaceUnscopedVariables::visit(OpSemiJoin& op_semi_join)
{
    auto original_parent_vars_in_scope = parent_vars_in_scope;
    op_semi_join.lhs->accept_visitor(*this);

    parent_vars_in_scope = set_union(parent_vars_in_scope, op_semi_join.lhs->get_scope_vars());
    op_semi_join.rhs->accept_visitor(*this);
    parent_vars_in_scope = original_parent_vars_in_scope;
}

void ReplaceUnscopedVariables::visit(OpMinus& op_minus)
{
    op_minus.lhs->accept_visitor(*this);
    op_minus.rhs->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpNotExists& op_not_exists)
{
    auto original_parent_vars_in_scope = parent_vars_in_scope;
    op_not_exists.lhs->accept_visitor(*this);

    parent_vars_in_scope = set_union(parent_vars_in_scope, op_not_exists.lhs->get_scope_vars());
    op_not_exists.rhs->accept_visitor(*this);
    parent_vars_in_scope = original_parent_vars_in_scope;
}

void ReplaceUnscopedVariables::visit(OpUnion& op_union)
{
    for (auto& child : op_union.unions) {
        child->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariables::visit(OpSequence& op_sequence)
{
    for (auto& op : op_sequence.ops) {
        op->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariables::visit(OpBind& op_bind)
{
    op_bind.op->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpBasicGraphPattern&) { }

void ReplaceUnscopedVariables::visit(OpProcedure&) { }

void ReplaceUnscopedVariables::visit(OpUnitTable&) { }

void ReplaceUnscopedVariables::visit(OpValues&) { }

void ReplaceUnscopedVariables::visit(OpShow&) { }

void ReplaceUnscopedVariables::visit(OpService&) { }

// +---------------------------------------------------------------------------+
// |                            ExprVisitor                                    |
// +---------------------------------------------------------------------------+
void ReplaceUnscopedVariablesExpr::visit_and_replace_if_necessary(std::unique_ptr<Expr>& expr)
{
    if (auto expr_var = dynamic_cast<ExprVar*>(expr.get())) {
        if (vars_in_scope.find(expr_var->var) == vars_in_scope.end()) {
            expr = std::make_unique<ExprTerm>(ObjectId::get_null());
        }
    } else if (auto expr_bound = dynamic_cast<ExprBound*>(expr.get())) {
        if (vars_in_scope.find(expr_bound->var) == vars_in_scope.end()) {
            // std::cout << "REPLACING BOUND(?" << get_query_ctx().get_var_name(expr_bound->var) << ") WITH false\n";
            expr = std::make_unique<ExprTerm>(ObjectId(ObjectId::BOOL_FALSE));
        }
    } else {
        expr->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariablesExpr::visit(ExprVar&)
{
    // This should have been handled in visit_and_replace_if_necessary()
    assert(false);
}

void ReplaceUnscopedVariablesExpr::visit(ExprTerm&) { }

void ReplaceUnscopedVariablesExpr::visit(ExprEqual& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprNotEqual& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprNot& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprUnaryMinus& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprUnaryPlus& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprMultiplication& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprDivision& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprAddition& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprSubtraction& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprAnd& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprOr& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprLess& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprGreater& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprLessOrEqual& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprGreaterOrEqual& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprIn& expr)
{
    visit_and_replace_if_necessary(expr.lhs_expr);
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariablesExpr::visit(ExprNotIn& expr)
{
    visit_and_replace_if_necessary(expr.lhs_expr);
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariablesExpr::visit(ExprExists& expr)
{
    ReplaceUnscopedVariables visitor;
    visitor.parent_vars_in_scope = this->vars_in_scope;
    expr.op->accept_visitor(visitor);
}

void ReplaceUnscopedVariablesExpr::visit(ExprNotExists& expr)
{
    ReplaceUnscopedVariables visitor;
    visitor.parent_vars_in_scope = this->vars_in_scope;
    expr.op->accept_visitor(visitor);
}

void ReplaceUnscopedVariablesExpr::visit(ExprAggAvg& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprAggCount& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprAggCountAll&) { }

void ReplaceUnscopedVariablesExpr::visit(ExprAggGroupConcat& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprAggMax& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprAggMin& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprAggSample& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprAggSum& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprAbs& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprBNode& expr)
{
    if (expr.expr) {
        visit_and_replace_if_necessary(expr.expr);
    }
}

void ReplaceUnscopedVariablesExpr::visit(ExprBound&)
{
    // This should have been handled in visit_and_replace_if_necessary()
    assert(false);
}

void ReplaceUnscopedVariablesExpr::visit(ExprCeil& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprCoalesce& expr)
{
    for (auto& e : expr.exprs) {
        visit_and_replace_if_necessary(e);
    }
}

void ReplaceUnscopedVariablesExpr::visit(ExprConcat& expr)
{
    for (auto& e : expr.exprs) {
        visit_and_replace_if_necessary(e);
    }
}

void ReplaceUnscopedVariablesExpr::visit(ExprContains& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprDatatype& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprDay& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprEncodeForUri& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprFloor& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprHours& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprIf& expr)
{
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
    visit_and_replace_if_necessary(expr.expr3);
}

void ReplaceUnscopedVariablesExpr::visit(ExprIRI& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprIsBlank& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprIsIRI& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprIsLiteral& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprIsNumeric& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprIsURI& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprLang& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprLangMatches& expr)
{
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
}

void ReplaceUnscopedVariablesExpr::visit(ExprLCase& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprMD5& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprMinutes& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprMonth& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprNow&) { }

void ReplaceUnscopedVariablesExpr::visit(ExprRand&) { }

void ReplaceUnscopedVariablesExpr::visit(ExprRegex& expr)
{
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
    if (expr.expr3) {
        visit_and_replace_if_necessary(expr.expr3);
    }
}

void ReplaceUnscopedVariablesExpr::visit(ExprReplace& expr)
{
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
    visit_and_replace_if_necessary(expr.expr3);
    if (expr.expr4) {
        visit_and_replace_if_necessary(expr.expr4);
    }
}

void ReplaceUnscopedVariablesExpr::visit(ExprRound& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprSameTerm& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprSeconds& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprSHA1& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprSHA256& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprSHA384& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprSHA512& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprStrAfter& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprStrBefore& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprStrDT& expr)
{
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
}

void ReplaceUnscopedVariablesExpr::visit(ExprStrEnds& expr)
{
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
}

void ReplaceUnscopedVariablesExpr::visit(ExprStrLang& expr)
{
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
}

void ReplaceUnscopedVariablesExpr::visit(ExprStrLen& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprStrStarts& expr)
{
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
}

void ReplaceUnscopedVariablesExpr::visit(ExprStrUUID&) { }

void ReplaceUnscopedVariablesExpr::visit(ExprStr& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprSubStr& expr)
{
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
}

void ReplaceUnscopedVariablesExpr::visit(ExprTimezone& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprTZ& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprUCase& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprURI& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprUUID&) { }

void ReplaceUnscopedVariablesExpr::visit(ExprYear& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprCast& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprCosineDistance& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprCosineSimilarity& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprDot& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprEuclideanDistance& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}
void ReplaceUnscopedVariablesExpr::visit(ExprManhattanDistance& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprPow& expr)
{
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(ExprSqrt& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(ExprSum& expr)
{
    visit_and_replace_if_necessary(expr.expr);
}
