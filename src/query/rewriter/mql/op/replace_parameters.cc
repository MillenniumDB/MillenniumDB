#include "replace_parameters.h"

#include "query/exceptions.h"
#include "query/parser/expr/mql/exprs.h"
#include "query/parser/op/mql/ops.h"

using namespace MQL;

void ReplaceParameters::visit(OpBasicGraphPattern& op_basic_graph_pattern)
{
    std::set<OpLabel> new_labels;
    for (const auto& op_label : op_basic_graph_pattern.labels) {
        new_labels.emplace(var_to_parameter(op_label.node), var_to_parameter(op_label.label));
    }
    op_basic_graph_pattern.labels = std::move(new_labels);

    std::set<OpProperty> new_properties;
    for (const auto& op_property : op_basic_graph_pattern.properties) {
        new_properties.emplace(
            var_to_parameter(op_property.node),
            op_property.key,
            var_to_parameter(op_property.value)
        );
    }
    op_basic_graph_pattern.properties = std::move(new_properties);

    std::set<OpEdge> new_edges;
    for (const auto& op_edge : op_basic_graph_pattern.edges) {
        new_edges.emplace(
            var_to_parameter(op_edge.from),
            var_to_parameter(op_edge.to),
            var_to_parameter(op_edge.type),
            var_to_parameter(op_edge.edge)
        );
    }
    op_basic_graph_pattern.edges = std::move(new_edges);

    std::set<OpPath> new_paths;
    for (const auto& op_path : op_basic_graph_pattern.paths) {
        if (parameters.find(op_path.var) != parameters.end()) {
            throw QuerySemanticException(
                "Path variable ?" + get_query_ctx().get_var_name(op_path.var) + " cannot be a parameter"
            );
        }
        new_paths.emplace(
            op_path.var,
            var_to_parameter(op_path.from),
            var_to_parameter(op_path.to),
            op_path.semantic,
            op_path.direction,
            op_path.K,
            op_path.path->clone()
        );
    }
    op_basic_graph_pattern.paths = std::move(new_paths);

    // Disjoint vars may be converted to disjoint terms due to parameters
    std::set<OpDisjointVar> new_disjoint_vars;
    std::set<OpDisjointTerm> additional_disjoint_terms;
    for (const auto& op_disjoin_var : op_basic_graph_pattern.disjoint_vars) {
        const auto it = parameters.find(op_disjoin_var.var);
        if (it != parameters.end()) {
            additional_disjoint_terms.emplace(it->second);
        } else {
            new_disjoint_vars.emplace(op_disjoin_var);
        }
    }
    op_basic_graph_pattern.disjoint_vars = std::move(new_disjoint_vars);
    op_basic_graph_pattern.disjoint_terms.insert(
        additional_disjoint_terms.begin(),
        additional_disjoint_terms.end()
    );
}

void ReplaceParameters::visit(OpCall& op_call)
{
    for (const auto& var_id : op_call.yield_vars) {
        if (parameters.find(var_id) != parameters.end()) {
            throw QuerySemanticException(
                "YIELD variable ?" + get_query_ctx().get_var_name(var_id) + " cannot be a parameter"
            );
        }
    }

    ReplaceParametersExpr visitor(parameters);
    for (auto& expr : op_call.argument_exprs) {
        visitor.visit_or_replace_parameter(expr);
    }
}

void ReplaceParameters::visit(OpLet& op_let)
{
    ReplaceParametersExpr visitor(parameters);
    for (auto& [var, expr] : op_let.var_expr) {
        if (parameters.find(var) != parameters.end()) {
            throw QueryException(
                "LET alias variable ?" + get_query_ctx().get_var_name(var) + " cannot be a parameter"
            );
        }
        visitor.visit_or_replace_parameter(expr);
    }
}

void ReplaceParameters::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);

    ReplaceParametersExpr visitor(parameters);
    for (auto& expr : op_group_by.exprs) {
        visitor.visit_or_replace_parameter(expr);
    }
}

void ReplaceParameters::visit(OpOptional& op_optional)
{
    op_optional.op->accept_visitor(*this);

    for (auto& op : op_optional.optionals) {
        op->accept_visitor(*this);
    }
}

void ReplaceParameters::visit(OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);

    ReplaceParametersExpr visitor(parameters);
    for (auto& expr : op_order_by.items) {
        visitor.visit_or_replace_parameter(expr);
    }
}

void ReplaceParameters::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);

    ReplaceParametersExpr visitor(parameters);
    for (auto& [expr, var_id] : op_return.projection) {
        if (parameters.find(var_id) != parameters.end()) {
            if (auto expr_var = dynamic_cast<ExprVar*>(expr.get())) {
                if (expr_var->var != var_id) {
                    // (?x AS ?x) is the only case that we allow projecting the parameter as
                    // (param AS ?x)
                    throw QuerySemanticException(
                        "RETURN alias variable ?" + get_query_ctx().get_var_name(var_id)
                        + " cannot be a parameter"
                    );
                }
            }
        }

        visitor.visit_or_replace_parameter(expr);
    }
}

