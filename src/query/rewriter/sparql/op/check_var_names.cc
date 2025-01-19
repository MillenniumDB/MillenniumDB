#include "check_var_names.h"

#include <cassert>
#include <set>

#include "query/exceptions.h"
#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;

void CheckVarNames::handle_select(OpSelect& op_select) {
    assert(op_select.vars.size() == op_select.vars_exprs.size());

    op_select.op->accept_visitor(*this);

    for (size_t i = 0; i < op_select.vars.size(); i++) {
        auto& var  = op_select.vars[i];
        auto  expr = op_select.vars_exprs[i].get();
        if (expr) {
            if (declared_vars.find(var) != declared_vars.end()) {
                throw QuerySemanticException("Variable \"" + get_query_ctx().get_var_name(var) + "\" already declared (select)");
            }
            declared_vars.insert(var);
        } else if (group_vars.size() > 0) {
            if (group_vars.find(var) == group_vars.end()) {
                throw QuerySemanticException("Variable \"" + get_query_ctx().get_var_name(var) + "\" is not part of GROUP BY");
            }
        }
    }
}

void CheckVarNames::visit(OpSelect& op_select) {
    if (op_select.is_sub_select) {
        CheckVarNames visitor;

        visitor.handle_select(op_select);

        for (auto& var: op_select.vars) {
            declared_vars.insert(var);
        }
    } else {
        this->handle_select(op_select);
    }
}


void CheckVarNames::visit(OpDescribe& op_describe) {
    if (op_describe.op) {
        op_describe.op->accept_visitor(*this);
    }
}


void CheckVarNames::visit(OpConstruct& op_construct) {
    op_construct.op->accept_visitor(*this);
}


void CheckVarNames::visit(OpAsk& op_ask) {
    op_ask.op->accept_visitor(*this);
}


void CheckVarNames::visit(OpEmpty& op_empty) {
    if (op_empty.deleted_op.has_value()) {
        op_empty.deleted_op.value()->accept_visitor(*this);
    }
}

void CheckVarNames::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    for (auto& op_triple : op_basic_graph_pattern.triples) {
        for (auto& var : op_triple.get_all_vars()) {
            declared_vars.insert(var);
            if (declared_path_vars.find(var) != declared_path_vars.end()) {
                throw QuerySemanticException("Duplicated path variable \"" + get_query_ctx().get_var_name(var)
                                             + "\". Paths must have an unique variable");
            }
        }
    }

    for (auto& op_path : op_basic_graph_pattern.paths) {
        if (op_path.subject.is_var()) {
            auto var = op_path.subject.get_var();
            declared_vars.insert(var);
            if (declared_path_vars.find(var) != declared_path_vars.end()) {
                throw QuerySemanticException("Duplicated path variable \"" + get_query_ctx().get_var_name(var)
                                             + "\". Paths must have an unique variable");
            }
        }

        if (op_path.object.is_var()) {
            auto var = op_path.object.get_var();
            declared_vars.insert(var);
            if (declared_path_vars.find(var) != declared_path_vars.end()) {
                throw QuerySemanticException("Duplicated path variable \"" + get_query_ctx().get_var_name(var)
                                             + "\". Paths must have an unique variable");
            }
        }

        if (!declared_path_vars.insert(op_path.var).second) {
            throw QuerySemanticException("Duplicated path variable \"" + get_query_ctx().get_var_name(op_path.var)
                                         + "\". Paths must have an unique variable");
        }
        if (!declared_vars.insert(op_path.var).second) {
            throw QuerySemanticException("Duplicated path variable \"" + get_query_ctx().get_var_name(op_path.var)
                                         + "\". Paths must have an unique variable");
        }
    }

    for (auto& op_text_search_index : op_basic_graph_pattern.text_searches) {
        for (auto& var : op_text_search_index.get_all_vars()) {
            declared_vars.insert(var);
            if (declared_path_vars.find(var) != declared_path_vars.end()) {
                throw QuerySemanticException("Duplicated path variable \"" + get_query_ctx().get_var_name(var)
                                             + "\". Paths must have an unique variable");
            }
        }
    }
}


