#pragma once

#include "query/exceptions.h"

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

class ExprCosineDistance;
class ExprCosineSimilarity;
class ExprDot;
class ExprEuclideanDistance;
class ExprManhattanDistance;
class ExprPow;
class ExprSqrt;
class ExprSum;

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    virtual void visit(ExprVar&)
    {
        throw LogicException("visit SPARQL::ExprVar not implemented");
    }
    virtual void visit(ExprNot&)
    {
        throw LogicException("visit SPARQL::ExprNot not implemented");
    }
    virtual void visit(ExprUnaryMinus&)
    {
        throw LogicException("visit SPARQL::ExprUnaryMinus not implemented");
    }
    virtual void visit(ExprUnaryPlus&)
    {
        throw LogicException("visit SPARQL::ExprUnaryPlus not implemented");
    }
    virtual void visit(ExprMultiplication&)
    {
        throw LogicException("visit SPARQL::ExprMultiplication not implemented");
    }
    virtual void visit(ExprDivision&)
    {
        throw LogicException("visit SPARQL::ExprDivision not implemented");
    }
    virtual void visit(ExprAddition&)
    {
        throw LogicException("visit SPARQL::ExprAddition not implemented");
    }
    virtual void visit(ExprSubtraction&)
    {
        throw LogicException("visit SPARQL::ExprSubtraction not implemented");
    }
    virtual void visit(ExprAnd&)
    {
        throw LogicException("visit SPARQL::ExprAnd not implemented");
    }
    virtual void visit(ExprOr&)
    {
        throw LogicException("visit SPARQL::ExprOr not implemented");
    }
    virtual void visit(ExprLess&)
    {
        throw LogicException("visit SPARQL::ExprLess not implemented");
    }
    virtual void visit(ExprGreater&)
    {
        throw LogicException("visit SPARQL::ExprGreater not implemented");
    }
    virtual void visit(ExprEqual&)
    {
        throw LogicException("visit SPARQL::ExprEqual not implemented");
    }
    virtual void visit(ExprNotEqual&)
    {
        throw LogicException("visit SPARQL::ExprNotEqual not implemented");
    }
    virtual void visit(ExprLessOrEqual&)
    {
        throw LogicException("visit SPARQL::ExprLessOrEqual not implemented");
    }
    virtual void visit(ExprGreaterOrEqual&)
    {
        throw LogicException("visit SPARQL::ExprGreaterOrEqual not implemented");
    }
    virtual void visit(ExprTerm&)
    {
        throw LogicException("visit SPARQL::ExprTerm not implemented");
    }
    virtual void visit(ExprIn&)
    {
        throw LogicException("visit SPARQL::ExprIn not implemented");
    }
    virtual void visit(ExprNotIn&)
    {
        throw LogicException("visit SPARQL::ExprNotIn not implemented");
    }
    virtual void visit(ExprAggAvg&)
    {
        throw LogicException("visit SPARQL::ExprAggAvg not implemented");
    }
    virtual void visit(ExprAggCount&)
    {
        throw LogicException("visit SPARQL::ExprAggCount not implemented");
    }
    virtual void visit(ExprAggCountAll&)
    {
        throw LogicException("visit SPARQL::ExprAggCountAll not implemented");
    }
    virtual void visit(ExprAggGroupConcat&)
    {
        throw LogicException("visit SPARQL::ExprAggGroupConcat not implemented");
    }
    virtual void visit(ExprAggMax&)
    {
        throw LogicException("visit SPARQL::ExprAggMax not implemented");
    }
    virtual void visit(ExprAggMin&)
    {
        throw LogicException("visit SPARQL::ExprAggMin not implemented");
    }
    virtual void visit(ExprAggSample&)
    {
        throw LogicException("visit SPARQL::ExprAggSample not implemented");
    }
    virtual void visit(ExprAggSum&)
    {
        throw LogicException("visit SPARQL::ExprAggSum not implemented");
    }
    virtual void visit(ExprAbs&)
    {
        throw LogicException("visit SPARQL::ExprAbs not implemented");
    }
    virtual void visit(ExprBNode&)
    {
        throw LogicException("visit SPARQL::ExprBNode not implemented");
    }
    virtual void visit(ExprBound&)
    {
        throw LogicException("visit SPARQL::ExprBound not implemented");
    }
    virtual void visit(ExprCeil&)
    {
        throw LogicException("visit SPARQL::ExprCeil not implemented");
    }
    virtual void visit(ExprCoalesce&)
    {
        throw LogicException("visit SPARQL::ExprCoalesce not implemented");
    }
    virtual void visit(ExprConcat&)
    {
        throw LogicException("visit SPARQL::ExprConcat not implemented");
    }
    virtual void visit(ExprContains&)
    {
        throw LogicException("visit SPARQL::ExprContains not implemented");
    }
    virtual void visit(ExprDatatype&)
    {
        throw LogicException("visit SPARQL::ExprDatatype not implemented");
    }
    virtual void visit(ExprDay&)
    {
        throw LogicException("visit SPARQL::ExprDay not implemented");
    }
    virtual void visit(ExprEncodeForUri&)
    {
        throw LogicException("visit SPARQL::ExprEncodeForUri not implemented");
    }
    virtual void visit(ExprExists&)
    {
        throw LogicException("visit SPARQL::ExprExists not implemented");
    }
    virtual void visit(ExprFloor&)
    {
        throw LogicException("visit SPARQL::ExprFloor not implemented");
    }
    virtual void visit(ExprHours&)
    {
        throw LogicException("visit SPARQL::ExprHours not implemented");
    }
    virtual void visit(ExprIf&)
    {
        throw LogicException("visit SPARQL::ExprIf not implemented");
    }
    virtual void visit(ExprIRI&)
    {
        throw LogicException("visit SPARQL::ExprIRI not implemented");
    }
    virtual void visit(ExprIsBlank&)
    {
        throw LogicException("visit SPARQL::ExprIsBlank not implemented");
    }
    virtual void visit(ExprIsIRI&)
    {
        throw LogicException("visit SPARQL::ExprIsIRI not implemented");
    }
    virtual void visit(ExprIsLiteral&)
    {
        throw LogicException("visit SPARQL::ExprIsLiteral not implemented");
    }
    virtual void visit(ExprIsNumeric&)
    {
        throw LogicException("visit SPARQL::ExprIsNumeric not implemented");
    }
    virtual void visit(ExprIsURI&)
    {
        throw LogicException("visit SPARQL::ExprIsURI not implemented");
    }
    virtual void visit(ExprLang&)
    {
        throw LogicException("visit SPARQL::ExprLang not implemented");
    }
    virtual void visit(ExprLangMatches&)
    {
        throw LogicException("visit SPARQL::ExprLangMatches not implemented");
    }
    virtual void visit(ExprLCase&)
    {
        throw LogicException("visit SPARQL::ExprLCase not implemented");
    }
    virtual void visit(ExprMD5&)
    {
        throw LogicException("visit SPARQL::ExprMD5 not implemented");
    }
    virtual void visit(ExprMinutes&)
    {
        throw LogicException("visit SPARQL::ExprMinutes not implemented");
    }
    virtual void visit(ExprMonth&)
    {
        throw LogicException("visit SPARQL::ExprMonth not implemented");
    }
    virtual void visit(ExprNotExists&)
    {
        throw LogicException("visit SPARQL::ExprNotExists not implemented");
    }
    virtual void visit(ExprNow&)
    {
        throw LogicException("visit SPARQL::ExprNow not implemented");
    }
    virtual void visit(ExprRand&)
    {
        throw LogicException("visit SPARQL::ExprRand not implemented");
    }
    virtual void visit(ExprRegex&)
    {
        throw LogicException("visit SPARQL::ExprRegex not implemented");
    }
    virtual void visit(ExprReplace&)
    {
        throw LogicException("visit SPARQL::ExprReplace not implemented");
    }
    virtual void visit(ExprRound&)
    {
        throw LogicException("visit SPARQL::ExprRound not implemented");
    }
    virtual void visit(ExprSameTerm&)
    {
        throw LogicException("visit SPARQL::ExprSameTerm not implemented");
    }
    virtual void visit(ExprSeconds&)
    {
        throw LogicException("visit SPARQL::ExprSeconds not implemented");
    }
    virtual void visit(ExprSHA1&)
    {
        throw LogicException("visit SPARQL::ExprSHA1 not implemented");
    }
    virtual void visit(ExprSHA256&)
    {
        throw LogicException("visit SPARQL::ExprSHA256 not implemented");
    }
    virtual void visit(ExprSHA384&)
    {
        throw LogicException("visit SPARQL::ExprSHA384 not implemented");
    }
    virtual void visit(ExprSHA512&)
    {
        throw LogicException("visit SPARQL::ExprSHA512 not implemented");
    }
    virtual void visit(ExprStrAfter&)
    {
        throw LogicException("visit SPARQL::ExprStrAfter not implemented");
    }
    virtual void visit(ExprStrBefore&)
    {
        throw LogicException("visit SPARQL::ExprStrBefore not implemented");
    }
    virtual void visit(ExprStrDT&)
    {
        throw LogicException("visit SPARQL::ExprStrDT not implemented");
    }
    virtual void visit(ExprStrEnds&)
    {
        throw LogicException("visit SPARQL::ExprStrEnds not implemented");
    }
    virtual void visit(ExprStrLang&)
    {
        throw LogicException("visit SPARQL::ExprStrLang not implemented");
    }
    virtual void visit(ExprStrLen&)
    {
        throw LogicException("visit SPARQL::ExprStrLen not implemented");
    }
    virtual void visit(ExprStrStarts&)
    {
        throw LogicException("visit SPARQL::ExprStrStarts not implemented");
    }
    virtual void visit(ExprStrUUID&)
    {
        throw LogicException("visit SPARQL::ExprStrUUID not implemented");
    }
    virtual void visit(ExprStr&)
    {
        throw LogicException("visit SPARQL::ExprStr not implemented");
    }
    virtual void visit(ExprSubStr&)
    {
        throw LogicException("visit SPARQL::ExprSubStr not implemented");
    }
    virtual void visit(ExprTimezone&)
    {
        throw LogicException("visit SPARQL::ExprTimezone not implemented");
    }
    virtual void visit(ExprTZ&)
    {
        throw LogicException("visit SPARQL::ExprTZ not implemented");
    }
    virtual void visit(ExprUCase&)
    {
        throw LogicException("visit SPARQL::ExprUCase not implemented");
    }
    virtual void visit(ExprURI&)
    {
        throw LogicException("visit SPARQL::ExprURI not implemented");
    }
    virtual void visit(ExprUUID&)
    {
        throw LogicException("visit SPARQL::ExprUUID not implemented");
    }
    virtual void visit(ExprYear&)
    {
        throw LogicException("visit SPARQL::ExprYear not implemented");
    }
    virtual void visit(ExprCast&)
    {
        throw LogicException("visit SPARQL::ExprCast not implemented");
    }
    virtual void visit(ExprCosineDistance&)
    {
        throw LogicException("visit SPARQL::ExprCosineDistance not implemented");
    }
    virtual void visit(ExprCosineSimilarity&)
    {
        throw LogicException("visit SPARQL::ExprCosineSimilarity not implemented");
    }
    virtual void visit(ExprDot&)
    {
        throw LogicException("visit SPARQL::ExprDot not implemented");
    }
    virtual void visit(ExprEuclideanDistance&)
    {
        throw LogicException("visit SPARQL::ExprEuclideanDistance not implemented");
    }
    virtual void visit(ExprManhattanDistance&)
    {
        throw LogicException("visit SPARQL::ExprManhattanDistance not implemented");
    }
    virtual void visit(ExprPow&)
    {
        throw LogicException("visit SPARQL::ExprPow not implemented");
    }
    virtual void visit(ExprSqrt&)
    {
        throw LogicException("visit SPARQL::ExprSqrt not implemented");
    }
    virtual void visit(ExprSum&)
    {
        throw LogicException("visit SPARQL::ExprSum not implemented");
    }
};
} // namespace SPARQL
