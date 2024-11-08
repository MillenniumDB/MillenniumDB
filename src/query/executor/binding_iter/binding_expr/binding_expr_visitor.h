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
} // namespace SPARQL

class BindingExprVisitor {
public:

    virtual void visit(BindingExprTerm&) = 0;
    virtual void visit(BindingExprVar&) = 0;

    virtual void visit(MQL::BindingExprAddition&)          { throw LogicException("visits MQL::BindingExprAddition not implemented"); }
    virtual void visit(MQL::BindingExprAnd&)               { throw LogicException("visits MQL::BindingExprAnd not implemented"); }
    virtual void visit(MQL::BindingExprDivision&)          { throw LogicException("visits MQL::BindingExprDivision not implemented"); }
    virtual void visit(MQL::BindingExprEquals&)            { throw LogicException("visits MQL::BindingExprEquals not implemented"); }
    virtual void visit(MQL::BindingExprIs&)                { throw LogicException("visits MQL::BindingExprIs not implemented"); }
    virtual void visit(MQL::BindingExprLessOrEquals&)      { throw LogicException("visits MQL::BindingExprLessOrEquals not implemented"); }
    virtual void visit(MQL::BindingExprLess&)              { throw LogicException("visits MQL::BindingExprLess not implemented"); }
    virtual void visit(MQL::BindingExprModulo&)            { throw LogicException("visits MQL::BindingExprModulo not implemented"); }
    virtual void visit(MQL::BindingExprMultiplication&)    { throw LogicException("visits MQL::BindingExprMultiplication not implemented"); }
    virtual void visit(MQL::BindingExprNotEquals&)         { throw LogicException("visits MQL::BindingExprNotEquals not implemented"); }
    virtual void visit(MQL::BindingExprNot&)               { throw LogicException("visits MQL::BindingExprNot not implemented"); }
    virtual void visit(MQL::BindingExprOr&)                { throw LogicException("visits MQL::BindingExprOr not implemented"); }
    virtual void visit(MQL::BindingExprSubtraction&)       { throw LogicException("visits MQL::BindingExprSubtraction not implemented"); }
    virtual void visit(MQL::BindingExprUnaryMinus&)        { throw LogicException("visits MQL::BindingExprUnaryMinus not implemented"); }
    virtual void visit(MQL::BindingExprUnaryPlus&)         { throw LogicException("visits MQL::BindingExprUnaryPlus not implemented"); }
    virtual void visit(MQL::BindingExprRegex&)             { throw LogicException("visits MQL::BindingExprRegex not implemented"); }

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
};