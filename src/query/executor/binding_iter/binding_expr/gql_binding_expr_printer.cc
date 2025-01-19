#include "gql_binding_expr_printer.h"
#include "gql_binding_exprs.h"

namespace GQL {

void BindingExprPrinter::visit(GQL::BindingExprOr& expr) {
    os << '(';
    auto first = true;
    for (auto& e : expr.or_list) {
        if (first) {
            first = false;
        } else {
            os << " OR ";
        }
        e->accept_visitor(*this);
    }
    os << ")";
}


void BindingExprPrinter::visit(BindingExprAnd& expr) {
    os << '(';
    auto first = true;
    for (auto& e : expr.and_list) {
        if (first) {
            first = false;
        } else {
            os << " AND ";
        }
        e->accept_visitor(*this);
    }
    os << ")";
}

void BindingExprPrinter::visit(BindingExprNot& expr) {
    os << "NOT(";
    expr.expr->accept_visitor(*this);
    os << ")";
}


void BindingExprPrinter::visit(GQL::BindingExprHasNodeLabel& expr) {
    os << "HasNodeLabel(";
    os << expr.node_id << ", " << expr.label_id;
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprHasEdgeLabel& expr) {
    os << "HasEdgeLabel(";
    os << expr.edge_id << ", " << expr.label_id;
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprEquals& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " = ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprProperty& expr) {
    os << '(';
    os << expr.property_var_id;
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprConcat& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " CONCAT ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprGreaterOrEquals& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " >= ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprGreater& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " > ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprIs& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " IS ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprLessOrEquals& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " <= ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprLess& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " < ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprNotEquals& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " != ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprXor& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " XOR ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprAddition& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " + ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprSubtraction& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " - ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprMultiplication& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " * ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}


void BindingExprPrinter::visit(GQL::BindingExprDivision& expr) {
    os << '(';
    expr.lhs->accept_visitor(*this);
    os << " / ";
    expr.rhs->accept_visitor(*this);
    os << ')';
}

} // namespace GQL
