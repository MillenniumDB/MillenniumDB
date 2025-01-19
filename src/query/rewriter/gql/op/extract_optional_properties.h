#pragma once

#include "query/parser/op/op_visitor.h"
#include "query/parser/op/gql/ops.h"
#include "query/var_id.h"
#include <set>

namespace GQL {
class ExtractOptionalProperties : public OpVisitor {
public:
    std::vector<OptPropertyValue> properties;

    void visit(OpReturn&) override;
    void visit(OpOrderBy&) override;

    void visit(OpFilter& op_filter) override;
    void visit(OpGraphPattern& op) override;
    void visit(OpGraphPatternList& op) override;
    void visit(OpBasicGraphPattern& op) override;
    void visit(OpRepetition& op) override;
    void visit(OpOptProperties&) override;
    void visit(OpPathUnion& op) override;

    void visit(OpNode&) override;
    void visit(OpEdge&) override;
    void visit(OpNodeLabel&) override;
    void visit(OpEdgeLabel&) override;
    void visit(OpProperty&) override;
};

} // namespace GQL
