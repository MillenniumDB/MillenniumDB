#pragma once

#include "query/parser/expr/expr_printer.h"

namespace MQL {

class ExprPrinter : public AbstractExprPrinter {
public:
    ExprPrinter(std::ostream& os) : AbstractExprPrinter(os) { }

    void visit(ExprVar&)             override;
    void visit(ExprVarProperty&)     override;
    void visit(ExprConstant&)        override;
    void visit(ExprAddition&)        override;
    void visit(ExprDivision&)        override;
    void visit(ExprModulo&)          override;
    void visit(ExprMultiplication&)  override;
    void visit(ExprSubtraction&)     override;
    void visit(ExprUnaryMinus&)      override;
    void visit(ExprUnaryPlus&)       override;
    void visit(ExprEquals&)          override;
    void visit(ExprGreaterOrEquals&) override;
    void visit(ExprGreater&)         override;
    void visit(ExprIs&)              override;
    void visit(ExprLessOrEquals&)    override;
    void visit(ExprLess&)            override;
    void visit(ExprNotEquals&)       override;
    void visit(ExprAnd&)             override;
    void visit(ExprNot&)             override;
    void visit(ExprOr&)              override;
    void visit(ExprRegex&)           override;
    void visit(ExprTensorDistance&)  override;
    void visit(ExprTextSearch&)      override;

    void visit(MQL::ExprAggAvg&)      override;
    void visit(MQL::ExprAggCountAll&) override;
    void visit(MQL::ExprAggCount&)    override;
    void visit(MQL::ExprAggMax&)      override;
    void visit(MQL::ExprAggMin&)      override;
    void visit(MQL::ExprAggSum&)      override;
};
} // namespace MQL