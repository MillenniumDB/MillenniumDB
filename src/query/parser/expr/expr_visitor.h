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
class ExprTensorDistance;
class ExprTextSearch;
}

namespace SPARQL {

class ExprVar;
class ExprTerm;

class ExprNot;
class ExprUnaryMinus;
class ExprUnaryPlus;
class ExprMultiplication;
class ExprDivision;
class ExprAddition;
class ExprSubtraction;
class ExprAnd;
class ExprOr;
class ExprEqual;
class ExprNotEqual;
class ExprLess;
class ExprGreater;
class ExprLessOrEqual;
class ExprGreaterOrEqual;

class ExprIn;
class ExprNotIn;

class ExprAggAvg;
class ExprAggCountAll;
class ExprAggCount;
class ExprAggGroupConcat;
class ExprAggMax;
class ExprAggMin;
class ExprAggSample;
class ExprAggSum;

class ExprAbs;
class ExprBNode;
class ExprBound;
class ExprCeil;
class ExprCoalesce;
class ExprConcat;
class ExprContains;
class ExprDatatype;
class ExprDay;
class ExprEncodeForUri;
class ExprExists;
class ExprFloor;
class ExprHours;
class ExprIf;
class ExprIRI;
class ExprIsBlank;
class ExprIsIRI;
class ExprIsLiteral;
class ExprIsNumeric;
class ExprIsURI;
class ExprLang;
class ExprLangMatches;
class ExprLCase;
class ExprMD5;
class ExprMinutes;
class ExprMonth;
class ExprNotExists;
class ExprNow;
class ExprRand;
class ExprRegex;
class ExprReplace;
class ExprRound;
class ExprSameTerm;
class ExprSeconds;
class ExprSHA1;
class ExprSHA256;
class ExprSHA384;
class ExprSHA512;
class ExprStrAfter;
class ExprStrBefore;
class ExprStrDT;
class ExprStrEnds;
class ExprStrLang;
class ExprStrLen;
class ExprStrStarts;
class ExprStrUUID;
class ExprStr;
class ExprSubStr;
class ExprTimezone;
class ExprTZ;
class ExprUCase;
class ExprURI;
class ExprUUID;
class ExprYear;
class ExprCast;

class ExprCosineSimilarity;
class ExprDot;
class ExprEuclideanDistance;
class ExprManhattanDistance;
class ExprPow;
class ExprSqrt;
class ExprSum;
} // namespace SPARQL


