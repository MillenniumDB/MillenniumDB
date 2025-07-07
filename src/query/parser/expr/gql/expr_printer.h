#pragma once

#include <vector>

#include "query/parser/expr/gql/expr_visitor.h"
#include "query/parser/op/gql/op.h"

namespace GQL {

class ExprPrinter : public ExprVisitor {
public:
    std::ostream& os;
    std::vector<Op*> ops;

    ExprPrinter(std::ostream& os) :
        os(os)
    { }

    void visit(ExprOr&) override;
    void visit(ExprAnd&) override;
    void visit(ExprNot&) override;
    void visit(ExprHasNodeLabel&) override;
    void visit(ExprHasEdgeLabel&) override;
    void visit(ExprWildcardLabel&) override;

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
    void visit(GQL::ExprAbs&) override;
    void visit(GQL::ExprModulo&) override;
    void visit(GQL::ExprSin&) override;
    void visit(GQL::ExprCos&) override;
    void visit(GQL::ExprTan&) override;
    void visit(GQL::ExprCot&) override;
    void visit(GQL::ExprSinh&) override;
    void visit(GQL::ExprCosh&) override;
    void visit(GQL::ExprTanh&) override;
    void visit(GQL::ExprAsin&) override;
    void visit(GQL::ExprAcos&) override;
    void visit(GQL::ExprAtan&) override;
    void visit(GQL::ExprDegrees&) override;
    void visit(GQL::ExprRadians&) override;
    void visit(GQL::ExprLog&) override;
    void visit(GQL::ExprLog10&) override;
    void visit(GQL::ExprLn&) override;
    void visit(GQL::ExprExp&) override;
    void visit(GQL::ExprPower&) override;
    void visit(GQL::ExprSqrt&) override;
    void visit(GQL::ExprFloor&) override;
    void visit(GQL::ExprCeil&) override;
    void visit(GQL::ExprLength&) override;
    void visit(GQL::ExprSubStr&) override;
    void visit(GQL::ExprFold&) override;
    void visit(GQL::ExprSingleTrim&) override;
    void visit(GQL::ExprMultiTrim&) override;
    void visit(GQL::ExprNormalize&) override;
    void visit(GQL::ExprNullIf&) override;
    void visit(GQL::ExprCoalesce&) override;
    void visit(GQL::ExprSimpleCase&) override;
    void visit(GQL::ExprSearchedCase&) override;
    void visit(GQL::ExprCast&) override;

    void visit(GQL::ExprAggCountAll&) override;
    void visit(GQL::ExprAggCount&) override;
    void visit(GQL::ExprAggAvg&) override;
    void visit(GQL::ExprAggMax&) override;
    void visit(GQL::ExprAggMin&) override;
    void visit(GQL::ExprAggSum&) override;
    void visit(GQL::ExprAggStddevPop&) override;
    void visit(GQL::ExprAggStddevSamp&) override;
    void visit(GQL::ExprAggCollect&) override;
    void visit(GQL::ExprAggPercentileCont&) override;
    void visit(GQL::ExprAggPercentileDisc&) override;
};
} // namespace GQL
