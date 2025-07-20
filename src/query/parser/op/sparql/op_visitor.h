#pragma once

namespace SPARQL {

class OpCreateHNSWIndex;
class OpCreateTextIndex;
class OpDeleteData;
class OpInsertData;
class OpUpdate;

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
class OpShow;
class OpTriple;
class OpUnion;
class OpUnitTable;
class OpValues;

class OpVisitor {
public:
    virtual ~OpVisitor() = default;

    virtual void visit(OpCreateHNSWIndex&) = 0;
    virtual void visit(OpCreateTextIndex&) = 0;
    virtual void visit(OpDeleteData&) = 0;
    virtual void visit(OpInsertData&) = 0;
    virtual void visit(OpUpdate&) = 0;

    virtual void visit(OpAsk&) = 0;
    virtual void visit(OpBasicGraphPattern&) = 0;
    virtual void visit(OpBind&) = 0;
    virtual void visit(OpConstruct&) = 0;
    virtual void visit(OpDescribe&) = 0;
    virtual void visit(OpEmpty&) = 0;
    virtual void visit(OpFrom&) = 0;
    virtual void visit(OpFilter&) = 0;
    virtual void visit(OpGraph&) = 0;
    virtual void visit(OpGroupBy&) = 0;
    virtual void visit(OpHaving&) = 0;
    virtual void visit(OpJoin&) = 0;
    virtual void visit(OpMinus&) = 0;
    virtual void visit(OpNotExists&) = 0;
    virtual void visit(OpOptional&) = 0;
    virtual void visit(OpOrderBy&) = 0;
    virtual void visit(OpPath&) = 0;
    virtual void visit(OpProcedure&) = 0;
    virtual void visit(OpSequence&) = 0;
    virtual void visit(OpSelect&) = 0;
    virtual void visit(OpSemiJoin&) = 0;
    virtual void visit(OpService&) = 0;
    virtual void visit(OpShow&) = 0;
    virtual void visit(OpTriple&) = 0;
    virtual void visit(OpUnion&) = 0;
    virtual void visit(OpUnitTable&) = 0;
    virtual void visit(OpValues&) = 0;
};
} // namespace SPARQL