void ReplaceParameters::visit(OpSequence& op_sequence)
{
    for (auto& op : op_sequence.sequence) {
        op->accept_visitor(*this);
    }
}

void ReplaceParameters::visit(OpWhere& op_where)
{
    op_where.op->accept_visitor(*this);

    ReplaceParametersExpr visitor(parameters);
    visitor.visit_or_replace_parameter(op_where.expr);
}

void ReplaceParameters::visit(OpDisjointTerm&)
{
    // handled by OpBasicGraphPattern
    assert(false);
}
void ReplaceParameters::visit(OpDisjointVar&)
{
    // handled by OpBasicGraphPattern
    assert(false);
}
void ReplaceParameters::visit(OpEdge&)
{
    // handled by OpBasicGraphPattern
    assert(false);
}
void ReplaceParameters::visit(OpLabel&)
{
    // handled by OpBasicGraphPattern
    assert(false);
}
void ReplaceParameters::visit(OpPath&)
{
    // handled by OpBasicGraphPattern
    assert(false);
}
void ReplaceParameters::visit(OpProperty&)
{
    // handled by OpBasicGraphPattern
    assert(false);
}

Id ReplaceParameters::var_to_parameter(const Id& id)
{
    if (!id.is_var()) {
        return id;
    }

    const auto it = parameters.find(id.get_var());
    if (it != parameters.end()) {
        return it->second;
    }

    return id;
}

void ReplaceParametersExpr::visit_or_replace_parameter(std::unique_ptr<Expr>& expr)
{
    if (auto expr_var = dynamic_cast<ExprVar*>(expr.get())) {
        const auto it = parameters.find(expr_var->var);
        if (it != parameters.end()) {
            // replace the variable by its parameter
            expr = std::make_unique<ExprConstant>(it->second);
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

void ReplaceParametersExpr::visit(ExprVarProperty& expr)
{
    if (parameters.find(expr.var_without_property) != parameters.end()) {
        throw QuerySemanticException(
            "Parameter variable ?" + get_query_ctx().get_var_name(expr.var_without_property)
            + " cannot have properties"
        );
    }

    if (parameters.find(expr.var_with_property) != parameters.end()) {
        throw QuerySemanticException(
            "Property variable ?" + get_query_ctx().get_var_name(expr.var_with_property)
            + " cannot be a parameter"
        );
    }
}

void ReplaceParametersExpr::visit(ExprAddition& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprDivision& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprModulo& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprMultiplication& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprSubtraction& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprUnaryMinus& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprUnaryPlus& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprEquals& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprGreaterOrEquals& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprGreater& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprIs& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprLessOrEquals& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprLess& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprNotEquals& expr)
{
    visit_or_replace_parameter(expr.lhs);
    visit_or_replace_parameter(expr.rhs);
}

void ReplaceParametersExpr::visit(ExprAnd& expr)
{
    for (auto& e : expr.and_list) {
        visit_or_replace_parameter(e);
    }
}

void ReplaceParametersExpr::visit(ExprNot& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprOr& expr)
{
    for (auto& e : expr.or_list) {
        visit_or_replace_parameter(e);
    }
}

void ReplaceParametersExpr::visit(ExprRegex& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
    if (expr.expr3) {
        visit_or_replace_parameter(expr.expr3);
    }
}

void ReplaceParametersExpr::visit(ExprCosineSimilarity& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
}

void ReplaceParametersExpr::visit(ExprCosineDistance& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
}

void ReplaceParametersExpr::visit(ExprManhattanDistance& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
}

void ReplaceParametersExpr::visit(ExprEuclideanDistance& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
}

void ReplaceParametersExpr::visit(ExprEditDistance& expr)
{
    visit_or_replace_parameter(expr.expr1);
    visit_or_replace_parameter(expr.expr2);
}

void ReplaceParametersExpr::visit(ExprNormalize& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprStr& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprLabels&)
{
    // TODO: expr.var;
}

void ReplaceParametersExpr::visit(ExprType&)
{
    // TODO: expr.var;
}

void ReplaceParametersExpr::visit(ExprProperties&)
{
    // TODO: expr.var;
}

void ReplaceParametersExpr::visit(ExprAggAvg& expr)
{
    visit_or_replace_parameter(expr.expr);
}

void ReplaceParametersExpr::visit(ExprAggCountAll&) { }

void ReplaceParametersExpr::visit(ExprAggCount& expr)
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

void ReplaceParametersExpr::visit(ExprAggSum& expr)
{
    visit_or_replace_parameter(expr.expr);
}