void CheckVarNames::visit(OpFilter& op_filter) {
    op_filter.op->accept_visitor(*this);
}


void CheckVarNames::visit(OpOptional& op_optional) {
    op_optional.lhs->accept_visitor(*this);
    op_optional.rhs->accept_visitor(*this);
}


void CheckVarNames::visit(OpOrderBy& op_order_by) {
    // SPARQL allows ORDER BY using non defined variables
    op_order_by.op->accept_visitor(*this);
}

void CheckVarNames::visit(OpFrom& op_from)
{
    op_from.op->accept_visitor(*this);
}

void CheckVarNames::visit(OpGraph& op_graph)
{
    op_graph.op->accept_visitor(*this);
}

void CheckVarNames::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);
    for (auto&& [expr, alias] : op_group_by.items) {
        ExprVar* casted = dynamic_cast<ExprVar*>(expr.get());
        if (casted) { // ExprVar
            group_vars.insert(casted->var);
        }
        if (alias) {
            group_vars.insert(*alias);
            if (declared_vars.find(*alias) != declared_vars.end()) {
                throw QuerySemanticException(
                    "Variable \"" + get_query_ctx().get_var_name(*alias) + "\" already declared"
                );
            }
            declared_vars.insert(*alias);
        }
    }
}

void CheckVarNames::visit(OpHaving& op_having) {
    op_having.op->accept_visitor(*this);
}


// Checks if SERVICE VAR is mentioned before.
// May fail if VAR is within another SERVICE.
void CheckVarNames::visit(OpService& op_service) {
    if (std::holds_alternative<VarId>(op_service.var_or_iri)) {
        auto service_var = std::get<VarId>(op_service.var_or_iri);
        if (declared_vars.find(service_var) == declared_vars.end()) {
            throw QuerySemanticException("Variable \"" + get_query_ctx().get_var_name(service_var) + "\" in SERVICE is not declared before");
        }
    }
    for (auto& var : op_service.get_scope_vars()) {
        declared_vars.insert(var);
    }
}


void CheckVarNames::visit(OpJoin& op_join) {
    op_join.lhs->accept_visitor(*this);
    op_join.rhs->accept_visitor(*this);
}


void CheckVarNames::visit(OpSemiJoin& op_semi_join) {
    op_semi_join.lhs->accept_visitor(*this);
    op_semi_join.rhs->accept_visitor(*this);
}


void CheckVarNames::visit(OpMinus& op_minus) {
    op_minus.lhs->accept_visitor(*this);
    op_minus.rhs->accept_visitor(*this);
}


void CheckVarNames::visit(OpNotExists& op_not_exists) {
    op_not_exists.lhs->accept_visitor(*this);
    op_not_exists.rhs->accept_visitor(*this);
}


void CheckVarNames::visit(OpUnion& op_union) {
    for (auto &child : op_union.unions) {
        child->accept_visitor(*this);
    }
}


void CheckVarNames::visit(OpSequence& op_sequence) {
    for (auto& op : op_sequence.ops) {
        op->accept_visitor(*this);
    }
}


void CheckVarNames::visit(OpBind& op_bind) {
    op_bind.op->accept_visitor(*this);

    auto& var = op_bind.var;
    auto declared_vars = op_bind.op->get_scope_vars();
    if (declared_vars.find(var) != declared_vars.end()) {
        throw QuerySemanticException("Variable \"" + get_query_ctx().get_var_name(var) + "\" already declared");
    }

    declared_vars.insert(var);
}


void CheckVarNames::visit(OpUnitTable&) { }


void CheckVarNames::visit(OpValues& op_values) {
    for (auto& var : op_values.vars) {
        declared_vars.insert(var);
    }
}

void CheckVarNames::visit(OpShow&) { }
