#pragma once

#include <vector>

#include "query/parser/op/gql/graph_pattern/op_optional_properties.h"
#include "query/parser/op/gql/op_visitor.h"

namespace GQL {
class ExtractOptionalProperties : public OpVisitor {
public:
    std::vector<OptPropertyValue> properties;

    void visit(OpReturn&) override;
    void visit(OpFilterStatement&) override;
    void visit(OpOrderBy&) override;

    void visit(OpQueryStatements& op) override;
    void visit(OpFilter& op) override;
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
    void visit(OpLet&) override;
};

} // namespace GQL
