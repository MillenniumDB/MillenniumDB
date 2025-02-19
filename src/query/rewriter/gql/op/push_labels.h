#pragma once

#include <memory>

#include "query/parser/op/gql/ops.h"
#include "query/parser/op/op_visitor.h"
#include "query/rewriter/gql/expr/extract_labels_from_expr.h"

namespace GQL {

class PushLabels : public OpVisitor {
private:
    ExtractLabelsFromExpr visitor;
    std::vector<LabelOpId> labels_to_push;
    std::set<VarId> vars_in_linear_pattern;

public:
    std::unique_ptr<Op> tmp;
    PushLabels() = default;

    void store_parent_var_labels();
    void restore_parent_var_labels();
    void update_var_labels_to_push();

    void visit(OpGraphPattern& op) override;
    void visit(OpGraphPatternList& op) override;
    void visit(OpBasicGraphPattern& op) override;
    void visit(OpRepetition& op) override;
    void visit(OpReturn& op) override;
    void visit(OpOrderBy& op) override;
    void visit(OpFilter& op_filter) override;
    void visit(OpOptProperties&) override;
    void visit(OpPathUnion& op) override;
    void visit(OpLinearPattern& op) override;

    void visit(OpNode&) override;
    void visit(OpEdge&) override;
    void visit(OpNodeLabel&) override;
    void visit(OpEdgeLabel&) override;
    void visit(OpProperty&) override;
};
} // namespace GQL
