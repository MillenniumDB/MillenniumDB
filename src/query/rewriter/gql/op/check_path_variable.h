#pragma once

#include "query/parser/op/gql/ops.h"

namespace GQL {

// MATCH COLLAPSE: rejects a path variable on a pattern whose paths can start or end with an edge
class CheckPathVariable : public OpVisitor {
    // facts about the paths that the pattern just visited can match
    bool can_be_empty = false;
    bool can_start_with_edge = false;
    bool can_end_with_edge = false;

    void visit(OpGraphPattern& op) override;
    void visit(OpGraphPatternList& op) override;
    void visit(OpBasicGraphPattern& op) override;
    void visit(OpRepetition& op) override;
    void visit(OpReturn& op) override;
    void visit(OpLinearPattern& op) override;

    void visit(OpWhere& op) override;
    void visit(OpPathUnion& op) override;
    void visit(OpGroupBy& op) override;

    void visit(OpOrderBy& op) override;
    void visit(OpFilter& op) override;
    void visit(OpLet& op) override;
    void visit(OpQueryStatements& op) override;
    void visit(OpNode& op) override;
    void visit(OpEdge& op) override;
    void visit(OpUnitTable&) override;
    void visit(OpEmpty&) override;
};

} // namespace GQL
