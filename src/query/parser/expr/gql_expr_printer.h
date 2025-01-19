#pragma once

#include "query/parser/expr/expr_printer.h"

namespace GQL {

class ExprPrinter : public AbstractExprPrinter {
public:
    ExprPrinter(std::ostream& os) : AbstractExprPrinter(os) { }

    void visit(ExprOr&) override;
    void visit(ExprAnd&) override;
    void visit(ExprNot&) override;
    void visit(ExprHasNodeLabel&) override;
    void visit(ExprHasEdgeLabel&) override;

    void visit(GQL::ExprEquals&) override;
    void visit(GQL::ExprVar&) override;
    void visit(GQL::ExprProperty&) override;
    void visit(GQL::ExprConcat&) override;
    void visit(GQL::ExprGreaterOrEquals&) override;
    void visit(GQL::ExprGreater&) override;
    void visit(GQL::ExprIs&) override;
    void visit(GQL::ExprLessOrEquals&) override;
    void visit(GQL::ExprLess&) override;
    void visit(GQL::ExprNotEquals&) override;
    void visit(GQL::ExprXor&) override;
    void visit(GQL::ExprTerm&) override;
    void visit(GQL::ExprAddition&) override;
    void visit(GQL::ExprSubtraction&) override;
    void visit(GQL::ExprMultiplication&) override;
    void visit(GQL::ExprDivision&) override;
};
} // namespace GQL
