#pragma once

#include "query/exceptions.h"

namespace MQL {
class OpUpdate;

class OpBasicGraphPattern;
class OpCall;
class OpCreateHNSWIndex;
class OpCreateTextIndex;
class OpDescribe;
class OpEdge;
class OpGroupBy;
class OpInsert;
class OpDisjointTerm;
class OpDisjointVar;
class OpLabel;
class OpLet;
class OpOptional;
class OpOrderBy;
class OpPath;
class OpProperty;
class OpReturn;
class OpSequence;
class OpShow;
class OpWhere;

class OpVisitor {
public:
    virtual ~OpVisitor() = default;

    // MillenniumDB
    virtual void visit(OpUpdate&)
    {
        throw LogicException("visit MQL::OpUpdate not implemented");
    }
    virtual void visit(OpBasicGraphPattern&)
    {
        throw LogicException("visit MQL::OpBasicGraphPattern not implemented");
    }
    virtual void visit(OpCall&)
    {
        throw LogicException("visit MQL::OpCall not implemented");
    }
    virtual void visit(OpCreateHNSWIndex&)
    {
        throw LogicException("visit MQL::OpCreateHNSWIndex not implemented");
    }
    virtual void visit(OpCreateTextIndex&)
    {
        throw LogicException("visit MQL::OpCreateTextIndex not implemented");
    }
    virtual void visit(OpDescribe&)
    {
        throw LogicException("visit MQL::OpDescribe not implemented");
    }
    virtual void visit(OpEdge&)
    {
        throw LogicException("visit MQL::OpEdge not implemented");
    }
    virtual void visit(OpGroupBy&)
    {
        throw LogicException("visit MQL::OpGroupBy not implemented");
    }
    virtual void visit(OpInsert&)
    {
        throw LogicException("visit MQL::OpInsert not implemented");
    }
    virtual void visit(OpDisjointTerm&)
    {
        throw LogicException("visit MQL::OpDisjointTerm not implemented");
    }
    virtual void visit(OpDisjointVar&)
    {
        throw LogicException("visit MQL::OpDisjointVar not implemented");
    }
    virtual void visit(OpLabel&)
    {
        throw LogicException("visit MQL::OpLabel not implemented");
    }
    virtual void visit(OpLet&) {
        throw LogicException("visit MQL::OpLet not implemented");
    }
    virtual void visit(OpOptional&)
    {
        throw LogicException("visit MQL::OpOptional not implemented");
    }
    virtual void visit(OpOrderBy&)
    {
        throw LogicException("visit MQL::OpOrderBy not implemented");
    }
    virtual void visit(OpPath&)
    {
        throw LogicException("visit MQL::OpPath not implemented");
    }
    virtual void visit(OpProperty&)
    {
        throw LogicException("visit MQL::OpProperty not implemented");
    }
    virtual void visit(OpReturn&)
    {
        throw LogicException("visit MQL::OpReturn not implemented");
    }
    virtual void visit(OpSequence&)
    {
        throw LogicException("visit MQL::OpSequence not implemented");
    }
    virtual void visit(OpShow&)
    {
        throw LogicException("visit MQL::OpShow not implemented");
    }
    virtual void visit(OpWhere&)
    {
        throw LogicException("visit MQL::OpWhere not implemented");
    }
};

} // namespace MQL
