#pragma once

#include "query/parser/op/gql/ops.h"

namespace GQL {

class CheckUnboundedRepetition : public OpVisitor {
    bool has_unbounded_repetition = false;

    void check_mode(const PathMode& mode);

    void visit(OpGraphPattern& op) override;
    void visit(OpGraphPatternList& op) override;
    void visit(OpBasicGraphPattern& op) override;
    void visit(OpRepetition& op) override;
    void visit(OpReturn& op) override;

    void visit(OpOrderBy& op) override;
    void visit(OpFilter& op) override;
    void visit(OpOptProperties& op) override;
    void visit(OpProperty& op) override;
    void visit(OpPathUnion& op) override;

    void visit(OpOrderByStatement& op) override;
    void visit(OpFilterStatement& op) override;
    void visit(OpLet& op) override;
    void visit(OpQueryStatements& op) override;
    void visit(OpNode& op) override;
    void visit(OpEdge& op) override;
    void visit(OpNodeLabel& op) override;
    void visit(OpEdgeLabel& op) override;
};

} // namespace GQL
