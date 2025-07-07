#pragma once

#include "query/parser/op/gql/ops.h"

using namespace GQL;

class AddLinearPatterns : public OpVisitor {
public:
    enum LinearType {
        None,
        Node,
        Edge,
    };

    LinearType linear_pattern = None;
    bool consecutive_nodes_found = false;

    std::unique_ptr<VarId> start;
    std::unique_ptr<VarId> end;

    std::unique_ptr<Op> tmp;

    void visit(OpQueryStatements&) override;
    void visit(OpFilterStatement&) override;
    void visit(OpGraphPattern&) override;
    void visit(OpGraphPatternList&) override;
    void visit(OpBasicGraphPattern&) override;
    void visit(OpRepetition&) override;
    void visit(OpReturn&) override;
    void visit(OpLet&) override;

    void visit(OpOrderBy&) override;
    void visit(OpOrderByStatement&) override;
    void visit(OpFilter&) override;
    void visit(OpOptProperties&) override;
    void visit(OpProperty&) override;
    void visit(OpPathUnion&) override;
    void visit(OpNode&) override;
    void visit(OpEdge&) override;
    void visit(OpNodeLabel&) override;
    void visit(OpEdgeLabel&) override;
};
