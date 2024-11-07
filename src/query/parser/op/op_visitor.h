#pragma once

#include "query/exceptions.h"

namespace MQL {
class OpUpdate;

class OpBasicGraphPattern;
class OpDescribe;
class OpEdge;
class OpGroupBy;
class OpInsert;
class OpDisjointTerm;
class OpDisjointVar;
class OpLabel;
class OpMatch;
class OpOptional;
class OpOrderBy;
class OpPath;
class OpProperty;
class OpReturn;
class OpSet;
class OpWhere;
class OpBruteSimilaritySearch;
class OpSimilaritySearch;
class OpProjectSimilarity;
} // namespace MQL

namespace SPARQL {

class OpUpdate;
class OpInsertData;
class OpDeleteData;

class OpOptional;
class OpOrderBy;
class OpSelect;
class OpAsk;
class OpDescribe;
class OpConstruct;
class OpGroupBy;
class OpHaving;
class OpBasicGraphPattern;
class OpTriple;
class OpPath;
class OpUnitTable;
class OpJoin;
class OpSemiJoin;
class OpUnion;
class OpMinus;
class OpNotExists;
class OpService;
class OpSequence;
class OpGraph;
class OpFilter;
class OpEmpty;
class OpBind;
class OpValues;
} // namespace SPARQL

class OpVisitor {
public:
    virtual ~OpVisitor() = default;

    // MillenniumDB
    virtual void visit(MQL::OpUpdate&)            { throw LogicException("visit MQL::OpUpdate not implemented"); }

    virtual void visit(MQL::OpBasicGraphPattern&)     { throw LogicException("visit MQL::OpBasicGraphPattern not implemented"); }
    virtual void visit(MQL::OpDescribe&)              { throw LogicException("visit MQL::OpDescribe not implemented"); }
    virtual void visit(MQL::OpEdge&)                  { throw LogicException("visit MQL::OpEdge not implemented"); }
    virtual void visit(MQL::OpGroupBy&)               { throw LogicException("visit MQL::OpGroupBy not implemented"); }
    virtual void visit(MQL::OpInsert&)                { throw LogicException("visit MQL::OpInsert not implemented"); }
    virtual void visit(MQL::OpDisjointTerm&)          { throw LogicException("visit MQL::OpDisjointTerm not implemented"); }
    virtual void visit(MQL::OpDisjointVar&)           { throw LogicException("visit MQL::OpDisjointVar not implemented"); }
    virtual void visit(MQL::OpLabel&)                 { throw LogicException("visit MQL::OpLabel not implemented"); }
    virtual void visit(MQL::OpMatch&)                 { throw LogicException("visit MQL::OpMatch not implemented"); }
    virtual void visit(MQL::OpOptional&)              { throw LogicException("visit MQL::OpOptional not implemented"); }
    virtual void visit(MQL::OpOrderBy&)               { throw LogicException("visit MQL::OpOrderBy not implemented"); }
    virtual void visit(MQL::OpPath&)                  { throw LogicException("visit MQL::OpPath not implemented"); }
    virtual void visit(MQL::OpProperty&)              { throw LogicException("visit MQL::OpProperty not implemented"); }
    virtual void visit(MQL::OpReturn&)                { throw LogicException("visit MQL::OpReturn not implemented"); }
    virtual void visit(MQL::OpSet&)                   { throw LogicException("visit MQL::OpSet not implemented"); }
    virtual void visit(MQL::OpWhere&)                 { throw LogicException("visit MQL::OpWhere not implemented"); }
    virtual void visit(MQL::OpSimilaritySearch&)      { throw LogicException("visit MQL::OpSimilaritySearch not implemented"); }
    virtual void visit(MQL::OpBruteSimilaritySearch&) { throw LogicException("visit MQL::OpBruteSimilaritySearch not implemented"); }
    virtual void visit(MQL::OpProjectSimilarity&)     { throw LogicException("visit MQL::OpProjectSimilarity not implemented"); }

    // SPARQL
    virtual void visit(SPARQL::OpUpdate&)            { throw LogicException("visit SPARQL::OpUpdate not implemented"); }
    virtual void visit(SPARQL::OpInsertData&)        { throw LogicException("visit SPARQL::OpInsertData not implemented"); }
    virtual void visit(SPARQL::OpDeleteData&)        { throw LogicException("visit SPARQL::OpDeleteData not implemented"); }

    virtual void visit(SPARQL::OpOptional&)          { throw LogicException("visit SPARQL::OpOptional not implemented"); }
    virtual void visit(SPARQL::OpOrderBy&)           { throw LogicException("visit SPARQL::OpOrderBy not implemented"); }
    virtual void visit(SPARQL::OpSelect&)            { throw LogicException("visit SPARQL::OpSelect not implemented"); }
    virtual void visit(SPARQL::OpAsk&)               { throw LogicException("visit SPARQL::OpAsk not implemented"); }
    virtual void visit(SPARQL::OpDescribe&)          { throw LogicException("visit SPARQL::OpDescribe not implemented"); }
    virtual void visit(SPARQL::OpConstruct&)         { throw LogicException("visit SPARQL::OpConstruct not implemented"); }
    virtual void visit(SPARQL::OpGroupBy&)           { throw LogicException("visit SPARQL::OpGroupBy not implemented"); }
    virtual void visit(SPARQL::OpHaving&)            { throw LogicException("visit SPARQL::OpHaving not implemented"); }
    virtual void visit(SPARQL::OpBasicGraphPattern&) { throw LogicException("visit SPARQL::OpBasicGraphPattern not implemented"); }
    virtual void visit(SPARQL::OpTriple&)            { throw LogicException("visit SPARQL::OpTriple not implemented"); }
    virtual void visit(SPARQL::OpPath&)              { throw LogicException("visit SPARQL::OpPath not implemented"); }
    virtual void visit(SPARQL::OpUnitTable&)         { throw LogicException("visit SPARQL::OpUnitTable not implemented"); }
    virtual void visit(SPARQL::OpJoin&)              { throw LogicException("visit SPARQL::OpJoin not implemented"); }
    virtual void visit(SPARQL::OpSemiJoin&)          { throw LogicException("visit SPARQL::OpSemiJoin not implemented"); }
    virtual void visit(SPARQL::OpUnion&)             { throw LogicException("visit SPARQL::OpUnion not implemented"); }
    virtual void visit(SPARQL::OpMinus&)             { throw LogicException("visit SPARQL::OpMinus not implemented"); }
    virtual void visit(SPARQL::OpNotExists&)         { throw LogicException("visit SPARQL::OpMinus not implemented"); }
    virtual void visit(SPARQL::OpSequence&)          { throw LogicException("visit SPARQL::OpSequence not implemented"); }
    virtual void visit(SPARQL::OpService&)           { throw LogicException("visit SPARQL::OpService not implemented"); }
    virtual void visit(SPARQL::OpGraph&)             { throw LogicException("visit SPARQL::OpGraph not implemented"); }
    virtual void visit(SPARQL::OpFilter&)            { throw LogicException("visit SPARQL::OpFilter not implemented"); }
    virtual void visit(SPARQL::OpEmpty&)             { throw LogicException("visit SPARQL::OpEmpty not implemented"); }
    virtual void visit(SPARQL::OpBind&)              { throw LogicException("visit SPARQL::OpBind not implemented"); }
    virtual void visit(SPARQL::OpValues&)            { throw LogicException("visit SPARQL::OpValues not implemented"); }
};
