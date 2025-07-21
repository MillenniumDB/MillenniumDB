#pragma once

#include <memory>
#include <vector>

#include "query/parser/op/gql/op.h"
#include "query/parser/op/gql/op_visitor.h"

namespace GQL {

class AddStartingEnum : public OpVisitor {
public:
    bool repetition_found = false;

    void visit(OpReturn&) override;

    void visit(OpQueryStatements& op) override;
    void visit(OpFilterStatement& op) override;
    void visit(OpOrderByStatement& op) override;
    void visit(OpFilter& op) override;
    void visit(OpGraphPattern& op) override;
    void visit(OpGraphPatternList& op) override;
    void visit(OpBasicGraphPattern& op) override;
    void visit(OpLinearPattern& op) override;
    void visit(OpRepetition& op) override;
    void visit(OpOptProperties&) override;
    void visit(OpPathUnion& op) override;
    void visit(OpOrderBy& op) override;
    void visit(OpLet& op) override;
    void visit(OpGroupBy& op) override;

    void visit(OpNode&) override;
    void visit(OpEdge&) override;
    void visit(OpNodeLabel&) override;
    void visit(OpEdgeLabel&) override;
    void visit(OpProperty&) override;
};
} // namespace GQL
