
#include "check_unbounded_repetition.h"

using namespace GQL;

void CheckUnboundedRepetition::visit(OpGraphPattern& op)
{
    op.op->accept_visitor(*this);
    check_mode(op.mode);
}

void CheckUnboundedRepetition::check_mode(const PathMode& mode)
{
    if (has_unbounded_repetition && mode.selector == PathMode::Selector::ALL
        && (mode.restrictor == PathMode::Restrictor::WALK || mode.restrictor == PathMode::Restrictor::DEFAULT
        ))
    {
        throw QuerySemanticException("Unbounded quantifiers must appear within a restrictive path mode"
                                     " or a selective search prefix.");
    }
}

void CheckUnboundedRepetition::visit(OpGraphPatternList& op)
{
    for (auto& pattern : op.patterns) {
        pattern->accept_visitor(*this);
    }
}

void CheckUnboundedRepetition::visit(OpBasicGraphPattern& op)
{
    for (auto& pattern : op.patterns) {
        pattern->accept_visitor(*this);
    }
}

void CheckUnboundedRepetition::visit(OpRepetition& op)
{
    op.op->accept_visitor(*this);

    if (!op.upper.has_value()) {
        has_unbounded_repetition = true;
    }
}

void CheckUnboundedRepetition::visit(OpReturn& op)
{
    op.op->accept_visitor(*this);
}

void CheckUnboundedRepetition::visit(OpGroupBy& op)
{
    op.op->accept_visitor(*this);
}

void CheckUnboundedRepetition::visit(OpQueryStatements& op)
{
    for (auto& pattern : op.ops) {
        pattern->accept_visitor(*this);
    }
}

void CheckUnboundedRepetition::visit(OpOrderBy& op)
{
    op.op->accept_visitor(*this);
}

void CheckUnboundedRepetition::visit(OpFilter& op)
{
    op.op->accept_visitor(*this);
}

void CheckUnboundedRepetition::visit(OpOptProperties& op)
{
    op.op->accept_visitor(*this);
}

void CheckUnboundedRepetition::visit(OpPathUnion& op)
{
    for (auto& pattern : op.op_list) {
        pattern->accept_visitor(*this);
    }
}

void CheckUnboundedRepetition::visit(OpOrderByStatement&) { }

void CheckUnboundedRepetition::visit(OpFilterStatement&) { }

void CheckUnboundedRepetition::visit(OpLet&) { }

void CheckUnboundedRepetition::visit(OpProperty&) { }

void CheckUnboundedRepetition::visit(OpNode&) { }

void CheckUnboundedRepetition::visit(OpEdge&) { }

void CheckUnboundedRepetition::visit(OpNodeLabel&) { }

void CheckUnboundedRepetition::visit(OpEdgeLabel&) { }
