#include "replace_parameters.h"

#include "query/exceptions.h"
#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;

void ReplaceParameters::visit(OpConstruct& op_construct)
{
    op_construct.op->accept_visitor(*this);

    for (auto& op_triple : op_construct.triples) {
        replace_var_if_posible(op_triple.subject);
        replace_var_if_posible(op_triple.predicate);
        replace_var_if_posible(op_triple.object);
    }
}

void ReplaceParameters::visit(OpDescribe& op_describe)

{
    op_describe.op->accept_visitor(*this);

    for (const auto& var : op_describe.vars) {
        if (parameters.find(var) != parameters.end()) {
            throw QuerySemanticException(
                "DESCRIBE variable ?" + get_query_ctx().get_var_name(var) + " cannot be a parameter"
            );
        }
    }
}

void ReplaceParameters::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    for (auto& op_triple : op_basic_graph_pattern.triples) {
        replace_var_if_posible(op_triple.subject);
        replace_var_if_posible(op_triple.predicate);
        replace_var_if_posible(op_triple.object);
    }

    for (auto& op_path : op_basic_graph_pattern.paths) {
        if (parameters.find(op_path.var) != parameters.end()) {
            throw QuerySemanticException(
                "Path variable ?" + get_query_ctx().get_var_name(op_path.var) + " cannot be a parameter"
            );
        }
        replace_var_if_posible(op_path.subject);
        replace_var_if_posible(op_path.object);
    }
}

void ReplaceParameters::visit(OpProcedure& op_procedure)
{
    for (const auto& var : op_procedure.binding_vars) {
        if (parameters.find(var) != parameters.end()) {
            throw QuerySemanticException(
                "Procedure yield variable ?" + get_query_ctx().get_var_name(var) + " cannot be a parameter"
            );
        }
    }

    ReplaceParametersExpr visitor(parameters);
    for (auto& expr : op_procedure.argument_exprs) {
        visitor.visit_or_replace_parameter(expr);
    }
}

void ReplaceParameters::visit(OpEmpty& op_empty)
{
    if (op_empty.deleted_op.has_value()) {
        op_empty.deleted_op.value()->accept_visitor(*this);
    }
}

void ReplaceParameters::visit(OpFilter& op_filter)
{
    op_filter.op->accept_visitor(*this);

    ReplaceParametersExpr visitor(parameters);
    for (auto& expr : op_filter.filters) {
        visitor.visit_or_replace_parameter(expr);
    }
}

void ReplaceParameters::visit(OpJoin& op_join)
{
    op_join.lhs->accept_visitor(*this);
    op_join.rhs->accept_visitor(*this);
}

void ReplaceParameters::visit(OpSemiJoin& op_semi_join)
{
    op_semi_join.lhs->accept_visitor(*this);
    op_semi_join.rhs->accept_visitor(*this);
}

void ReplaceParameters::visit(OpMinus& op_minus)
{
    op_minus.lhs->accept_visitor(*this);
    op_minus.rhs->accept_visitor(*this);
}

void ReplaceParameters::visit(OpNotExists& op_not_exists)
{
    op_not_exists.lhs->accept_visitor(*this);
    op_not_exists.rhs->accept_visitor(*this);
}

void ReplaceParameters::visit(OpUnion& op_union)
{
    for (auto& op : op_union.unions) {
        op->accept_visitor(*this);
    }
}

void ReplaceParameters::visit(OpOptional& op_optional)
{
    op_optional.lhs->accept_visitor(*this);
    op_optional.rhs->accept_visitor(*this);
}

void ReplaceParameters::visit(OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);

    ReplaceParametersExpr visitor(parameters);
    for (auto& item : op_order_by.items) {
        if (std::holds_alternative<VarId>(item)) {
            const auto var_id = std::get<VarId>(item);
            if (parameters.find(var_id) != parameters.end()) {
                throw QuerySemanticException(
                    "ORDER BY variable alias ?" + get_query_ctx().get_var_name(var_id)
                    + " cannot be a parameter"
                );
            }
        } else {
            auto& expr = std::get<std::unique_ptr<Expr>>(item);
            visitor.visit_or_replace_parameter(expr);
        }
    }
}

void ReplaceParameters::visit(OpFrom& op_from)
{
    op_from.op->accept_visitor(*this);
}

void ReplaceParameters::visit(OpGraph& op_graph)
{
    if (std::holds_alternative<VarId>(op_graph.graph)) {
        const auto var_id = std::get<VarId>(op_graph.graph);
        if (parameters.find(var_id) != parameters.end()) {
            throw QuerySemanticException(
                "GRAPH variable ?" + get_query_ctx().get_var_name(var_id) + " cannot be a parameter"
            );
        }
    }

    op_graph.op->accept_visitor(*this);
}

