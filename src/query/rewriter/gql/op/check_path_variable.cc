#include "check_path_variable.h"

using namespace GQL;

void CheckPathVariable::visit(OpGraphPattern& op)
{
    op.op->accept_visitor(*this);

    if (op.path_var_id.has_value() && (can_start_with_edge || can_end_with_edge)) {
        throw QuerySemanticException(
            "The path variable \"" + get_query_ctx().get_var_name(*op.path_var_id)
            + "\" cannot be bound to a path that starts or ends with an edge."
        );
    }
}

void CheckPathVariable::visit(OpNode& op)
{
    // a node that is off the path adds no element to it
    can_be_empty = !op.on_path;
    can_start_with_edge = false;
    can_end_with_edge = false;
}

void CheckPathVariable::visit(OpEdge&)
{
    can_be_empty = false;
    can_start_with_edge = true;
    can_end_with_edge = true;
}

void CheckPathVariable::visit(OpBasicGraphPattern& op)
{
    bool all_can_be_empty = true;
    bool starts_with_edge = false;
    bool ends_with_edge = false;

    for (auto& pattern : op.patterns) {
        pattern->accept_visitor(*this);

        // the path starts in the first pattern whose match is not empty, and ends in the last
        starts_with_edge = starts_with_edge || (all_can_be_empty && can_start_with_edge);
        ends_with_edge = can_end_with_edge || (can_be_empty && ends_with_edge);
        all_can_be_empty = all_can_be_empty && can_be_empty;
    }

    can_be_empty = all_can_be_empty;
    can_start_with_edge = starts_with_edge;
    can_end_with_edge = ends_with_edge;
}

void CheckPathVariable::visit(OpPathUnion& op)
{
    bool some_can_be_empty = false;
    bool starts_with_edge = false;
    bool ends_with_edge = false;

    for (auto& pattern : op.op_list) {
        pattern->accept_visitor(*this);

        some_can_be_empty = some_can_be_empty || can_be_empty;
        starts_with_edge = starts_with_edge || can_start_with_edge;
        ends_with_edge = ends_with_edge || can_end_with_edge;
    }

    can_be_empty = some_can_be_empty;
    can_start_with_edge = starts_with_edge;
    can_end_with_edge = ends_with_edge;
}

void CheckPathVariable::visit(OpRepetition& op)
{
    op.op->accept_visitor(*this);

    can_be_empty = can_be_empty || op.lower == 0;
}

void CheckPathVariable::visit(OpWhere& op)
{
    op.op->accept_visitor(*this);
}

void CheckPathVariable::visit(OpGraphPatternList& op)
{
    for (auto& pattern : op.patterns) {
        pattern->accept_visitor(*this);
    }
}

void CheckPathVariable::visit(OpReturn& op)
{
    op.op->accept_visitor(*this);
}

void CheckPathVariable::visit(OpGroupBy& op)
{
    op.op->accept_visitor(*this);
}

void CheckPathVariable::visit(OpQueryStatements& op)
{
    for (auto& pattern : op.ops) {
        pattern->accept_visitor(*this);
    }
}

void CheckPathVariable::visit(OpLinearPattern&) { }
void CheckPathVariable::visit(OpOrderBy&) { }
void CheckPathVariable::visit(OpFilter&) { }
void CheckPathVariable::visit(OpLet&) { }
void CheckPathVariable::visit(OpUnitTable&) { }
void CheckPathVariable::visit(OpEmpty&) { }
