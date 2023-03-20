#pragma once

class OpBasicGraphPattern;
class OpDescribe;
class OpEdge;
class OpGroupBy;
class OpInsert;
class OpIsolatedTerm;
class OpIsolatedVar;
class OpLabel;
class OpMatch;
class OpOptional;
class OpOrderBy;
class OpPath;
class OpProperty;
class OpReturn;
class OpSet;
class OpWhere;

class OpVisitor {
public:
    virtual ~OpVisitor() = default;

    virtual void visit(OpBasicGraphPattern&) = 0;
    virtual void visit(OpDescribe&)          = 0;
    virtual void visit(OpEdge&)              = 0;
    virtual void visit(OpGroupBy&)           = 0;
    virtual void visit(OpInsert&) { } // TODO: make empty default for all Ops?
    virtual void visit(OpIsolatedTerm&)      = 0;
    virtual void visit(OpIsolatedVar&)       = 0;
    virtual void visit(OpLabel&)             = 0;
    virtual void visit(OpMatch&)             = 0;
    virtual void visit(OpOptional&)          = 0;
    virtual void visit(OpOrderBy&)           = 0;
    virtual void visit(OpPath&)              = 0;
    virtual void visit(OpProperty&)          = 0;
    virtual void visit(OpReturn&)            = 0;
    virtual void visit(OpSet&)               = 0;
    virtual void visit(OpWhere&)             = 0;
};
