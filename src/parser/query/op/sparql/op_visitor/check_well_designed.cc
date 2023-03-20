#include "check_well_designed.h"

#include "parser/query/op/sparql/ops.h"

using namespace SPARQL;

void CheckWellDesigned::visit(OpOptional& op_optional) {
    auto local_vars = op_optional.op->get_vars();

    for (const auto& var : local_vars) {
        if (global_vars.find(var) != global_vars.end() && parent_vars.find(var) == parent_vars.end()) {
            throw QuerySemanticException("Query is not well defined. Var " + var.name + " is breaking the rule");
        }
        global_vars.insert(var);
    }

    parent_vars = local_vars;
    for (auto& optional_child : op_optional.optionals) {
        optional_child->accept_visitor(*this);
        parent_vars = local_vars;
    }
}

void CheckWellDesigned::visit(OpTriples& op_triples) {
    auto local_vars = op_triples.get_vars();

    for (const auto& var : local_vars) {
        if (global_vars.find(var) != global_vars.end() && parent_vars.find(var) == parent_vars.end()) {
            throw QuerySemanticException("Query is not well defined. Var " + var.name + " is breaking the rule");
        }
        global_vars.insert(var);
    }
}

void CheckWellDesigned::visit(OpSelect& op_select) {
    op_select.op->accept_visitor(*this);
}

void CheckWellDesigned::visit(OpWhere& op_where) {
    op_where.op->accept_visitor(*this);
}