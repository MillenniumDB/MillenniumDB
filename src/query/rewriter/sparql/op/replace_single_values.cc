#include "replace_single_values.h"

#include "query/parser/expr/sparql/atom/expr_term.h"
#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;

void ReplaceSingleValues::try_replace(Id& id)
{
    auto var = id.get_var();

    auto it = assigned_values.find(var);

    if (it != assigned_values.end()) {
        id = it->second;
    }
}

void ReplaceSingleValues::visit(OpSequence& op_sequence)
{
    auto original_map = assigned_values;

    delete_child = false;
    for (size_t i = 0; i < op_sequence.ops.size();) {
        op_sequence.ops[i]->accept_visitor(*this);
        if (delete_child) {
            op_sequence.ops.erase(op_sequence.ops.begin() + i);
            delete_child = false;
        } else {
            i++;
        }
    }

    for (auto&& [var, value] : assigned_values) {
        if (original_map.find(var) == original_map.end()) {
            auto bind = std::make_unique<OpBind>(
                std::make_unique<OpUnitTable>(),
                var,
                std::make_unique<ExprTerm>(value)
            );
            op_sequence.ops.insert(op_sequence.ops.begin(), std::move(bind));
        }
    }

    assigned_values = original_map;
}

void ReplaceSingleValues::visit(OpBind& op_bind)
{
    auto casted = dynamic_cast<ExprTerm*>(op_bind.expr.get());
    if (casted != nullptr) {
        assigned_values[op_bind.var] = casted->term;
    }

    if (dynamic_cast<OpUnitTable*>(op_bind.op.get()) != nullptr) {
        delete_child = true;
    } else {
        op_bind.op->accept_visitor(*this);
    }
}

void ReplaceSingleValues::visit(OpValues& op_values)
{
    if (op_values.vars.size() == op_values.values.size()) {
        for (size_t i = 0; i < op_values.vars.size(); i++) {
            if (!op_values.has_undef[i]) {
                assigned_values[op_values.vars[i]] = op_values.values[i];
            }
        }
        delete_child = true;
    }
}

void ReplaceSingleValues::visit(OpBasicGraphPattern& op_bgp)
{
    for (auto& triple : op_bgp.triples) {
        triple.accept_visitor(*this);
    }

    for (auto& path : op_bgp.paths) {
        path.accept_visitor(*this);
    }
}

void ReplaceSingleValues::visit(OpTriple& op_triple)
{
    if (op_triple.subject.is_var()) {
        try_replace(op_triple.subject);
    }

    if (op_triple.predicate.is_var()) {
        try_replace(op_triple.predicate);
    }

    if (op_triple.object.is_var()) {
        try_replace(op_triple.object);
    }
}

void ReplaceSingleValues::visit(OpPath& op_path)
{
    if (op_path.subject.is_var()) {
        try_replace(op_path.subject);
    }

    if (op_path.object.is_var()) {
        try_replace(op_path.object);
    }
}

void ReplaceSingleValues::visit(OpSelect& op)
{
    if (!op.is_sub_select)
        op.op->accept_visitor(*this);
    else {
        // maybe we can visit the sub select, but we only can push
        // the variables projected in the sub select
        // auto original_map = assigned_values;
        // op.op->accept_visitor(*this);
        // assigned_values = original_map;
    }
}

void ReplaceSingleValues::visit(OpOptional& op)
{
    op.lhs->accept_visitor(*this);
    // not visiting rhs, because it may be not well designed
    // op.rhs->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpSemiJoin& op)
{
    op.lhs->accept_visitor(*this);
    // op.rhs->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpMinus& op)
{
    op.lhs->accept_visitor(*this);
    // op.rhs->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpNotExists& op)
{
    op.lhs->accept_visitor(*this);
    // op.rhs->accept_visitor(*this);
}

// Other visits are trivial

void ReplaceSingleValues::visit(OpConstruct& op)
{
    if (op.op)
        op.op->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpDescribe& op)
{
    if (op.op)
        op.op->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpFilter& op)
{
    op.op->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpOrderBy& op)
{
    op.op->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpGroupBy& op)
{
    op.op->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpHaving& op)
{
    op.op->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpAsk& op)
{
    op.op->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpService& op)
{
    op.op->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpGraph& op)
{
    op.op->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpJoin& op)
{
    op.lhs->accept_visitor(*this);
    op.rhs->accept_visitor(*this);
}

void ReplaceSingleValues::visit(OpUnion& op)
{
    for (auto& child : op.unions) {
        child->accept_visitor(*this);
    }
}
