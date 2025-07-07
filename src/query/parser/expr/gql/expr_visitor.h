#pragma once

#include "query/exceptions.h"

namespace GQL {
class OpFilter;
class ExprAnd;
class ExprOr;
class ExprNot;
class ExprHasNodeLabel;
class ExprHasEdgeLabel;
class ExprWildcardLabel;
class ExprEquals;
class ExprVar;
class ExprProperty;
class ExprConcat;
class ExprGreaterOrEquals;
class ExprGreater;
class ExprIs;
class ExprLessOrEquals;
class ExprNotEquals;
class ExprLess;
class ExprLess;
class ExprXor;
class ExprTerm;
class ExprAddition;
class ExprSubtraction;
class ExprMultiplication;
class ExprDivision;
class ExprAbs;
class ExprModulo;
class ExprSin;
class ExprCos;
class ExprTan;
class ExprCot;
class ExprSinh;
class ExprCosh;
class ExprTanh;
class ExprAsin;
class ExprAcos;
class ExprAtan;
class ExprDegrees;
class ExprRadians;
class ExprLog;
class ExprLog10;
class ExprLn;
class ExprExp;
class ExprPower;
class ExprSqrt;
class ExprFloor;
class ExprCeil;
class ExprLength;
class ExprSubStr;
class ExprFold;
class ExprSingleTrim;
class ExprMultiTrim;
class ExprNormalize;
class ExprNullIf;
class ExprCoalesce;
class ExprSimpleCase;
class ExprSearchedCase;
class ExprCast;

class ExprAggCountAll;
class ExprAggCount;
class ExprAggAvg;
class ExprAggMax;
class ExprAggMin;
class ExprAggSum;
class ExprAggStddevPop;
class ExprAggStddevSamp;
class ExprAggCollect;
class ExprAggPercentileCont;
class ExprAggPercentileDisc;

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    // GQL
    virtual void visit(GQL::ExprAnd&)
    {
        throw LogicException("visit GQL::ExprAnd not implemented");
    }
    virtual void visit(GQL::ExprOr&)
    {
        throw LogicException("visit GQL::ExprOr not implemented");
    }
    virtual void visit(GQL::ExprNot&)
    {
        throw LogicException("visit GQL::ExprNot not implemented");
    }
    virtual void visit(GQL::ExprHasNodeLabel&)
    {
        throw LogicException("visit GQL::ExprHasNodeLabel not implemented");
    }
    virtual void visit(GQL::ExprHasEdgeLabel&)
    {
        throw LogicException("visit GQL::ExprHasEdgeLabel not implemented");
    }
    virtual void visit(GQL::ExprWildcardLabel&)
    {
        throw LogicException("visit GQL::ExprWildcardLabel not implemented");
    }
    virtual void visit(GQL::ExprEquals&)
    {
        throw LogicException("visit GQL::ExprEquals not implemented");
    }
    virtual void visit(GQL::ExprVar&)
    {
        throw LogicException("visit GQL::ExprEquals not implemented");
    }
    virtual void visit(GQL::ExprProperty&)
    {
        throw LogicException("visit GQL::ExprEquals not implemented");
    }
    virtual void visit(GQL::ExprXor&)
    {
        throw LogicException("visit GQL::NotXor not implemented");
    }
    virtual void visit(GQL::ExprConcat&)
    {
        throw LogicException("visit GQL::ExprConcat not implemented");
    }
    virtual void visit(GQL::ExprGreaterOrEquals&)
    {
        throw LogicException("visit GQL::ExprGreaterOrEquals not implemented");
    }
    virtual void visit(GQL::ExprGreater&)
    {
        throw LogicException("visit GQL::ExprGreater not implemented");
    }
    virtual void visit(GQL::ExprIs&)
    {
        throw LogicException("visit GQL::ExprIs not implemented");
    }
    virtual void visit(GQL::ExprLessOrEquals&)
    {
        throw LogicException("visit GQL::ExprLessOrEquals not implemented");
    }
    virtual void visit(GQL::ExprLess&)
    {
        throw LogicException("visit GQL::ExprLess not implemented");
    }
    virtual void visit(GQL::ExprNotEquals&)
    {
        throw LogicException("visit GQL::ExprNotEquals not implemented");
    }
    virtual void visit(GQL::ExprTerm&)
    {
        throw LogicException("visit GQL::ExprTerm not implemented");
    }
    virtual void visit(GQL::ExprAddition&)
    {
        throw LogicException("visit GQL::ExprAddition not implemented");
    }
    virtual void visit(GQL::ExprSubtraction&)
    {
        throw LogicException("visit GQL::ExprSubtraction not implemented");
    }
    virtual void visit(GQL::ExprMultiplication&)
    {
        throw LogicException("visit GQL::ExprMultiplication not implemented");
    }
    virtual void visit(GQL::ExprDivision&)
    {
        throw LogicException("visit GQL::ExprDivision not implemented");
    }
    virtual void visit(GQL::ExprAbs&)
    {
        throw LogicException("visit GQL::ExprAbs not implemented");
    }
    virtual void visit(GQL::ExprModulo&)
    {
        throw LogicException("visit GQL::ExprModulo not implemented");
    }
    virtual void visit(GQL::ExprSin&)
    {
        throw LogicException("visit GQL::ExprSin not implemented");
    }
    virtual void visit(GQL::ExprCos&)
    {
        throw LogicException("visit GQL::ExprCos not implemented");
    }
    virtual void visit(GQL::ExprTan&)
    {
        throw LogicException("visit GQL::ExprTan not implemented");
    }
    virtual void visit(GQL::ExprCot&)
    {
        throw LogicException("visit GQL::ExprCot not implemented");
    }
    virtual void visit(GQL::ExprSinh&)
    {
        throw LogicException("visit GQL::ExprSinh not implemented");
    }
    virtual void visit(GQL::ExprCosh&)
    {
        throw LogicException("visit GQL::ExprCosh not implemented");
    }
    virtual void visit(GQL::ExprTanh&)
    {
        throw LogicException("visit GQL::ExprTanh not implemented");
    }
    virtual void visit(GQL::ExprAsin&)
    {
        throw LogicException("visit GQL::ExprAsin not implemented");
    }
    virtual void visit(GQL::ExprAcos&)
    {
        throw LogicException("visit GQL::ExprAcos not implemented");
    }
    virtual void visit(GQL::ExprAtan&)
    {
        throw LogicException("visit GQL::ExprAtan not implemented");
    }
    virtual void visit(GQL::ExprDegrees&)
    {
        throw LogicException("visit GQL::ExprDegrees not implemented");
    }
    virtual void visit(GQL::ExprRadians&)
    {
        throw LogicException("visit GQL::ExprRadians not implemented");
    }
    virtual void visit(GQL::ExprLog&)
    {
        throw LogicException("visit GQL::ExprLog not implemented");
    }
    virtual void visit(GQL::ExprLog10&)
    {
        throw LogicException("visit GQL::ExprLog10 not implemented");
    }
    virtual void visit(GQL::ExprLn&)
    {
        throw LogicException("visit GQL::ExprLn not implemented");
    }
    virtual void visit(GQL::ExprExp&)
    {
        throw LogicException("visit GQL::ExprExp not implemented");
    }
    virtual void visit(GQL::ExprPower&)
    {
        throw LogicException("visit GQL::ExprPower not implemented");
    }
    virtual void visit(GQL::ExprSqrt&)
    {
        throw LogicException("visit GQL::ExprSqrt not implemented");
    }
    virtual void visit(GQL::ExprFloor&)
    {
        throw LogicException("visit GQL::ExprFloor not implemented");
    }
    virtual void visit(GQL::ExprCeil&)
    {
        throw LogicException("visit GQL::ExprCeil not implemented");
    }
    virtual void visit(GQL::ExprLength&)
    {
        throw LogicException("visit GQL::ExprLength not implemented");
    }
    virtual void visit(GQL::ExprSubStr&)
    {
        throw LogicException("visit GQL::ExprSubStr not implemented");
    }
    virtual void visit(GQL::ExprFold&)
    {
        throw LogicException("visit GQL::ExprFold not implemented");
    }
    virtual void visit(GQL::ExprSingleTrim&)
    {
        throw LogicException("visit GQL::ExprSingleTrim not implemented");
    }
    virtual void visit(GQL::ExprMultiTrim&)
    {
        throw LogicException("visit GQL::ExprMultiTrim not implemented");
    }
    virtual void visit(GQL::ExprNormalize&)
    {
        throw LogicException("visit GQL::ExprNormalize not implemented");
    }
    virtual void visit(GQL::ExprNullIf&)
    {
        throw LogicException("visit GQL::ExprNullIf not implemented");
    }
    virtual void visit(GQL::ExprCoalesce&)
    {
        throw LogicException("visit GQL::ExprCoalesce not implemented");
    }
    virtual void visit(GQL::ExprSimpleCase&)
    {
        throw LogicException("visit GQL::ExprSimpleCase not implemented");
    }
    virtual void visit(GQL::ExprSearchedCase&)
    {
        throw LogicException("visit GQL::ExprSearchedCase not implemented");
    }
    virtual void visit(GQL::ExprCast&)
    {
        throw LogicException("visit GQL::ExprCast not implemented");
    }
    virtual void visit(GQL::ExprAggCountAll&)
    {
        throw LogicException("visit GQL::ExprAggCountAll not implemented");
    }
    virtual void visit(GQL::ExprAggCount&)
    {
        throw LogicException("visit GQL::ExprAggCount not implemented");
    }
    virtual void visit(GQL::ExprAggAvg&)
    {
        throw LogicException("visit GQL::ExprAggAvg not implemented");
    }
    virtual void visit(GQL::ExprAggMax&)
    {
        throw LogicException("visit GQL::ExprAggMax not implemented");
    }
    virtual void visit(GQL::ExprAggMin&)
    {
        throw LogicException("visit GQL::ExprAggMin not implemented");
    }
    virtual void visit(GQL::ExprAggSum&)
    {
        throw LogicException("visit GQL::ExprAggSum not implemented");
    }
    virtual void visit(GQL::ExprAggStddevPop&)
    {
        throw LogicException("visit GQL::ExprAggStddevPop not implemented");
    }
    virtual void visit(GQL::ExprAggStddevSamp&)
    {
        throw LogicException("visit GQL::ExprAggStddevSamp not implemented");
    }
    virtual void visit(GQL::ExprAggCollect&)
    {
        throw LogicException("visit GQL::ExprAggCollect not implemented");
    }
    virtual void visit(GQL::ExprAggPercentileCont&)
    {
        throw LogicException("visit GQL::ExprAggPercentileCont not implemented");
    }
    virtual void visit(GQL::ExprAggPercentileDisc&)
    {
        throw LogicException("visit GQL::ExprAggPercentileDisc not implemented");
    }
};
} // namespace GQL
