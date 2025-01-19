#include "gql_expr_printer.h"
#include "query/parser/expr/gql_exprs.h"
#include "query/query_context.h"

namespace GQL {

void ExprPrinter::visit(ExprNot& expr)
{
    os << "NOT(";
    expr.expr->accept_visitor(*this);
    os << ")";
}

void ExprPrinter::visit(ExprAnd& expr)
{
    os << "AND(";
    expr.exprs[0]->accept_visitor(*this);
    for (size_t i = 1; i < expr.exprs.size(); ++i) {
        os << ", ";
        expr.exprs[i]->accept_visitor(*this);
    }
    os << ')';
}

void ExprPrinter::visit(ExprOr& expr)
{
    os << "OR(";
    expr.exprs[0]->accept_visitor(*this);
    for (size_t i = 1; i < expr.exprs.size(); ++i) {
        os << ", ";
        expr.exprs[i]->accept_visitor(*this);
    }
    os << ')';
}

void ExprPrinter::visit(ExprHasNodeLabel& expr)
{
    os << "HasNodeLabel(";
    os << expr.node_id << ", " << expr.label_id;
    os << ')';
}

void ExprPrinter::visit(ExprHasEdgeLabel& expr)
{
    os << "HasEdgeLabel(";
    os << expr.edge_id << ", " << expr.label_id;
    os << ')';
}

void ExprPrinter::visit(ExprEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " = ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprNotEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " != ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprXor& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " XOR ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprConcat& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " || ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprGreaterOrEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " >= ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprGreater& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " > ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprLess& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " < ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprLessOrEquals& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " < ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprIs& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " IS ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprAddition& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " + ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprSubtraction& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " - ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprMultiplication& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " * ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprDivision& expr)
{
    expr.lhs->accept_visitor(*this);
    os << " / ";
    expr.rhs->accept_visitor(*this);
}

void ExprPrinter::visit(ExprVar& expr)
{
    os << expr.id;
}

void ExprPrinter::visit(ExprProperty& expr)
{
    os << expr.object << "." << expr.key;
}

void ExprPrinter::visit(ExprTerm& expr)
{
    os << expr.term;
}

} // namespace GQL
