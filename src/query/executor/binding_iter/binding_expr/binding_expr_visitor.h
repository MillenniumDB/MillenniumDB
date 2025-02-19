#pragma once

#include "query/exceptions.h"

class BindingExprVar;
class BindingExprTerm;

namespace MQL {
class BindingExprAddition;
class BindingExprAnd;
class BindingExprDivision;
class BindingExprEquals;
class BindingExprIs;
class BindingExprLessOrEquals;
class BindingExprLess;
class BindingExprModulo;
class BindingExprMultiplication;
class BindingExprNotEquals;
class BindingExprNot;
class BindingExprOr;
class BindingExprSubtraction;
class BindingExprUnaryMinus;
class BindingExprUnaryPlus;
class BindingExprRegex;
class BindingExprTensorDistance;
} // namespace MQL

namespace SPARQL {
class BindingExprAbs;
class BindingExprAddition;
class BindingExprAnd;
class BindingExprBNode;
class BindingExprBound;
class BindingExprCast;
class BindingExprCeil;
class BindingExprCoalesce;
class BindingExprConcat;
class BindingExprContains;
class BindingExprDatatype;
class BindingExprDay;
class BindingExprDivision;
class BindingExprEncodeForUri;
class BindingExprEquals;
class BindingExprExists;
class BindingExprFloor;
class BindingExprGreaterOrEqual;
class BindingExprGreater;
class BindingExprHours;
class BindingExprIf;
class BindingExprIn;
class BindingExprIRI;
class BindingExprIsBlank;
class BindingExprIsIRI;
class BindingExprIsLiteral;
class BindingExprIsNumeric;
class BindingExprLangMatches;
class BindingExprLang;
class BindingExprLCase;
class BindingExprLessOrEqual;
class BindingExprLess;
class BindingExprMD5;
class BindingExprMinutes;
class BindingExprMonth;
class BindingExprMultiplication;
class BindingExprNotExists;
class BindingExprNot;
class BindingExprOr;
class BindingExprRegex;
class BindingExprReplace;
class BindingExprRound;
class BindingExprSameTerm;
class BindingExprSeconds;
class BindingExprSHA1;
class BindingExprSHA256;
class BindingExprSHA384;
class BindingExprSHA512;
class BindingExprStrAfter;
class BindingExprStrBefore;
class BindingExprStrDT;
class BindingExprStrEnds;
class BindingExprStrLang;
class BindingExprStrLen;
class BindingExprStrStarts;
class BindingExprStr;
class BindingExprSubStr;
class BindingExprSubtraction;
class BindingExprTimezone;
class BindingExprTZ;
class BindingExprUCase;
class BindingExprUnaryMinus;
class BindingExprUnaryPlus;
class BindingExprYear;

class BindingExprCosineSimilarity;
class BindingExprDot;
class BindingExprEuclideanDistance;
class BindingExprManhattanDistance;
class BindingExprPow;
class BindingExprSqrt;
class BindingExprSum;
} // namespace SPARQL

namespace GQL {
class BindingExprOr;
class BindingExprAnd;
class BindingExprNot;
class BindingExprHasNodeLabel;
class BindingExprHasEdgeLabel;
class BindingExprEquals;
class BindingExprProperty;
class BindingExprConcat;
class BindingExprGreaterOrEquals;
class BindingExprGreater;
class BindingExprIs;
class BindingExprLessOrEquals;
class BindingExprLess;
class BindingExprNotEquals;
class BindingExprXor;
class BindingExprAddition;
class BindingExprSubtraction;
class BindingExprMultiplication;
class BindingExprDivision;
class BindingExprAbs;
class BindingExprModulo;
class BindingExprSin;
class BindingExprCos;
class BindingExprTan;
class BindingExprCot;
class BindingExprSinh;
class BindingExprCosh;
class BindingExprTanh;
class BindingExprAsin;
class BindingExprAcos;
class BindingExprAtan;
class BindingExprDegrees;
class BindingExprRadians;
class BindingExprLog;
class BindingExprLog10;
class BindingExprLn;
class BindingExprExp;
class BindingExprPower;
class BindingExprSqrt;
class BindingExprFloor;
class BindingExprCeil;
class BindingExprLength;
class BindingExprSubStr;
class BindingExprFold;
class BindingExprSingleTrim;
class BindingExprMultiTrim;
class BindingExprNormalize;
class BindingExprNullIf;
class BindingExprCoalesce;
class BindingExprSimpleCase;
class BindingExprSearchedCase;
class BindingExprCast;
} // namespace GQL

