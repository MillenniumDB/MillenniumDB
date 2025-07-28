#pragma once

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

    virtual void visit(OpUpdate&) = 0;
    virtual void visit(OpBasicGraphPattern&) = 0;
    virtual void visit(OpCall&) = 0;
    virtual void visit(OpCreateHNSWIndex&) = 0;
    virtual void visit(OpCreateTextIndex&) = 0;
    virtual void visit(OpDescribe&) = 0;
    virtual void visit(OpEdge&) = 0;
    virtual void visit(OpGroupBy&) = 0;
    virtual void visit(OpInsert&) = 0;
    virtual void visit(OpDisjointTerm&) = 0;
    virtual void visit(OpDisjointVar&) = 0;
    virtual void visit(OpLabel&) = 0;
    virtual void visit(OpLet&) = 0;
    virtual void visit(OpOptional&) = 0;
    virtual void visit(OpOrderBy&) = 0;
    virtual void visit(OpPath&) = 0;
    virtual void visit(OpProperty&) = 0;
    virtual void visit(OpReturn&) = 0;
    virtual void visit(OpSequence&) = 0;
    virtual void visit(OpShow&) = 0;
    virtual void visit(OpWhere&) = 0;
};

} // namespace MQL
