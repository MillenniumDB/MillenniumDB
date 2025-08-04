#pragma once

#include "query/parser/op/gql/ops.h"

namespace GQL {
class AddLinearPatterns : public OpVisitor {
public:
    enum LinearType {
        None,
        Node,
        Edge,
    };

    LinearType linear_pattern = None;
    bool consecutive_nodes_found = false;

    std::unique_ptr<Op> tmp;

    void visit(OpQueryStatements&) override;
    void visit(OpFilter&) override;
    void visit(OpGraphPattern&) override;
    void visit(OpGraphPatternList&) override;
    void visit(OpBasicGraphPattern&) override;
    void visit(OpRepetition&) override;
    void visit(OpReturn&) override;
    void visit(OpLet&) override;

    void visit(OpGroupBy&) override;
    void visit(OpOrderBy&) override;
    void visit(OpWhere&) override;
    void visit(OpPathUnion&) override;
    void visit(OpNode&) override;
    void visit(OpEdge&) override;
    void visit(OpLinearPattern&) override;
};
} // namespace GQL
