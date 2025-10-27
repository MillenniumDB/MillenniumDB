#pragma once

namespace MQL {
class OpUpdate;
class OpDescribe;
class OpReturn;
class OpShow;

class OpBasicGraphPattern;
class OpCall;
class OpGroupBy;
class OpHaving;
class OpLet;
class OpOptional;
class OpOrderBy;
class OpSequence;
class OpUnitTable;
class OpWhere;

class OpVisitor {
public:
    virtual ~OpVisitor() = default;

    virtual void visit(OpUpdate&) = 0;
    virtual void visit(OpDescribe&) = 0;
    virtual void visit(OpReturn&) = 0;
    virtual void visit(OpShow&) = 0;

    virtual void visit(OpBasicGraphPattern&) = 0;
    virtual void visit(OpCall&) = 0;
    virtual void visit(OpGroupBy&) = 0;
    virtual void visit(OpHaving&) = 0;
    virtual void visit(OpLet&) = 0;
    virtual void visit(OpOptional&) = 0;
    virtual void visit(OpOrderBy&) = 0;
    virtual void visit(OpSequence&) = 0;
    virtual void visit(OpUnitTable&) = 0;
    virtual void visit(OpWhere&) = 0;
};

} // namespace MQL