class BindingExprVisitor {
public:

    virtual void visit(BindingExprTerm&) = 0;
    virtual void visit(BindingExprVar&) = 0;

    virtual void visit(MQL::BindingExprAddition&)       { throw LogicException("visits MQL::BindingExprAddition not implemented"); }
    virtual void visit(MQL::BindingExprAnd&)            { throw LogicException("visits MQL::BindingExprAnd not implemented"); }
    virtual void visit(MQL::BindingExprDivision&)       { throw LogicException("visits MQL::BindingExprDivision not implemented"); }
    virtual void visit(MQL::BindingExprEquals&)         { throw LogicException("visits MQL::BindingExprEquals not implemented"); }
    virtual void visit(MQL::BindingExprIs&)             { throw LogicException("visits MQL::BindingExprIs not implemented"); }
    virtual void visit(MQL::BindingExprLessOrEquals&)   { throw LogicException("visits MQL::BindingExprLessOrEquals not implemented"); }
    virtual void visit(MQL::BindingExprLess&)           { throw LogicException("visits MQL::BindingExprLess not implemented"); }
    virtual void visit(MQL::BindingExprModulo&)         { throw LogicException("visits MQL::BindingExprModulo not implemented"); }
    virtual void visit(MQL::BindingExprMultiplication&) { throw LogicException("visits MQL::BindingExprMultiplication not implemented"); }
    virtual void visit(MQL::BindingExprNotEquals&)      { throw LogicException("visits MQL::BindingExprNotEquals not implemented"); }
    virtual void visit(MQL::BindingExprNot&)            { throw LogicException("visits MQL::BindingExprNot not implemented"); }
    virtual void visit(MQL::BindingExprOr&)             { throw LogicException("visits MQL::BindingExprOr not implemented"); }
    virtual void visit(MQL::BindingExprSubtraction&)    { throw LogicException("visits MQL::BindingExprSubtraction not implemented"); }
    virtual void visit(MQL::BindingExprUnaryMinus&)     { throw LogicException("visits MQL::BindingExprUnaryMinus not implemented"); }
    virtual void visit(MQL::BindingExprUnaryPlus&)      { throw LogicException("visits MQL::BindingExprUnaryPlus not implemented"); }
    virtual void visit(MQL::BindingExprRegex&)          { throw LogicException("visits MQL::BindingExprRegex not implemented"); }
    virtual void visit(MQL::BindingExprTensorDistance&) { throw LogicException("visits MQL::BindingExprTensorDistance not implemented"); }