namespace GQL {
class OpFilter;
class ExprAnd;
class ExprOr;
class ExprNot;
class ExprHasNodeLabel;
class ExprHasEdgeLabel;
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
} // namespace GQL

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    virtual void visit(MQL::ExprVar&)             { throw LogicException("visit MQL::ExprVar not implemented"); }
    virtual void visit(MQL::ExprVarProperty&)     { throw LogicException("visit MQL::ExprVarProperty not implemented"); }
    virtual void visit(MQL::ExprConstant&)        { throw LogicException("visit MQL::ExprConstant not implemented"); }
    virtual void visit(MQL::ExprAddition&)        { throw LogicException("visit MQL::ExprAddition not implemented"); }
    virtual void visit(MQL::ExprDivision&)        { throw LogicException("visit MQL::ExprDivision not implemented"); }
    virtual void visit(MQL::ExprModulo&)          { throw LogicException("visit MQL::ExprModulo not implemented"); }
    virtual void visit(MQL::ExprMultiplication&)  { throw LogicException("visit MQL::ExprMultiplication not implemented"); }
    virtual void visit(MQL::ExprSubtraction&)     { throw LogicException("visit MQL::ExprSubtraction not implemented"); }
    virtual void visit(MQL::ExprUnaryMinus&)      { throw LogicException("visit MQL::ExprUnaryMinus not implemented"); }
    virtual void visit(MQL::ExprUnaryPlus&)       { throw LogicException("visit MQL::ExprUnaryPlus not implemented"); }
    virtual void visit(MQL::ExprEquals&)          { throw LogicException("visit MQL::ExprEquals not implemented"); }
    virtual void visit(MQL::ExprGreaterOrEquals&) { throw LogicException("visit MQL::ExprGreaterOrEquals not implemented"); }
    virtual void visit(MQL::ExprGreater&)         { throw LogicException("visit MQL::ExprGreater not implemented"); }
    virtual void visit(MQL::ExprIs&)              { throw LogicException("visit MQL::ExprIs not implemented"); }
    virtual void visit(MQL::ExprLessOrEquals&)    { throw LogicException("visit MQL::ExprLessOrEquals not implemented"); }
    virtual void visit(MQL::ExprLess&)            { throw LogicException("visit MQL::ExprLess not implemented"); }
    virtual void visit(MQL::ExprNotEquals&)       { throw LogicException("visit MQL::ExprNotEquals not implemented"); }
    virtual void visit(MQL::ExprAnd&)             { throw LogicException("visit MQL::ExprAnd not implemented"); }
    virtual void visit(MQL::ExprNot&)             { throw LogicException("visit MQL::ExprNot not implemented"); }
    virtual void visit(MQL::ExprOr&)              { throw LogicException("visit MQL::ExprOr not implemented"); }

    virtual void visit(MQL::ExprAggAvg&)      { throw LogicException("visit MQL::ExprAggAvg not implemented"); }
    virtual void visit(MQL::ExprAggCountAll&) { throw LogicException("visit MQL::ExprAggCountAll not implemented"); }
    virtual void visit(MQL::ExprAggCount&)    { throw LogicException("visit MQL::ExprAggCount not implemented"); }
    virtual void visit(MQL::ExprAggMax&)      { throw LogicException("visit MQL::ExprAggMax not implemented"); }
    virtual void visit(MQL::ExprAggMin&)      { throw LogicException("visit MQL::ExprAggMin not implemented"); }
    virtual void visit(MQL::ExprAggSum&)      { throw LogicException("visit MQL::ExprAggSum not implemented"); }

    virtual void visit(MQL::ExprRegex&)          { throw LogicException("visit MQL::ExprRegex not implemented"); }
    virtual void visit(MQL::ExprTensorDistance&) { throw LogicException("visit MQL::ExprTensorDistance not implemented"); }
    virtual void visit(MQL::ExprTextSearch&)     { throw LogicException("visit MQL::ExprTextSearch not implemented"); }

    // SPARQL exprs
    virtual void visit(SPARQL::ExprVar&)            { throw LogicException("visit SPARQL::ExprVar not implemented"); }
    virtual void visit(SPARQL::ExprNot&)            { throw LogicException("visit SPARQL::ExprNot not implemented"); }
    virtual void visit(SPARQL::ExprUnaryMinus&)     { throw LogicException("visit SPARQL::ExprUnaryMinus not implemented"); }
    virtual void visit(SPARQL::ExprUnaryPlus&)      { throw LogicException("visit SPARQL::ExprUnaryPlus not implemented"); }
    virtual void visit(SPARQL::ExprMultiplication&) { throw LogicException("visit SPARQL::ExprMultiplication not implemented"); }
    virtual void visit(SPARQL::ExprDivision&)       { throw LogicException("visit SPARQL::ExprDivision not implemented"); }
    virtual void visit(SPARQL::ExprAddition&)       { throw LogicException("visit SPARQL::ExprAddition not implemented"); }
    virtual void visit(SPARQL::ExprSubtraction&)    { throw LogicException("visit SPARQL::ExprSubtraction not implemented"); }
    virtual void visit(SPARQL::ExprAnd&)            { throw LogicException("visit SPARQL::ExprAnd not implemented"); }
    virtual void visit(SPARQL::ExprOr&)             { throw LogicException("visit SPARQL::ExprOr not implemented"); }
    virtual void visit(SPARQL::ExprLess&)           { throw LogicException("visit SPARQL::ExprLess not implemented"); }
    virtual void visit(SPARQL::ExprGreater&)        { throw LogicException("visit SPARQL::ExprGreater not implemented"); }
    virtual void visit(SPARQL::ExprEqual&)          { throw LogicException("visit SPARQL::ExprEqual not implemented"); }
    virtual void visit(SPARQL::ExprNotEqual&)       { throw LogicException("visit SPARQL::ExprNotEqual not implemented"); }
    virtual void visit(SPARQL::ExprLessOrEqual&)    { throw LogicException("visit SPARQL::ExprLessOrEqual not implemented"); }
    virtual void visit(SPARQL::ExprGreaterOrEqual&) { throw LogicException("visit SPARQL::ExprGreaterOrEqual not implemented"); }

    virtual void visit(SPARQL::ExprTerm&)           { throw LogicException("visit SPARQL::ExprTerm not implemented"); }
    virtual void visit(SPARQL::ExprIn&)             { throw LogicException("visit SPARQL::ExprIn not implemented"); }
    virtual void visit(SPARQL::ExprNotIn&)          { throw LogicException("visit SPARQL::ExprNotIn not implemented"); }
    virtual void visit(SPARQL::ExprAggAvg&)         { throw LogicException("visit SPARQL::ExprAggAvg not implemented"); }
    virtual void visit(SPARQL::ExprAggCount&)       { throw LogicException("visit SPARQL::ExprAggCount not implemented"); }
    virtual void visit(SPARQL::ExprAggCountAll&)    { throw LogicException("visit SPARQL::ExprAggCountAll not implemented"); }
    virtual void visit(SPARQL::ExprAggGroupConcat&) { throw LogicException("visit SPARQL::ExprAggGroupConcat not implemented"); }
    virtual void visit(SPARQL::ExprAggMax&)         { throw LogicException("visit SPARQL::ExprAggMax not implemented"); }
    virtual void visit(SPARQL::ExprAggMin&)         { throw LogicException("visit SPARQL::ExprAggMin not implemented"); }
    virtual void visit(SPARQL::ExprAggSample&)      { throw LogicException("visit SPARQL::ExprAggSample not implemented"); }
    virtual void visit(SPARQL::ExprAggSum&)         { throw LogicException("visit SPARQL::ExprAggSum not implemented"); }
    virtual void visit(SPARQL::ExprAbs&)            { throw LogicException("visit SPARQL::ExprAbs not implemented"); }
    virtual void visit(SPARQL::ExprBNode&)          { throw LogicException("visit SPARQL::ExprBNode not implemented"); }
    virtual void visit(SPARQL::ExprBound&)          { throw LogicException("visit SPARQL::ExprBound not implemented"); }
    virtual void visit(SPARQL::ExprCeil&)           { throw LogicException("visit SPARQL::ExprCeil not implemented"); }
    virtual void visit(SPARQL::ExprCoalesce&)       { throw LogicException("visit SPARQL::ExprCoalesce not implemented"); }
    virtual void visit(SPARQL::ExprConcat&)         { throw LogicException("visit SPARQL::ExprConcat not implemented"); }
    virtual void visit(SPARQL::ExprContains&)       { throw LogicException("visit SPARQL::ExprContains not implemented"); }
    virtual void visit(SPARQL::ExprDatatype&)       { throw LogicException("visit SPARQL::ExprDatatype not implemented"); }
    virtual void visit(SPARQL::ExprDay&)            { throw LogicException("visit SPARQL::ExprDay not implemented"); }
    virtual void visit(SPARQL::ExprEncodeForUri&)   { throw LogicException("visit SPARQL::ExprEncodeForUri not implemented"); }
    virtual void visit(SPARQL::ExprExists&)         { throw LogicException("visit SPARQL::ExprExists not implemented"); }
    virtual void visit(SPARQL::ExprFloor&)          { throw LogicException("visit SPARQL::ExprFloor not implemented"); }
    virtual void visit(SPARQL::ExprHours&)          { throw LogicException("visit SPARQL::ExprHours not implemented"); }
    virtual void visit(SPARQL::ExprIf&)             { throw LogicException("visit SPARQL::ExprIf not implemented"); }
    virtual void visit(SPARQL::ExprIRI&)            { throw LogicException("visit SPARQL::ExprIRI not implemented"); }
    virtual void visit(SPARQL::ExprIsBlank&)        { throw LogicException("visit SPARQL::ExprIsBlank not implemented"); }
    virtual void visit(SPARQL::ExprIsIRI&)          { throw LogicException("visit SPARQL::ExprIsIRI not implemented"); }
    virtual void visit(SPARQL::ExprIsLiteral&)      { throw LogicException("visit SPARQL::ExprIsLiteral not implemented"); }
    virtual void visit(SPARQL::ExprIsNumeric&)      { throw LogicException("visit SPARQL::ExprIsNumeric not implemented"); }
    virtual void visit(SPARQL::ExprIsURI&)          { throw LogicException("visit SPARQL::ExprIsURI not implemented"); }
    virtual void visit(SPARQL::ExprLang&)           { throw LogicException("visit SPARQL::ExprLang not implemented"); }
    virtual void visit(SPARQL::ExprLangMatches&)    { throw LogicException("visit SPARQL::ExprLangMatches not implemented"); }
    virtual void visit(SPARQL::ExprLCase&)          { throw LogicException("visit SPARQL::ExprLCase not implemented"); }
    virtual void visit(SPARQL::ExprMD5&)            { throw LogicException("visit SPARQL::ExprMD5 not implemented"); }
    virtual void visit(SPARQL::ExprMinutes&)        { throw LogicException("visit SPARQL::ExprMinutes not implemented"); }
    virtual void visit(SPARQL::ExprMonth&)          { throw LogicException("visit SPARQL::ExprMonth not implemented"); }
    virtual void visit(SPARQL::ExprNotExists&)      { throw LogicException("visit SPARQL::ExprNotExists not implemented"); }
    virtual void visit(SPARQL::ExprNow&)            { throw LogicException("visit SPARQL::ExprNow not implemented"); }
    virtual void visit(SPARQL::ExprRand&)           { throw LogicException("visit SPARQL::ExprRand not implemented"); }
    virtual void visit(SPARQL::ExprRegex&)          { throw LogicException("visit SPARQL::ExprRegex not implemented"); }
    virtual void visit(SPARQL::ExprReplace&)        { throw LogicException("visit SPARQL::ExprReplace not implemented"); }
    virtual void visit(SPARQL::ExprRound&)          { throw LogicException("visit SPARQL::ExprRound not implemented"); }
    virtual void visit(SPARQL::ExprSameTerm&)       { throw LogicException("visit SPARQL::ExprSameTerm not implemented"); }
    virtual void visit(SPARQL::ExprSeconds&)        { throw LogicException("visit SPARQL::ExprSeconds not implemented"); }
    virtual void visit(SPARQL::ExprSHA1&)           { throw LogicException("visit SPARQL::ExprSHA1 not implemented"); }
    virtual void visit(SPARQL::ExprSHA256&)         { throw LogicException("visit SPARQL::ExprSHA256 not implemented"); }
    virtual void visit(SPARQL::ExprSHA384&)         { throw LogicException("visit SPARQL::ExprSHA384 not implemented"); }
    virtual void visit(SPARQL::ExprSHA512&)         { throw LogicException("visit SPARQL::ExprSHA512 not implemented"); }
    virtual void visit(SPARQL::ExprStrAfter&)       { throw LogicException("visit SPARQL::ExprStrAfter not implemented"); }
    virtual void visit(SPARQL::ExprStrBefore&)      { throw LogicException("visit SPARQL::ExprStrBefore not implemented"); }
    virtual void visit(SPARQL::ExprStrDT&)          { throw LogicException("visit SPARQL::ExprStrDT not implemented"); }
    virtual void visit(SPARQL::ExprStrEnds&)        { throw LogicException("visit SPARQL::ExprStrEnds not implemented"); }
    virtual void visit(SPARQL::ExprStrLang&)        { throw LogicException("visit SPARQL::ExprStrLang not implemented"); }
    virtual void visit(SPARQL::ExprStrLen&)         { throw LogicException("visit SPARQL::ExprStrLen not implemented"); }
    virtual void visit(SPARQL::ExprStrStarts&)      { throw LogicException("visit SPARQL::ExprStrStarts not implemented"); }
    virtual void visit(SPARQL::ExprStrUUID&)        { throw LogicException("visit SPARQL::ExprStrUUID not implemented"); }
    virtual void visit(SPARQL::ExprStr&)            { throw LogicException("visit SPARQL::ExprStr not implemented"); }
    virtual void visit(SPARQL::ExprSubStr&)         { throw LogicException("visit SPARQL::ExprSubStr not implemented"); }
    virtual void visit(SPARQL::ExprTimezone&)       { throw LogicException("visit SPARQL::ExprTimezone not implemented"); }
    virtual void visit(SPARQL::ExprTZ&)             { throw LogicException("visit SPARQL::ExprTZ not implemented"); }
    virtual void visit(SPARQL::ExprUCase&)          { throw LogicException("visit SPARQL::ExprUCase not implemented"); }
    virtual void visit(SPARQL::ExprURI&)            { throw LogicException("visit SPARQL::ExprURI not implemented"); }
    virtual void visit(SPARQL::ExprUUID&)           { throw LogicException("visit SPARQL::ExprUUID not implemented"); }
    virtual void visit(SPARQL::ExprYear&)           { throw LogicException("visit SPARQL::ExprYear not implemented"); }
    virtual void visit(SPARQL::ExprCast&)           { throw LogicException("visit SPARQL::ExprCast not implemented"); }

    virtual void visit(SPARQL::ExprCosineSimilarity&)  { throw LogicException("visit SPARQL::ExprCosineSimilarity not implemented"); }
    virtual void visit(SPARQL::ExprDot&)               { throw LogicException("visit SPARQL::ExprDot not implemented"); }
    virtual void visit(SPARQL::ExprEuclideanDistance&) { throw LogicException("visit SPARQL::ExprEuclideanDistance not implemented"); }
    virtual void visit(SPARQL::ExprManhattanDistance&) { throw LogicException("visit SPARQL::ExprManhattanDistance not implemented"); }
    virtual void visit(SPARQL::ExprPow&)               { throw LogicException("visit SPARQL::ExprPow not implemented"); }
    virtual void visit(SPARQL::ExprSqrt&)              { throw LogicException("visit SPARQL::ExprSqrt not implemented"); }
    virtual void visit(SPARQL::ExprSum&)               { throw LogicException("visit SPARQL::ExprSum not implemented"); }

    // GQL
    virtual void visit(GQL::ExprAnd&) { throw LogicException("visit GQL::ExprAnd not implemented"); }
    virtual void visit(GQL::ExprOr&) { throw LogicException("visit GQL::ExprOr not implemented"); }
    virtual void visit(GQL::ExprNot&) { throw LogicException("visit GQL::ExprNot not implemented"); }
    virtual void visit(GQL::ExprHasNodeLabel&) { throw LogicException("visit GQL::ExprHasNodeLabel not implemented"); }
    virtual void visit(GQL::ExprHasEdgeLabel&) { throw LogicException("visit GQL::ExprHasEdgeLabel not implemented"); }
    virtual void visit(GQL::ExprEquals&) { throw LogicException("visit GQL::ExprEquals not implemented"); }
    virtual void visit(GQL::ExprVar&) { throw LogicException("visit GQL::ExprEquals not implemented"); }
    virtual void visit(GQL::ExprProperty&) { throw LogicException("visit GQL::ExprEquals not implemented"); }
    virtual void visit(GQL::ExprXor&) { throw LogicException("visit GQL::NotXor not implemented"); }
    virtual void visit(GQL::ExprConcat&) { throw LogicException("visit GQL::ExprConcat not implemented"); }
    virtual void visit(GQL::ExprGreaterOrEquals&) { throw LogicException("visit GQL::ExprGreaterOrEquals not implemented"); }
    virtual void visit(GQL::ExprGreater&) { throw LogicException("visit GQL::ExprGreater not implemented"); }
    virtual void visit(GQL::ExprIs&) { throw LogicException("visit GQL::ExprIs not implemented"); }
    virtual void visit(GQL::ExprLessOrEquals&) { throw LogicException("visit GQL::ExprLessOrEquals not implemented"); }
    virtual void visit(GQL::ExprLess&) { throw LogicException("visit GQL::ExprLess not implemented"); }
    virtual void visit(GQL::ExprNotEquals&) { throw LogicException("visit GQL::ExprNotEquals not implemented"); }
    virtual void visit(GQL::ExprTerm&) { throw LogicException("visit GQL::ExprTerm not implemented"); }
    virtual void visit(GQL::ExprAddition&) { throw LogicException("visit GQL::ExprAddition not implemented"); }
    virtual void visit(GQL::ExprSubtraction&) { throw LogicException("visit GQL::ExprSubtraction not implemented"); }
    virtual void visit(GQL::ExprMultiplication&) { throw LogicException("visit GQL::ExprMultiplication not implemented"); }
    virtual void visit(GQL::ExprDivision&) { throw LogicException("visit GQL::ExprDivision not implemented"); }
    virtual void visit(GQL::ExprAbs&) { throw LogicException("visit GQL::ExprAbs not implemented"); }
    virtual void visit(GQL::ExprModulo&) { throw LogicException("visit GQL::ExprModulo not implemented"); }
    virtual void visit(GQL::ExprSin&) { throw LogicException("visit GQL::ExprSin not implemented"); }
    virtual void visit(GQL::ExprCos&) { throw LogicException("visit GQL::ExprCos not implemented"); }
    virtual void visit(GQL::ExprTan&) { throw LogicException("visit GQL::ExprTan not implemented"); }
    virtual void visit(GQL::ExprCot&) { throw LogicException("visit GQL::ExprCot not implemented"); }
    virtual void visit(GQL::ExprSinh&) { throw LogicException("visit GQL::ExprSinh not implemented"); }
    virtual void visit(GQL::ExprCosh&) { throw LogicException("visit GQL::ExprCosh not implemented"); }
    virtual void visit(GQL::ExprTanh&) { throw LogicException("visit GQL::ExprTanh not implemented"); }
    virtual void visit(GQL::ExprAsin&) { throw LogicException("visit GQL::ExprAsin not implemented"); }
    virtual void visit(GQL::ExprAcos&) { throw LogicException("visit GQL::ExprAcos not implemented"); }
    virtual void visit(GQL::ExprAtan&) { throw LogicException("visit GQL::ExprAtan not implemented"); }
    virtual void visit(GQL::ExprDegrees&) { throw LogicException("visit GQL::ExprDegrees not implemented"); }
    virtual void visit(GQL::ExprRadians&) { throw LogicException("visit GQL::ExprRadians not implemented"); }
    virtual void visit(GQL::ExprLog&) { throw LogicException("visit GQL::ExprLog not implemented"); }
    virtual void visit(GQL::ExprLog10&) { throw LogicException("visit GQL::ExprLog10 not implemented"); }
    virtual void visit(GQL::ExprLn&) { throw LogicException("visit GQL::ExprLn not implemented"); }
    virtual void visit(GQL::ExprExp&) { throw LogicException("visit GQL::ExprExp not implemented"); }
    virtual void visit(GQL::ExprPower&) { throw LogicException("visit GQL::ExprPower not implemented"); }
    virtual void visit(GQL::ExprSqrt&) { throw LogicException("visit GQL::ExprSqrt not implemented"); }
    virtual void visit(GQL::ExprFloor&) { throw LogicException("visit GQL::ExprFloor not implemented"); }
    virtual void visit(GQL::ExprCeil&) { throw LogicException("visit GQL::ExprCeil not implemented"); }
    virtual void visit(GQL::ExprLength&) { throw LogicException("visit GQL::ExprLength not implemented"); }
    virtual void visit(GQL::ExprSubStr&) { throw LogicException("visit GQL::ExprSubStr not implemented"); }
    virtual void visit(GQL::ExprFold&) { throw LogicException("visit GQL::ExprFold not implemented"); }
    virtual void visit(GQL::ExprSingleTrim&) { throw LogicException("visit GQL::ExprSingleTrim not implemented"); }
    virtual void visit(GQL::ExprMultiTrim&) { throw LogicException("visit GQL::ExprMultiTrim not implemented"); }
    virtual void visit(GQL::ExprNormalize&) { throw LogicException("visit GQL::ExprNormalize not implemented"); }
    virtual void visit(GQL::ExprNullIf&) { throw LogicException("visit GQL::ExprNullIf not implemented"); }
    virtual void visit(GQL::ExprCoalesce&) { throw LogicException("visit GQL::ExprCoalesce not implemented"); }
    virtual void visit(GQL::ExprSimpleCase&) { throw LogicException("visit GQL::ExprSimpleCase not implemented"); }
    virtual void visit(GQL::ExprSearchedCase&) { throw LogicException("visit GQL::ExprSearchedCase not implemented"); }
    virtual void visit(GQL::ExprCast&) { throw LogicException("visit GQL::ExprCast not implemented"); }
    virtual void visit(GQL::ExprAggCountAll&) { throw LogicException("visit GQL::ExprAggCountAll not implemented"); }
    virtual void visit(GQL::ExprAggCount&) { throw LogicException("visit GQL::ExprAggCount not implemented"); }
    virtual void visit(GQL::ExprAggAvg&) { throw LogicException("visit GQL::ExprAggAvg not implemented"); }
    virtual void visit(GQL::ExprAggMax&) { throw LogicException("visit GQL::ExprAggMax not implemented"); }
    virtual void visit(GQL::ExprAggMin&) { throw LogicException("visit GQL::ExprAggMin not implemented"); }
    virtual void visit(GQL::ExprAggSum&) { throw LogicException("visit GQL::ExprAggSum not implemented"); }
    virtual void visit(GQL::ExprAggStddevPop&) { throw LogicException("visit GQL::ExprAggStddevPop not implemented"); }
    virtual void visit(GQL::ExprAggStddevSamp&) { throw LogicException("visit GQL::ExprAggStddevSamp not implemented"); }
    virtual void visit(GQL::ExprAggCollect&) { throw LogicException("visit GQL::ExprAggCollect not implemented"); }
    virtual void visit(GQL::ExprAggPercentileCont&) { throw LogicException("visit GQL::ExprAggPercentileCont not implemented"); }
    virtual void visit(GQL::ExprAggPercentileDisc&) { throw LogicException("visit GQL::ExprAggPercentileDisc not implemented"); }
};
