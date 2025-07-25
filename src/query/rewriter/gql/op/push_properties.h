#pragma once

#include <memory>

#include "query/parser/op/gql/op_visitor.h"
#include "query/rewriter/gql/expr/extract_properties_from_expr.h"
#include "query/rewriter/gql/op/rewrite_rules/rewrite_rule.h"

namespace GQL {

class PushProperties : public OpVisitor {
public:
    ExtractPropertiesFromExpr visitor;
    std::unique_ptr<Op> tmp;
    std::vector<PropertyValue> properties;
    std::set<VarId> vars_in_linear_pattern;

    void visit(OpQueryStatements&) override;
    void visit(OpFilterStatement&) override;
    void visit(OpOrderByStatement&) override;
    void visit(OpGraphPattern&) override;
    void visit(OpGraphPatternList&) override;
    void visit(OpBasicGraphPattern&) override;
    void visit(OpRepetition&) override;
    void visit(OpReturn&) override;
    void visit(OpLinearPattern&) override;
    void visit(OpLet&) override;

    void visit(OpOrderBy&) override;
    void visit(OpFilter&) override;
    void visit(OpOptProperties&) override;
    void visit(OpPathUnion&) override;
    void visit(OpNode&) override;
    void visit(OpEdge&) override;
    void visit(OpNodeLabel&) override;
    void visit(OpEdgeLabel&) override;
};
} // namespace GQL