    virtual void visit(SPARQL::BindingExprAbs&)            { throw LogicException("visit SPARQL::BindingExprAbs not implemented"); }
    virtual void visit(SPARQL::BindingExprAddition&)       { throw LogicException("visit SPARQL::BindingExprAddition not implemented"); }
    virtual void visit(SPARQL::BindingExprAnd&)            { throw LogicException("visit SPARQL::BindingExprAnd not implemented"); }
    virtual void visit(SPARQL::BindingExprBNode&)          { throw LogicException("visit SPARQL::BindingExprBNode not implemented"); }
    virtual void visit(SPARQL::BindingExprBound&)          { throw LogicException("visit SPARQL::BindingExprBound not implemented"); }
    virtual void visit(SPARQL::BindingExprCast&)           { throw LogicException("visit SPARQL::BindingExprCast not implemented"); }
    virtual void visit(SPARQL::BindingExprCeil&)           { throw LogicException("visit SPARQL::BindingExprCeil not implemented"); }
    virtual void visit(SPARQL::BindingExprCoalesce&)       { throw LogicException("visit SPARQL::BindingExprCoalesce not implemented"); }
    virtual void visit(SPARQL::BindingExprConcat&)         { throw LogicException("visit SPARQL::BindingExprConcat not implemented"); }
    virtual void visit(SPARQL::BindingExprContains&)       { throw LogicException("visit SPARQL::BindingExprContains not implemented"); }
    virtual void visit(SPARQL::BindingExprDatatype&)       { throw LogicException("visit SPARQL::BindingExprDatatype not implemented"); }
    virtual void visit(SPARQL::BindingExprDay&)            { throw LogicException("visit SPARQL::BindingExprDay not implemented"); }
    virtual void visit(SPARQL::BindingExprDivision&)       { throw LogicException("visit SPARQL::BindingExprDivision not implemented"); }
    virtual void visit(SPARQL::BindingExprEncodeForUri&)   { throw LogicException("visit SPARQL::BindingExprEncodeForUri not implemented"); }
    virtual void visit(SPARQL::BindingExprEquals&)         { throw LogicException("visit SPARQL::BindingExprEquals not implemented"); }
    virtual void visit(SPARQL::BindingExprExists&)         { throw LogicException("visit SPARQL::BindingExprExists not implemented"); }
    virtual void visit(SPARQL::BindingExprFloor&)          { throw LogicException("visit SPARQL::BindingExprFloor not implemented"); }
    virtual void visit(SPARQL::BindingExprGreaterOrEqual&) { throw LogicException("visit SPARQL::BindingExprGreaterOrEqual not implemented"); }
    virtual void visit(SPARQL::BindingExprGreater&)        { throw LogicException("visit SPARQL::BindingExprGreater not implemented"); }
    virtual void visit(SPARQL::BindingExprHours&)          { throw LogicException("visit SPARQL::BindingExprHours not implemented"); }
    virtual void visit(SPARQL::BindingExprIf&)             { throw LogicException("visit SPARQL::BindingExprIf not implemented"); }
    virtual void visit(SPARQL::BindingExprIn&)             { throw LogicException("visit SPARQL::BindingExprIn not implemented"); }
    virtual void visit(SPARQL::BindingExprIRI&)            { throw LogicException("visit SPARQL::BindingExprIRI not implemented"); }
    virtual void visit(SPARQL::BindingExprIsBlank&)        { throw LogicException("visit SPARQL::BindingExprIsBlank not implemented"); }
    virtual void visit(SPARQL::BindingExprIsIRI&)          { throw LogicException("visit SPARQL::BindingExprIsIRI not implemented"); }
    virtual void visit(SPARQL::BindingExprIsLiteral&)      { throw LogicException("visit SPARQL::BindingExprIsLiteral not implemented"); }
    virtual void visit(SPARQL::BindingExprIsNumeric&)      { throw LogicException("visit SPARQL::BindingExprIsNumeric not implemented"); }
    virtual void visit(SPARQL::BindingExprLangMatches&)    { throw LogicException("visit SPARQL::BindingExprLangMatches not implemented"); }
    virtual void visit(SPARQL::BindingExprLang&)           { throw LogicException("visit SPARQL::BindingExprLang not implemented"); }
    virtual void visit(SPARQL::BindingExprLCase&)          { throw LogicException("visit SPARQL::BindingExprLCase not implemented"); }
    virtual void visit(SPARQL::BindingExprLessOrEqual&)    { throw LogicException("visit SPARQL::BindingExprLessOrEqual not implemented"); }
    virtual void visit(SPARQL::BindingExprLess&)           { throw LogicException("visit SPARQL::BindingExprLess not implemented"); }
    virtual void visit(SPARQL::BindingExprMD5&)            { throw LogicException("visit SPARQL::BindingExprMD5 not implemented"); }
    virtual void visit(SPARQL::BindingExprMinutes&)        { throw LogicException("visit SPARQL::BindingExprMinutes not implemented"); }
    virtual void visit(SPARQL::BindingExprMonth&)          { throw LogicException("visit SPARQL::BindingExprMonth not implemented"); }
    virtual void visit(SPARQL::BindingExprMultiplication&) { throw LogicException("visit SPARQL::BindingExprMultiplication not implemented"); }
    virtual void visit(SPARQL::BindingExprNotExists&)      { throw LogicException("visit SPARQL::BindingExprNotExists not implemented"); }
    virtual void visit(SPARQL::BindingExprNot&)            { throw LogicException("visit SPARQL::BindingExprNot not implemented"); }
    virtual void visit(SPARQL::BindingExprOr&)             { throw LogicException("visit SPARQL::BindingExprOr not implemented"); }
    virtual void visit(SPARQL::BindingExprRegex&)          { throw LogicException("visit SPARQL::BindingExprRegex not implemented"); }
    virtual void visit(SPARQL::BindingExprReplace&)        { throw LogicException("visit SPARQL::BindingExprReplace not implemented"); }
    virtual void visit(SPARQL::BindingExprRound&)          { throw LogicException("visit SPARQL::BindingExprRound not implemented"); }
    virtual void visit(SPARQL::BindingExprSameTerm&)       { throw LogicException("visit SPARQL::BindingExprSameTerm not implemented"); }
    virtual void visit(SPARQL::BindingExprSeconds&)        { throw LogicException("visit SPARQL::BindingExprSeconds not implemented"); }
    virtual void visit(SPARQL::BindingExprSHA1&)           { throw LogicException("visit SPARQL::BindingExprSHA1 not implemented"); }
    virtual void visit(SPARQL::BindingExprSHA256&)         { throw LogicException("visit SPARQL::BindingExprSHA256 not implemented"); }
    virtual void visit(SPARQL::BindingExprSHA384&)         { throw LogicException("visit SPARQL::BindingExprSHA384 not implemented"); }
    virtual void visit(SPARQL::BindingExprSHA512&)         { throw LogicException("visit SPARQL::BindingExprSHA512 not implemented"); }
    virtual void visit(SPARQL::BindingExprStrAfter&)       { throw LogicException("visit SPARQL::BindingExprStrAfter not implemented"); }
    virtual void visit(SPARQL::BindingExprStrBefore&)      { throw LogicException("visit SPARQL::BindingExprStrBefore not implemented"); }
    virtual void visit(SPARQL::BindingExprStrDT&)          { throw LogicException("visit SPARQL::BindingExprStrDT not implemented"); }
    virtual void visit(SPARQL::BindingExprStrEnds&)        { throw LogicException("visit SPARQL::BindingExprStrEnds not implemented"); }
    virtual void visit(SPARQL::BindingExprStrLang&)        { throw LogicException("visit SPARQL::BindingExprStrLang not implemented"); }
    virtual void visit(SPARQL::BindingExprStrLen&)         { throw LogicException("visit SPARQL::BindingExprStrLen not implemented"); }
    virtual void visit(SPARQL::BindingExprStrStarts&)      { throw LogicException("visit SPARQL::BindingExprStrStarts not implemented"); }
    virtual void visit(SPARQL::BindingExprStr&)            { throw LogicException("visit SPARQL::BindingExprStr not implemented"); }
    virtual void visit(SPARQL::BindingExprSubStr&)         { throw LogicException("visit SPARQL::BindingExprSubStr not implemented"); }
    virtual void visit(SPARQL::BindingExprSubtraction&)    { throw LogicException("visit SPARQL::BindingExprSubtraction not implemented"); }
    virtual void visit(SPARQL::BindingExprTimezone&)       { throw LogicException("visit SPARQL::BindingExprTimezone not implemented"); }
    virtual void visit(SPARQL::BindingExprTZ&)             { throw LogicException("visit SPARQL::BindingExprTZ not implemented"); }
    virtual void visit(SPARQL::BindingExprUCase&)          { throw LogicException("visit SPARQL::BindingExprUCase not implemented"); }
    virtual void visit(SPARQL::BindingExprUnaryMinus&)     { throw LogicException("visit SPARQL::BindingExprUnaryMinus not implemented"); }
    virtual void visit(SPARQL::BindingExprUnaryPlus&)      { throw LogicException("visit SPARQL::BindingExprUnaryPlus not implemented"); }
    virtual void visit(SPARQL::BindingExprYear&)           { throw LogicException("visit SPARQL::BindingExprYear not implemented"); }

