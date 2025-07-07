#pragma once

#include "query/exceptions.h"

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

    virtual void visit(MQL::ExprVar&)
    {
        throw LogicException("visit MQL::ExprVar not implemented");
    }
    virtual void visit(MQL::ExprVarProperty&)
    {
        throw LogicException("visit MQL::ExprVarProperty not implemented");
    }
    virtual void visit(MQL::ExprConstant&)
    {
        throw LogicException("visit MQL::ExprConstant not implemented");
    }
    virtual void visit(MQL::ExprAddition&)
    {
        throw LogicException("visit MQL::ExprAddition not implemented");
    }
    virtual void visit(MQL::ExprDivision&)
    {
        throw LogicException("visit MQL::ExprDivision not implemented");
    }
    virtual void visit(MQL::ExprModulo&)
    {
        throw LogicException("visit MQL::ExprModulo not implemented");
    }
    virtual void visit(MQL::ExprMultiplication&)
    {
        throw LogicException("visit MQL::ExprMultiplication not implemented");
    }
    virtual void visit(MQL::ExprSubtraction&)
    {
        throw LogicException("visit MQL::ExprSubtraction not implemented");
    }
    virtual void visit(MQL::ExprUnaryMinus&)
    {
        throw LogicException("visit MQL::ExprUnaryMinus not implemented");
    }
    virtual void visit(MQL::ExprUnaryPlus&)
    {
        throw LogicException("visit MQL::ExprUnaryPlus not implemented");
    }
    virtual void visit(MQL::ExprEquals&)
    {
        throw LogicException("visit MQL::ExprEquals not implemented");
    }
    virtual void visit(MQL::ExprGreaterOrEquals&)
    {
        throw LogicException("visit MQL::ExprGreaterOrEquals not implemented");
    }
    virtual void visit(MQL::ExprGreater&)
    {
        throw LogicException("visit MQL::ExprGreater not implemented");
    }
    virtual void visit(MQL::ExprIs&)
    {
        throw LogicException("visit MQL::ExprIs not implemented");
    }
    virtual void visit(MQL::ExprLessOrEquals&)
    {
        throw LogicException("visit MQL::ExprLessOrEquals not implemented");
    }
    virtual void visit(MQL::ExprLess&)
    {
        throw LogicException("visit MQL::ExprLess not implemented");
    }
    virtual void visit(MQL::ExprNotEquals&)
    {
        throw LogicException("visit MQL::ExprNotEquals not implemented");
    }
    virtual void visit(MQL::ExprAnd&)
    {
        throw LogicException("visit MQL::ExprAnd not implemented");
    }
    virtual void visit(MQL::ExprNot&)
    {
        throw LogicException("visit MQL::ExprNot not implemented");
    }
    virtual void visit(MQL::ExprOr&)
    {
        throw LogicException("visit MQL::ExprOr not implemented");
    }
    virtual void visit(MQL::ExprAggAvg&)
    {
        throw LogicException("visit MQL::ExprAggAvg not implemented");
    }
    virtual void visit(MQL::ExprAggCountAll&)
    {
        throw LogicException("visit MQL::ExprAggCountAll not implemented");
    }
    virtual void visit(MQL::ExprAggCount&)
    {
        throw LogicException("visit MQL::ExprAggCount not implemented");
    }
    virtual void visit(MQL::ExprAggMax&)
    {
        throw LogicException("visit MQL::ExprAggMax not implemented");
    }
    virtual void visit(MQL::ExprAggMin&)
    {
        throw LogicException("visit MQL::ExprAggMin not implemented");
    }
    virtual void visit(MQL::ExprAggSum&)
    {
        throw LogicException("visit MQL::ExprAggSum not implemented");
    }
    virtual void visit(MQL::ExprRegex&)
    {
        throw LogicException("visit MQL::ExprRegex not implemented");
    }
    virtual void visit(MQL::ExprCosineSimilarity&)
    {
        throw LogicException("visit MQL::ExprCosineSimilarity not implemented");
    }
    virtual void visit(MQL::ExprCosineDistance&)
    {
        throw LogicException("visit MQL::ExprCosineDistance not implemented");
    }
    virtual void visit(MQL::ExprManhattanDistance&)
    {
        throw LogicException("visit MQL::ExprManhattanDistance not implemented");
    }
    virtual void visit(MQL::ExprEuclideanDistance&)
    {
        throw LogicException("visit MQL::ExprEuclideanDistance not implemented");
    }
};
} // namespace MQL
