#pragma once

#include <vector>

#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_term.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_var.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_visitor.h"
#include "query/executor/binding_iter/binding_expr/mql_binding_exprs.h"

namespace MQL {

class BindingExprPrinter : public ::BindingExprPrinter {
public:
    using ::BindingExprPrinter::visit;

    BindingExprPrinter(std::ostream& os) : ::BindingExprPrinter(os) { }

    void visit(BindingExprAddition&)        override;
    void visit(BindingExprAnd&)             override;
    void visit(BindingExprDivision&)        override;
    void visit(BindingExprEquals&)          override;
    void visit(BindingExprIs&)              override;
    void visit(BindingExprLessOrEquals&)    override;
    void visit(BindingExprLess&)            override;
    void visit(BindingExprModulo&)          override;
    void visit(BindingExprMultiplication&)  override;
    void visit(BindingExprNotEquals&)       override;
    void visit(BindingExprNot&)             override;
    void visit(BindingExprOr&)              override;
    void visit(BindingExprSubtraction&)     override;
    void visit(BindingExprUnaryMinus&)      override;
    void visit(BindingExprUnaryPlus&)       override;
    void visit(BindingExprRegex&)           override;
};
} // namespace MQL