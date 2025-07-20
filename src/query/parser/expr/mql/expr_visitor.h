#pragma once

namespace MQL {

class ExprVar;
class ExprVarProperty;
class ExprConstant;
class ExprAddition;
class ExprDivision;
class ExprModulo;
class ExprMultiplication;
class ExprSubtraction;
class ExprUnaryMinus;
class ExprUnaryPlus;
class ExprEquals;
class ExprGreaterOrEquals;
class ExprGreater;
class ExprIs;
class ExprLessOrEquals;
class ExprLess;
class ExprNotEquals;
class ExprAnd;
class ExprNot;
class ExprOr;

class ExprAggAvg;
class ExprAggCountAll;
class ExprAggCount;
class ExprAggMax;
class ExprAggMin;
class ExprAggSum;

class ExprRegex;
class ExprCosineSimilarity;
class ExprCosineDistance;
class ExprManhattanDistance;
class ExprEuclideanDistance;

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    virtual void visit(MQL::ExprVar&) = 0;
    virtual void visit(MQL::ExprVarProperty&) = 0;
    virtual void visit(MQL::ExprConstant&) = 0;
    virtual void visit(MQL::ExprAddition&) = 0;
    virtual void visit(MQL::ExprDivision&) = 0;
    virtual void visit(MQL::ExprModulo&) = 0;
    virtual void visit(MQL::ExprMultiplication&) = 0;
    virtual void visit(MQL::ExprSubtraction&) = 0;
    virtual void visit(MQL::ExprUnaryMinus&) = 0;
    virtual void visit(MQL::ExprUnaryPlus&) = 0;
    virtual void visit(MQL::ExprEquals&) = 0;
    virtual void visit(MQL::ExprGreaterOrEquals&) = 0;
    virtual void visit(MQL::ExprGreater&) = 0;
    virtual void visit(MQL::ExprIs&) = 0;
    virtual void visit(MQL::ExprLessOrEquals&) = 0;
    virtual void visit(MQL::ExprLess&) = 0;
    virtual void visit(MQL::ExprNotEquals&) = 0;
    virtual void visit(MQL::ExprAnd&) = 0;
    virtual void visit(MQL::ExprNot&) = 0;
    virtual void visit(MQL::ExprOr&) = 0;
    virtual void visit(MQL::ExprAggAvg&) = 0;
    virtual void visit(MQL::ExprAggCountAll&) = 0;
    virtual void visit(MQL::ExprAggCount&) = 0;
    virtual void visit(MQL::ExprAggMax&) = 0;
    virtual void visit(MQL::ExprAggMin&) = 0;
    virtual void visit(MQL::ExprAggSum&) = 0;
    virtual void visit(MQL::ExprRegex&) = 0;
    virtual void visit(MQL::ExprCosineSimilarity&) = 0;
    virtual void visit(MQL::ExprCosineDistance&) = 0;
    virtual void visit(MQL::ExprManhattanDistance&) = 0;
    virtual void visit(MQL::ExprEuclideanDistance&) = 0;
};
} // namespace MQL
