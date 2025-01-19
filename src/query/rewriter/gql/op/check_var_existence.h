#pragma once

#include "query/parser/op/op_visitor.h"
#include "query/var_id.h"
#include <set>

namespace GQL {
class CheckVarExistence : public OpVisitor {
public:
    std::set<VarId> variables;

    void visit(GQL::OpGraphPattern&) override;
    void visit(GQL::OpBasicGraphPattern&) override;
    void visit(GQL::OpGraphPatternList&) override;
    void visit(GQL::OpFilter&) override;
    void visit(GQL::OpOptProperties&) override;
    void visit(GQL::OpProperty&) override;
    void visit(GQL::OpReturn&) override;
    void visit(GQL::OpOrderBy&) override;
    void visit(GQL::OpPathUnion&) override;
    void visit(GQL::OpNode&) override;
    void visit(GQL::OpEdge&) override;

    void visit(GQL::OpEdgeLabel&) override { }
    void visit(GQL::OpNodeLabel&) override { }
};

} // namespace GQL
