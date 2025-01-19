#pragma once

#include <vector>

#include "binding_expr_printer.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_term.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_var.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_visitor.h"
#include "query/executor/binding_iter/binding_expr/gql_binding_exprs.h"


namespace GQL {

class BindingExprPrinter : public ::BindingExprPrinter {
public:
    using ::BindingExprPrinter::visit;

    BindingExprPrinter(std::ostream& os) : ::BindingExprPrinter(os) { }

    void visit(GQL::BindingExprOr&) override;
    void visit(GQL::BindingExprAnd&) override;
    void visit(GQL::BindingExprNot&) override;
    void visit(GQL::BindingExprHasNodeLabel&) override;
    void visit(GQL::BindingExprHasEdgeLabel&) override;
    void visit(GQL::BindingExprEquals&) override;
    void visit(GQL::BindingExprProperty&) override;
    void visit(GQL::BindingExprConcat&) override;
    void visit(GQL::BindingExprGreaterOrEquals&) override;
    void visit(GQL::BindingExprGreater&) override;
    void visit(GQL::BindingExprIs&) override;
    void visit(GQL::BindingExprLessOrEquals&) override;
    void visit(GQL::BindingExprLess&) override;
    void visit(GQL::BindingExprNotEquals&) override;
    void visit(GQL::BindingExprXor&) override;
    void visit(GQL::BindingExprAddition&) override;
    void visit(GQL::BindingExprSubtraction&) override;
    void visit(GQL::BindingExprMultiplication&) override;
    void visit(GQL::BindingExprDivision&) override;
};
} // namespace GQL
