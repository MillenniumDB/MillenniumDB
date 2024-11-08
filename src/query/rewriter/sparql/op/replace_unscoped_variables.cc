#include "replace_unscoped_variables.h"

#include <cassert>

#include "query/exceptions.h"
#include "graph_models/object_id.h"
#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;

void ReplaceUnscopedVariables::visit(OpSelect& op_select) {
    op_select.op->accept_visitor(*this);
    for (auto& var : op_select.vars) {
        vars_in_scope.insert(var);
    }
}


void ReplaceUnscopedVariables::visit(OpDescribe& op_describe) {
    if (op_describe.op) {
        op_describe.op->accept_visitor(*this);
    }

    std::vector<VarId> new_vars;
    new_vars.reserve(op_describe.vars.size());

    for (auto& var : op_describe.vars) {
        if (vars_in_scope.find(var) != vars_in_scope.end()) {
            // Only keep variable if it is in scope.
            // If it is not in scope then it would always be unassigned.
            new_vars.push_back(var);
        }
    }

    op_describe.vars = std::move(new_vars);
}


void ReplaceUnscopedVariables::visit(OpConstruct& op_construct) {
    op_construct.op->accept_visitor(*this);
}


void ReplaceUnscopedVariables::visit(OpAsk& op_ask) {
    op_ask.op->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpEmpty& op_empty) {
    if (op_empty.deleted_op.has_value()) {
        op_empty.deleted_op.value()->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariables::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    for (auto& var : op_basic_graph_pattern.get_scope_vars()) {
        vars_in_scope.insert(var);
    }
}

void ReplaceUnscopedVariables::visit(OpFilter& op_filter) {
    op_filter.op->accept_visitor(*this);

    ReplaceUnscopedVariablesExpr visitor(vars_in_scope);
    for (auto& filter : op_filter.filters) {
        visitor.visit_and_replace_if_necessary(filter);
    }
}


void ReplaceUnscopedVariables::visit(OpOptional& op_optional) {
    op_optional.lhs->accept_visitor(*this);
    op_optional.rhs->accept_visitor(*this);
}


void ReplaceUnscopedVariables::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpHaving& op_having) {
    op_having.op->accept_visitor(*this);
}

void ReplaceUnscopedVariables::visit(OpService& op_service) {
    for (auto& var : op_service.get_scope_vars()) {
        vars_in_scope.insert(var);
    }
}

void ReplaceUnscopedVariables::visit(OpJoin& op_join) {
    auto original_vars_in_scope = vars_in_scope;

    op_join.lhs->accept_visitor(*this);
    auto after_lhs_vars_in_scope = std::move(vars_in_scope);

    vars_in_scope = std::move(original_vars_in_scope);
    op_join.rhs->accept_visitor(*this);

    for (auto& var : after_lhs_vars_in_scope) {
        vars_in_scope.insert(var);
    }
}

void ReplaceUnscopedVariables::visit(OpSemiJoin& op_semi_join) {
    op_semi_join.lhs->accept_visitor(*this);
    auto after_lhs_vars_in_scope = vars_in_scope;

    op_semi_join.rhs->accept_visitor(*this);
    vars_in_scope = std::move(after_lhs_vars_in_scope);
}

void ReplaceUnscopedVariables::visit(OpMinus& op_minus) {
    op_minus.lhs->accept_visitor(*this);
    auto after_lhs_vars_in_scope = vars_in_scope;

    op_minus.rhs->accept_visitor(*this);
    vars_in_scope = std::move(after_lhs_vars_in_scope);
}


void ReplaceUnscopedVariables::visit(OpNotExists& op_not_exists) {
    op_not_exists.lhs->accept_visitor(*this);
    auto after_lhs_vars_in_scope = vars_in_scope;

    op_not_exists.rhs->accept_visitor(*this);
    vars_in_scope = std::move(after_lhs_vars_in_scope);
}


