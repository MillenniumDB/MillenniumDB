#pragma once

namespace MQL {

class InsertNode;
class InsertLabel;
class SetLabelOrType;
class InsertProperty;
class InsertPropertyExpr;
class DeleteProperty;
class DeleteLabel;
class InsertEdge;
class DeleteObject;
class CreateTextIndex;
class CreateHNSWIndex;

class UpdateActionVisitor {
public:
    virtual ~UpdateActionVisitor() = default;

    virtual void visit(InsertNode&) = 0;
    virtual void visit(InsertLabel&) = 0;
    virtual void visit(SetLabelOrType&) = 0;
    virtual void visit(InsertProperty&) = 0;
    virtual void visit(InsertPropertyExpr&) = 0;
    virtual void visit(DeleteProperty&) = 0;
    virtual void visit(DeleteLabel&) = 0;
    virtual void visit(InsertEdge&) = 0;
    virtual void visit(DeleteObject&) = 0;
    virtual void visit(CreateTextIndex&) = 0;
    virtual void visit(CreateHNSWIndex&) = 0;
};
} // namespace MQL
