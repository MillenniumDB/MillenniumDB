#pragma once

#include "query/parser/op/op.h"
#include "query/parser/op/op_visitor.h"
#include <memory>
#include <vector>

namespace GQL {

class ExtractOptionalLabels : public OpVisitor {
public:
    std::vector<std::unique_ptr<Op>> op_labels_to_push;

    void visit(OpReturn&) override;

    void visit(OpFilter& op_filter) override;
    void visit(OpGraphPattern& op) override;
    void visit(OpGraphPatternList& op) override;
    void visit(OpBasicGraphPattern& op) override;
    void visit(OpRepetition& op) override;
    void visit(OpOptProperties&) override;
    void visit(OpPathUnion& op) override;
    void visit(OpOrderBy& op) override;

    void visit(OpNode&) override;
    void visit(OpEdge&) override;
    void visit(OpNodeLabel&) override;
    void visit(OpEdgeLabel&) override;
    void visit(OpProperty&) override;
};
} // namespace GQL