    virtual void visit(SPARQL::BindingExprCosineSimilarity&)  { throw LogicException("visit SPARQL::BindingExprCosineSimilarity not implemented"); }
    virtual void visit(SPARQL::BindingExprDot&)               { throw LogicException("visit SPARQL::BindingExprDot not implemented"); }
    virtual void visit(SPARQL::BindingExprEuclideanDistance&) { throw LogicException("visit SPARQL::BindingExprEuclideanDistance not implemented"); }
    virtual void visit(SPARQL::BindingExprManhattanDistance&) { throw LogicException("visit SPARQL::BindingExprManhattanDistance not implemented"); }
    virtual void visit(SPARQL::BindingExprPow&)               { throw LogicException("visit SPARQL::BindingExprPow not implemented"); }
    virtual void visit(SPARQL::BindingExprSqrt&)              { throw LogicException("visit SPARQL::BindingExprSqrt not implemented"); }
    virtual void visit(SPARQL::BindingExprSum&)               { throw LogicException("visit SPARQL::BindingExprSum not implemented"); }

    virtual void visit(GQL::BindingExprOr&) { throw LogicException("visit GQL::BindingExprOr not implemented"); }
    virtual void visit(GQL::BindingExprAnd&) { throw LogicException("visit GQL::BindingExprAnd not implemented"); }
    virtual void visit(GQL::BindingExprNot&) { throw LogicException("visit GQL::BindingExprNot not implemented"); }
    virtual void visit(GQL::BindingExprHasNodeLabel&) { throw LogicException("visit GQL::BindingExprHasNodeLabel not implemented"); }
    virtual void visit(GQL::BindingExprHasEdgeLabel&) { throw LogicException("visit GQL::BindingExprHasEdgeLabel not implemented"); }
    virtual void visit(GQL::BindingExprEquals&) { throw LogicException("visit GQL::BindingExprEquals not implemented"); }
    virtual void visit(GQL::BindingExprProperty&) { throw LogicException("visit GQL::BindingExprProperty not implemented"); }
    virtual void visit(GQL::BindingExprConcat&) { throw LogicException("visit GQL::BindingExprConcat not implemented"); }
    virtual void visit(GQL::BindingExprGreaterOrEquals&) { throw LogicException("visit GQL::BindingExprGreaterOrEquals not implemented"); }
    virtual void visit(GQL::BindingExprGreater&) { throw LogicException("visit GQL::BindingExprGreater not implemented"); }
    virtual void visit(GQL::BindingExprIs&) { throw LogicException("visit GQL::BindingExprIs not implemented"); }
    virtual void visit(GQL::BindingExprLessOrEquals&) { throw LogicException("visit GQL::BindingExprLessOrEquals not implemented"); }
    virtual void visit(GQL::BindingExprLess&) { throw LogicException("visit GQL::BindingExprLess not implemented"); }
    virtual void visit(GQL::BindingExprNotEquals&) { throw LogicException("visit GQL::BindingExprNotEquals not implemented"); }
    virtual void visit(GQL::BindingExprXor&) { throw LogicException("visit GQL::BindingExprXor not implemented"); }
    virtual void visit(GQL::BindingExprAddition&) { throw LogicException("visit GQL::BindingExprAddition not implemented"); }
    virtual void visit(GQL::BindingExprSubtraction&) { throw LogicException("visit GQL::BindingExprSubtraction not implemented"); }
    virtual void visit(GQL::BindingExprMultiplication&) { throw LogicException("visit GQL::BindingExprMultiplication not implemented"); }
    virtual void visit(GQL::BindingExprDivision&) { throw LogicException("visit GQL::BindingExprDivision not implemented"); }
    virtual void visit(GQL::BindingExprAbs&) { throw LogicException("visit GQL::BindingExprAbs not implemented"); }
    virtual void visit(GQL::BindingExprModulo&) { throw LogicException("visit GQL::BindingExprModulo not implemented"); }
    virtual void visit(GQL::BindingExprSin&) { throw LogicException("visit GQL::BindingExprSin not implemented"); }
    virtual void visit(GQL::BindingExprCos&) { throw LogicException("visit GQL::BindingExprCos not implemented"); }
    virtual void visit(GQL::BindingExprTan&) { throw LogicException("visit GQL::BindingExprTan not implemented"); }
    virtual void visit(GQL::BindingExprCot&) { throw LogicException("visit GQL::BindingExprCot not implemented"); }
    virtual void visit(GQL::BindingExprSinh&) { throw LogicException("visit GQL::BindingExprSinh not implemented"); }
    virtual void visit(GQL::BindingExprCosh&) { throw LogicException("visit GQL::BindingExprCosh not implemented"); }
    virtual void visit(GQL::BindingExprTanh&) { throw LogicException("visit GQL::BindingExprTanh not implemented"); }
    virtual void visit(GQL::BindingExprAsin&) { throw LogicException("visit GQL::BindingExprAsin not implemented"); }
    virtual void visit(GQL::BindingExprAcos&) { throw LogicException("visit GQL::BindingExprAcos not implemented"); }
    virtual void visit(GQL::BindingExprAtan&) { throw LogicException("visit GQL::BindingExprAtan not implemented"); }
    virtual void visit(GQL::BindingExprDegrees&) { throw LogicException("visit GQL::BindingExprDegrees not implemented"); }
    virtual void visit(GQL::BindingExprRadians&) { throw LogicException("visit GQL::BindingExprRadians not implemented"); }
    virtual void visit(GQL::BindingExprLog&) { throw LogicException("visit GQL::BindingExprLog not implemented"); }
    virtual void visit(GQL::BindingExprLog10&) { throw LogicException("visit GQL::BindingExprLog10 not implemented"); }
    virtual void visit(GQL::BindingExprLn&) { throw LogicException("visit GQL::BindingExprLn not implemented"); }
    virtual void visit(GQL::BindingExprExp&) { throw LogicException("visit GQL::BindingExprExp not implemented"); }
    virtual void visit(GQL::BindingExprPower&) { throw LogicException("visit GQL::BindingExprPower not implemented"); }
    virtual void visit(GQL::BindingExprSqrt&) { throw LogicException("visit GQL::BindingExprSqrt not implemented"); }
    virtual void visit(GQL::BindingExprFloor&) { throw LogicException("visit GQL::BindingExprFloor not implemented"); }
    virtual void visit(GQL::BindingExprCeil&) { throw LogicException("visit GQL::BindingExprCeil not implemented"); }
    virtual void visit(GQL::BindingExprLength&) { throw LogicException("visit GQL::BindingExprLength not implemented"); }
    virtual void visit(GQL::BindingExprSubStr&) { throw LogicException("visit GQL::BindingExprSubString not implemented"); }
    virtual void visit(GQL::BindingExprFold&) { throw LogicException("visit GQL::BindingExprFold not implemented"); }
    virtual void visit(GQL::BindingExprSingleTrim&) { throw LogicException("visit GQL::BindingExprSingleTrim not implemented"); }
    virtual void visit(GQL::BindingExprMultiTrim&) { throw LogicException("visit GQL::BindingExprMultiTrim not implemented"); }
    virtual void visit(GQL::BindingExprNormalize&) { throw LogicException("visit GQL::BindingExprNormalize not implemented"); }
    virtual void visit(GQL::BindingExprNullIf&) { throw LogicException("visit GQL::BindingExprNullIf not implemented"); }
    virtual void visit(GQL::BindingExprCoalesce&) { throw LogicException("visit GQL::BindingExprCoalesce not implemented"); }
    virtual void visit(GQL::BindingExprSimpleCase&) { throw LogicException("visit GQL::BindingExprSimpleCase not implemented"); }
    virtual void visit(GQL::BindingExprSearchedCase&) { throw LogicException("visit GQL::BindingExprSearchedCase not implemented"); }
    virtual void visit(GQL::BindingExprCast&) { throw LogicException("visit GQL::BindingExprCast not implemented"); }
};
