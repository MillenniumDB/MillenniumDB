#pragma once

#include "query/exceptions.h"

namespace SPARQL {

class OpUpdate;
class OpInsertData;
class OpDeleteData;
class OpCreateTextIndex;
class OpCreateHNSWIndex;
class OpShow;

class OpAsk;
class OpBasicGraphPattern;
class OpBind;
class OpConstruct;
class OpDescribe;
class OpEmpty;
class OpFilter;
class OpFrom;
class OpGraph;
class OpGroupBy;
class OpHaving;
class OpJoin;
class OpMinus;
class OpNotExists;
class OpOptional;
class OpOrderBy;
class OpPath;
class OpProcedure;
class OpSelect;
class OpSemiJoin;
class OpSequence;
class OpService;
class OpTriple;
class OpUnion;
class OpUnitTable;
class OpValues;

class OpVisitor {
public:
    virtual ~OpVisitor() = default;

    // SPARQL
    virtual void visit(SPARQL::OpUpdate&)
    {
        throw LogicException("visit SPARQL::OpUpdate not implemented");
    }
    virtual void visit(SPARQL::OpInsertData&)
    {
        throw LogicException("visit SPARQL::OpInsertData not implemented");
    }
    virtual void visit(SPARQL::OpDeleteData&)
    {
        throw LogicException("visit SPARQL::OpDeleteData not implemented");
    }
    virtual void visit(SPARQL::OpCreateTextIndex&)
    {
        throw LogicException("visit SPARQL::OpCreateTextIndex not implemented");
    }
    virtual void visit(SPARQL::OpCreateHNSWIndex&)
    {
        throw LogicException("visit SPARQL::OpCreateHnswIndex not implemented");
    }

    virtual void visit(SPARQL::OpOptional&)
    {
        throw LogicException("visit SPARQL::OpOptional not implemented");
    }
    virtual void visit(SPARQL::OpOrderBy&)
    {
        throw LogicException("visit SPARQL::OpOrderBy not implemented");
    }
    virtual void visit(SPARQL::OpSelect&)
    {
        throw LogicException("visit SPARQL::OpSelect not implemented");
    }
    virtual void visit(SPARQL::OpAsk&)
    {
        throw LogicException("visit SPARQL::OpAsk not implemented");
    }
    virtual void visit(SPARQL::OpDescribe&)
    {
        throw LogicException("visit SPARQL::OpDescribe not implemented");
    }
    virtual void visit(SPARQL::OpConstruct&)
    {
        throw LogicException("visit SPARQL::OpConstruct not implemented");
    }
    virtual void visit(SPARQL::OpGroupBy&)
    {
        throw LogicException("visit SPARQL::OpGroupBy not implemented");
    }
    virtual void visit(SPARQL::OpHaving&)
    {
        throw LogicException("visit SPARQL::OpHaving not implemented");
    }
    virtual void visit(SPARQL::OpBasicGraphPattern&)
    {
        throw LogicException("visit SPARQL::OpBasicGraphPattern not implemented");
    }
    virtual void visit(SPARQL::OpTriple&)
    {
        throw LogicException("visit SPARQL::OpTriple not implemented");
    }
    virtual void visit(SPARQL::OpPath&)
    {
        throw LogicException("visit SPARQL::OpPath not implemented");
    }
    virtual void visit(SPARQL::OpProcedure&)
    {
        throw LogicException("visit SPARQL::OpProcedure not implemented");
    }
    virtual void visit(SPARQL::OpUnitTable&)
    {
        throw LogicException("visit SPARQL::OpUnitTable not implemented");
    }
    virtual void visit(SPARQL::OpJoin&)
    {
        throw LogicException("visit SPARQL::OpJoin not implemented");
    }
    virtual void visit(SPARQL::OpSemiJoin&)
    {
        throw LogicException("visit SPARQL::OpSemiJoin not implemented");
    }
    virtual void visit(SPARQL::OpUnion&)
    {
        throw LogicException("visit SPARQL::OpUnion not implemented");
    }
    virtual void visit(SPARQL::OpMinus&)
    {
        throw LogicException("visit SPARQL::OpMinus not implemented");
    }
    virtual void visit(SPARQL::OpNotExists&)
    {
        throw LogicException("visit SPARQL::OpMinus not implemented");
    }
    virtual void visit(SPARQL::OpSequence&)
    {
        throw LogicException("visit SPARQL::OpSequence not implemented");
    }
    virtual void visit(SPARQL::OpService&)
    {
        throw LogicException("visit SPARQL::OpService not implemented");
    }
    virtual void visit(SPARQL::OpFrom&)
    {
        throw LogicException("visit SPARQL::OpFrom not implemented");
    }
    virtual void visit(SPARQL::OpGraph&)
    {
        throw LogicException("visit SPARQL::OpGraph not implemented");
    }
    virtual void visit(SPARQL::OpFilter&)
    {
        throw LogicException("visit SPARQL::OpFilter not implemented");
    }
    virtual void visit(SPARQL::OpEmpty&)
    {
        throw LogicException("visit SPARQL::OpEmpty not implemented");
    }
    virtual void visit(SPARQL::OpBind&)
    {
        throw LogicException("visit SPARQL::OpBind not implemented");
    }
    virtual void visit(SPARQL::OpValues&)
    {
        throw LogicException("visit SPARQL::OpValues not implemented");
    }
    virtual void visit(SPARQL::OpShow&)
    {
        throw LogicException("visit SPARQL::OpShow not implemented");
    }
};
} // namespace SPARQL
