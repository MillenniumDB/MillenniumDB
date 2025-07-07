#pragma once

#include "query/parser/expr/mql/expr_visitor.h"
#include "query/parser/op/mql/op.h"

namespace MQL {

class ExprPrinter : public ExprVisitor {
public:
    std::ostream& os;
    std::vector<Op*> ops;

    ExprPrinter(std::ostream& os) :
        os(os)
    { }

    void visit(ExprVar&) override;
    void visit(ExprVarProperty&) override;
    void visit(ExprConstant&) override;
    void visit(ExprAddition&) override;
    void visit(ExprDivision&) override;
    void visit(ExprModulo&) override;
    void visit(ExprMultiplication&) override;
    void visit(ExprSubtraction&) override;
    void visit(ExprUnaryMinus&) override;
    void visit(ExprUnaryPlus&) override;
    void visit(ExprEquals&) override;
    void visit(ExprGreaterOrEquals&) override;
    void visit(ExprGreater&) override;
    void visit(ExprIs&) override;
    void visit(ExprLessOrEquals&) override;
    void visit(ExprLess&) override;
    void visit(ExprNotEquals&) override;
    void visit(ExprAnd&) override;
    void visit(ExprNot&) override;
    void visit(ExprOr&) override;
    void visit(ExprRegex&) override;
    void visit(ExprCosineDistance&) override;
    void visit(ExprCosineSimilarity&) override;
    void visit(ExprManhattanDistance&) override;
    void visit(ExprEuclideanDistance&) override;

    void visit(MQL::ExprAggAvg&) override;
    void visit(MQL::ExprAggCountAll&) override;
    void visit(MQL::ExprAggCount&) override;
    void visit(MQL::ExprAggMax&) override;
    void visit(MQL::ExprAggMin&) override;
    void visit(MQL::ExprAggSum&) override;
};
} // namespace MQL
