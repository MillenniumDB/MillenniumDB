#pragma once

#include "binding_expr_printer.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_visitor.h"
#include "query/executor/binding_iter/binding_expr/gql_binding_exprs.h"

namespace GQL {

class BindingExprPrinter : public ::BindingExprPrinter {
public:
    using ::BindingExprPrinter::visit;

    BindingExprPrinter(std::ostream& os) :
        ::BindingExprPrinter(os)
    { }

    void visit(GQL::BindingExprOr&) override;
    void visit(GQL::BindingExprAnd&) override;
    void visit(GQL::BindingExprNot&) override;
    void visit(GQL::BindingExprHasNodeLabel&) override;
    void visit(GQL::BindingExprHasEdgeLabel&) override;
    void visit(GQL::BindingExprWildcardLabel&) override;
    void visit(GQL::BindingExprEquals&) override;
    void visit(GQL::BindingExprNodeProperty&) override;
    void visit(GQL::BindingExprEdgeProperty&) override;
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
    void visit(GQL::BindingExprAbs&) override;
    void visit(GQL::BindingExprModulo&) override;
    void visit(GQL::BindingExprSin&) override;
    void visit(GQL::BindingExprCos&) override;
    void visit(GQL::BindingExprTan&) override;
    void visit(GQL::BindingExprCot&) override;
    void visit(GQL::BindingExprSinh&) override;
    void visit(GQL::BindingExprCosh&) override;
    void visit(GQL::BindingExprTanh&) override;
    void visit(GQL::BindingExprAsin&) override;
    void visit(GQL::BindingExprAcos&) override;
    void visit(GQL::BindingExprAtan&) override;
    void visit(GQL::BindingExprDegrees&) override;
    void visit(GQL::BindingExprRadians&) override;
    void visit(GQL::BindingExprLog&) override;
    void visit(GQL::BindingExprLog10&) override;
    void visit(GQL::BindingExprLn&) override;
    void visit(GQL::BindingExprExp&) override;
    void visit(GQL::BindingExprPower&) override;
    void visit(GQL::BindingExprSqrt&) override;
    void visit(GQL::BindingExprFloor&) override;
    void visit(GQL::BindingExprCeil&) override;
    void visit(GQL::BindingExprLength&) override;
    void visit(GQL::BindingExprSubStr&) override;
    void visit(GQL::BindingExprFold&) override;
    void visit(GQL::BindingExprSingleTrim&) override;
    void visit(GQL::BindingExprMultiTrim&) override;
    void visit(GQL::BindingExprNormalize&) override;
    void visit(GQL::BindingExprNullIf&) override;
    void visit(GQL::BindingExprCoalesce&) override;
    void visit(GQL::BindingExprSimpleCase&) override;
    void visit(GQL::BindingExprSearchedCase&) override;
    void visit(GQL::BindingExprCast&) override;
};
} // namespace GQL