void ReplaceParameters::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);

    ReplaceParametersExpr visitor(parameters);
    for (auto& [expr, alias] : op_group_by.items) {
        if (alias) {
            if (parameters.find(*alias) != parameters.end()) {
                throw QuerySemanticException(
                    "GROUP BY variable alias ?" + get_query_ctx().get_var_name(*alias)
                    + " cannot be a parameter"
                );
            }
        }
        visitor.visit_or_replace_parameter(expr);
    }
}

void ReplaceParameters::visit(OpHaving& op_having)
{
    op_having.op->accept_visitor(*this);

    ReplaceParametersExpr visitor(parameters);
    for (auto& expr : op_having.exprs) {
        visitor.visit_or_replace_parameter(expr);
    }
}

void ReplaceParameters::visit(OpSelect& op_select)
{
    op_select.op->accept_visitor(*this);

    ReplaceParametersExpr visitor(parameters);
    for (size_t i = 0; i < op_select.vars.size(); i++) {
        const auto& var_id = op_select.vars[i];
        auto& expr = op_select.vars_exprs[i];
        if (expr) {
            // var is an alias
            if (parameters.find(var_id) != parameters.end()) {
                throw QuerySemanticException(
                    "SELECT alias variable ?" + get_query_ctx().get_var_name(var_id)
                    + " cannot be a parameter"
                );
            }
            visitor.visit_or_replace_parameter(expr);
        } else {
            // var is a projection
            const auto it = parameters.find(var_id);
            if (it != parameters.end()) {
                expr = std::make_unique<ExprTerm>(it->second);
            }
        }
    }
}

void ReplaceParameters::visit(OpSequence& op_sequence)
{
    for (auto& op : op_sequence.ops) {
        op->accept_visitor(*this);
    }
}

void ReplaceParameters::visit(OpAsk& op_ask)
{
    op_ask.op->accept_visitor(*this);
}

void ReplaceParameters::visit(OpService& op_service)
{
    if (std::holds_alternative<VarId>(op_service.var_or_iri)) {
        const auto var_id = std::get<VarId>(op_service.var_or_iri);
        if (parameters.find(var_id) != parameters.end()) {
            throw QueryException(
                "SERVICE variable ?" + get_query_ctx().get_var_name(var_id) + " cannot be a parameter"
            );
        }
    }

    op_service.op->accept_visitor(*this);
}

void ReplaceParameters::visit(OpBind& op_bind)
{
    op_bind.op->accept_visitor(*this);

    if (parameters.find(op_bind.var) != parameters.end()) {
        throw QueryException(
            "BIND alias variable ?" + get_query_ctx().get_var_name(op_bind.var) + " cannot be a parameter"
        );
    }

    ReplaceParametersExpr visitor(parameters);
    visitor.visit_or_replace_parameter(op_bind.expr);
}

void ReplaceParameters::visit(OpShow&) { }

void ReplaceParameters::visit(OpUpdate& op_update)
{
    for (auto& op : op_update.updates) {
        op->accept_visitor(*this);
    }
}

void ReplaceParameters::visit(OpInsertData& op_insert_data)
{
    for (auto& op_triple : op_insert_data.triples) {
        replace_var_if_posible(op_triple.subject);
        replace_var_if_posible(op_triple.predicate);
        replace_var_if_posible(op_triple.object);
    }
}

void ReplaceParameters::visit(OpDeleteData& op_delete_data)
{
    for (auto& op_triple : op_delete_data.triples) {
        replace_var_if_posible(op_triple.subject);
        replace_var_if_posible(op_triple.predicate);
        replace_var_if_posible(op_triple.object);
    }
}

void ReplaceParameters::visit(OpValues& op_values)
{
    for (const auto& var : op_values.vars) {
        if (parameters.find(var) != parameters.end()) {
            throw QueryException(
                "VALUES variable ?" + get_query_ctx().get_var_name(var) + " cannot be a parameter"
            );
        }
    }
}

void ReplaceParameters::replace_var_if_posible(Id& id) const
{
    if (!id.is_var()) {
        return;
    }

    const auto it = parameters.find(id.get_var());
    if (it != parameters.end()) {
        id = it->second;
    }
}

void ReplaceParametersExpr::visit_or_replace_parameter(std::unique_ptr<Expr>& expr)
{
    if (auto expr_var = dynamic_cast<ExprVar*>(expr.get())) {
        const auto it = parameters.find(expr_var->var);
        if (it != parameters.end()) {
            // replace the variable by its parameter
            expr = std::make_unique<ExprTerm>(it->second);
        }
    } else if (auto expr_bound = dynamic_cast<ExprBound*>(expr.get())) {
        const auto it = parameters.find(expr_bound->var);
        if (it != parameters.end()) {
            // the variable is bounded by a parameter
            expr = std::make_unique<ExprTerm>(ObjectId(ObjectId::BOOL_TRUE));
        }
    } else {
        expr->accept_visitor(*this);
    }
}