void ReplaceUnscopedVariables::visit(OpUnion& op_union) {
    for (auto &child : op_union.unions) {
        child->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariables::visit(OpSequence& op_sequence) {
    for (auto& op : op_sequence.ops) {
        op->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariables::visit(OpBind& op_bind) {
    op_bind.op->accept_visitor(*this);
    vars_in_scope.insert(op_bind.var);
}

void ReplaceUnscopedVariables::visit(OpUnitTable&) { }


void ReplaceUnscopedVariables::visit(OpValues& op_values) {
    for (auto& var : op_values.vars) {
        vars_in_scope.insert(var);
    }
}


// +---------------------------------------------------------------------------+
// |                            ExprVisitor                                    |
// +---------------------------------------------------------------------------+
void ReplaceUnscopedVariablesExpr::visit_and_replace_if_necessary(std::unique_ptr<Expr>& expr) {
    if (auto expr_var = dynamic_cast<ExprVar*>(expr.get())) {
        if (vars_in_scope.find(expr_var->var) == vars_in_scope.end()) {
            expr = std::make_unique<SPARQL::ExprTerm>(ObjectId::get_null());
        }
    } else if (auto expr_bound = dynamic_cast<ExprBound*>(expr.get())) {
        if (vars_in_scope.find(expr_bound->var) == vars_in_scope.end()) {
            // std::cout << "REPLACING BOUND(?" << get_query_ctx().get_var_name(expr_bound->var) << ") WITH false\n";
            expr = std::make_unique<SPARQL::ExprTerm>(ObjectId(ObjectId::BOOL_FALSE));
        }
    } else {
        expr->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprVar&) {
    // This should have been handled in visit_and_replace_if_necessary()
    assert(false);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprTerm&) { }

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprEqual& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprNotEqual& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprNot& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprUnaryMinus& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprUnaryPlus& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprMultiplication& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprDivision& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprAddition& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprSubtraction& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprAnd& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprOr& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprLess& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprGreater& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprLessOrEqual& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprGreaterOrEqual& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprIn& expr) {
    visit_and_replace_if_necessary(expr.lhs_expr);
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprNotIn& expr) {
    visit_and_replace_if_necessary(expr.lhs_expr);
    for (auto& e : expr.exprs) {
        e->accept_visitor(*this);
    }
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprExists& expr) {
    ReplaceUnscopedVariables visitor;
    expr.op->accept_visitor(visitor);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprNotExists& expr) {
    ReplaceUnscopedVariables visitor;
    expr.op->accept_visitor(visitor);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprAggAvg& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprAggCount& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprAggCountAll&) { }

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprAggGroupConcat& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprAggMax& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprAggMin& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprAggSample& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprAggSum& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprAbs& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprBNode& expr) {
    if (expr.expr) {
        visit_and_replace_if_necessary(expr.expr);
    }
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprBound&) {
    // This should have been handled in visit_and_replace_if_necessary()
    assert(false);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprCeil& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprCoalesce& expr) {
    for (auto& e : expr.exprs) {
        visit_and_replace_if_necessary(e);
    }
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprConcat& expr) {
    for (auto& e : expr.exprs) {
        visit_and_replace_if_necessary(e);
    }
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprContains& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprDatatype& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprDay& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprEncodeForUri& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprFloor& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprHours& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprIf& expr) {
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
    visit_and_replace_if_necessary(expr.expr3);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprIRI& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprIsBlank& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprIsIRI& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprIsLiteral& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprIsNumeric& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprIsURI& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprLang& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprLangMatches& expr) {
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprLCase& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprMD5& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprMinutes& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprMonth& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprNow&) { }

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprRand&) { }

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprRegex& expr) {
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
    if (expr.expr3) {
        visit_and_replace_if_necessary(expr.expr3);
    }
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprReplace& expr) {
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
    visit_and_replace_if_necessary(expr.expr3);
    if (expr.expr4) {
        visit_and_replace_if_necessary(expr.expr4);
    }
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprRound& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprSameTerm& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprSeconds& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprSHA1& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprSHA256& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprSHA384& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprSHA512& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprStrAfter& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprStrBefore& expr) {
    visit_and_replace_if_necessary(expr.lhs);
    visit_and_replace_if_necessary(expr.rhs);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprStrDT& expr) {
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprStrEnds& expr) {
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprStrLang& expr) {
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprStrLen& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprStrStarts& expr) {
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprStrUUID&) { }

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprStr& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprSubStr& expr) {
    visit_and_replace_if_necessary(expr.expr1);
    visit_and_replace_if_necessary(expr.expr2);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprTimezone& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprTZ& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprUCase& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprURI& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprUUID&) { }

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprYear& expr) {
    visit_and_replace_if_necessary(expr.expr);
}

void ReplaceUnscopedVariablesExpr::visit(SPARQL::ExprCast& expr) {
    visit_and_replace_if_necessary(expr.expr);
}
