#include "check_statements.h"

#include "query/parser/op/gql/ops.h"

using namespace GQL;

void CheckStatements::visit(OpQueryStatements& op_statements)
{
    op_statements.ops[0]->accept_visitor(*this);
    if (!has_match_or_let) {
        throw QueryException("FILTER and ORDER BY statements need to come after a MATCH or LET statement.");
    }
}

void CheckStatements::visit(OpReturn& op_return)
{
    op_return.op->accept_visitor(*this);
}

void CheckStatements::visit(OpGroupBy& op_group_by)
{
    op_group_by.op->accept_visitor(*this);
}

void CheckStatements::visit(OpOrderBy& op_order_by)
{
    op_order_by.op->accept_visitor(*this);
}

void CheckStatements::visit(OpFilter& op)
{
    op.op->accept_visitor(*this);
}

void CheckStatements::visit(OpGraphPatternList&)
{
    has_match_or_let = true;
}

void CheckStatements::visit(OpLet&)
{
    has_match_or_let = true;
}

void CheckStatements::visit(OpFilterStatement&) { }
void CheckStatements::visit(OpOrderByStatement&) { }