void ReplaceParametersExpr::visit(ExprVar&)
{
    // should have been handled in visit_or_replace_parameter()
    assert(false);
}

void ReplaceParametersExpr::visit(ExprTerm&) { }

void ReplaceParametersExpr::visit(ExprEqual& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprNotEqual& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprNot& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprUnaryMinus& expr)
{
    visit_or_replace_parameter(expr.expr);
}
void ReplaceParametersExpr::visit(ExprUnaryPlus& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprMultiplication& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprDivision& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprAddition& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprSubtraction& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprAnd& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprOr& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprLess& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprGreater& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprLessOrEqual& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprGreaterOrEqual& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprIn& expr)
{
    visit_or_replace_parameter(expr.lhs_expr);
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void ReplaceParametersExpr::visit(ExprNotIn& expr)
{
    visit_or_replace_parameter(expr.lhs_expr);
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void ReplaceParametersExpr::visit(ExprExists& expr)
{
    ReplaceParameters visitor(parameters);
    expr.op->accept_visitor(visitor);
}
void ReplaceParametersExpr::visit(ExprNotExists& expr)
{
    ReplaceParameters visitor(parameters);
    expr.op->accept_visitor(visitor);
}

void ReplaceParametersExpr::visit(ExprAggAvg& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprAggCount& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprAggCountAll&) { }

void ReplaceParametersExpr::visit(ExprAggGroupConcat& expr)
{
    visit_or_replace_parameter(expr.expr);
}
void ReplaceParametersExpr::visit(ExprAggMax& expr)
{
    visit_or_replace_parameter(expr.expr);
}
void ReplaceParametersExpr::visit(ExprAggMin& expr)
{
    visit_or_replace_parameter(expr.expr);
}
void ReplaceParametersExpr::visit(ExprAggSample& expr)
{
    visit_or_replace_parameter(expr.expr);
}
void ReplaceParametersExpr::visit(ExprAggSum& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprAbs& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprBNode& expr)
{
    if (expr.expr) {
        visit_or_replace_parameter(expr.expr);
    }
}

void ReplaceParametersExpr::visit(ExprBound&)
{
    // should have been handeld in visit_or_replace_parameater()
    assert(false);
}

void ReplaceParametersExpr::visit(ExprCeil& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprCoalesce& expr)
{
    for (auto& e : expr.exprs) {
        visit_or_replace_parameter(e);
    }
}

void ReplaceParametersExpr::visit(ExprConcat& expr)
{
    for (auto& e : expr.exprs) {
        visit_or_replace_parameter(e);
    }
}

void ReplaceParametersExpr::visit(ExprContains& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprDatatype& expr)
{
    visit_or_replace_parameter(expr.expr);
}
void ReplaceParametersExpr::visit(ExprDay& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprEncodeForUri& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprFloor& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprHours& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprIf& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
    visit_or_replace_parameter(expr.expr3);
}

void ReplaceParametersExpr::visit(ExprIRI& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprIsBlank& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprIsIRI& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprIsLiteral& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprIsNumeric& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprIsURI& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprLang& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprLangMatches& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
}

void ReplaceParametersExpr::visit(ExprLCase& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprMD5& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprMinutes& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprMonth& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprNow&) { }

void ReplaceParametersExpr::visit(ExprRand&) { }

void ReplaceParametersExpr::visit(ExprRegex& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
    if (expr.expr3) {
        visit_or_replace_parameter(expr.expr3);
    }
}

void ReplaceParametersExpr::visit(ExprReplace& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
    visit_or_replace_parameter(expr.expr3);
    if (expr.expr4) {
        visit_or_replace_parameter(expr.expr4);
    }
}

void ReplaceParametersExpr::visit(ExprRound& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprSameTerm& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprSeconds& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprSHA1& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprSHA256& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprSHA384& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprSHA512& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprStrAfter& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}
void ReplaceParametersExpr::visit(ExprStrBefore& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}
void ReplaceParametersExpr::visit(ExprStrDT& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
}

void ReplaceParametersExpr::visit(ExprStrEnds& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
}

void ReplaceParametersExpr::visit(ExprStrLang& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
}

void ReplaceParametersExpr::visit(ExprStrLen& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprStrStarts& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
}

void ReplaceParametersExpr::visit(ExprStrUUID&) { }

void ReplaceParametersExpr::visit(ExprStr& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprSubStr& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
}

void ReplaceParametersExpr::visit(ExprTimezone& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprTZ& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprUCase& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprURI& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprUUID&) { }

void ReplaceParametersExpr::visit(ExprYear& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprCast& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprCosineDistance& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprCosineSimilarity& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprDot& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprEuclideanDistance& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprManhattanDistance& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprPow& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprSqrt& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprSum& expr)
{
    visit_or_replace_parameter(expr.expr);
}
