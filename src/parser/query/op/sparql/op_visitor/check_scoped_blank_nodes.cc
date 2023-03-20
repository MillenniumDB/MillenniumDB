#include "check_scoped_blank_nodes.h"

#include "base/exceptions.h"
#include "parser/query/op/sparql/ops.h"

using namespace SPARQL;

void CheckScopedBlankNodes::visit(OpOptional& op_optional) {
    // Visit op_triples
    op_optional.op->accept_visitor(*this);
    // Check for the property for every optional child
    for (auto& optional_child : op_optional.optionals) {
        optional_child->accept_visitor(*this);
    }
}

void CheckScopedBlankNodes::visit(OpTriples& op_triples) {
    // It is important to notice that we treat blank nodes as "unprojectable variables" 
    // and we identify them by their name (they start with "_:")
    std::set<Var> scope_bnodes;
    for (auto& triple : op_triples.triples) {
        if (triple.subject.is_bnode()) {
            scope_bnodes.insert(triple.subject.to_var());
        }
        if (triple.object.is_bnode()) {
            scope_bnodes.insert(triple.object.to_var());
        }
    }
    // Check if any blank node was mentioned before in another scope
    for (auto& bnode : scope_bnodes) {
        if (mentioned_bnodes.find(bnode) != mentioned_bnodes.end()) {
            throw QuerySemanticException("Query is not well defined. Blank node " + bnode.name + " is breaking the rule");
        }
    }
    // Store current scope blank nodes into mentioned blank nodes
    mentioned_bnodes.insert(scope_bnodes.begin(), scope_bnodes.end());
}

void CheckScopedBlankNodes::visit(OpSelect& op_select) {
    op_select.op->accept_visitor(*this);
}

void CheckScopedBlankNodes::visit(OpWhere& op_where) {
    op_where.op->accept_visitor(*this);
}