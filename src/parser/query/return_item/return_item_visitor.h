#pragma once

class ReturnItemAgg;
class ReturnItemCount;
class ReturnItemVar;

class ReturnItemVisitor {
public:
    virtual ~ReturnItemVisitor() = default;

    virtual void visit(ReturnItemAgg&)   = 0;
    virtual void visit(ReturnItemCount&) = 0;
    virtual void visit(ReturnItemVar&)   = 0;
};
