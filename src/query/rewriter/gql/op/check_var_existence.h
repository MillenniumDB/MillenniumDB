#pragma once

#include <set>
#include <vector>

#include "query/parser/op/gql/op_visitor.h"
#include "query/var_id.h"

namespace GQL {
class CheckVarExistence : public OpVisitor {
public:
    std::set<VarId> variables;
    std::set<VarId> let_variables;
    std::set<VarId> op_return_vars;

    void check_expr_variables(const std::set<VarId>& expr_variables);

    void visit(GQL::OpQueryStatements&) override;
    void visit(GQL::OpFilterStatement&) override;
    void visit(GQL::OpGraphPattern&) override;
    void visit(GQL::OpBasicGraphPattern&) override;
    void visit(GQL::OpGraphPatternList&) override;
    void visit(GQL::OpFilter&) override;
    void visit(GQL::OpOptProperties&) override;
    void visit(GQL::OpProperty&) override;
    void visit(GQL::OpReturn&) override;
    void visit(GQL::OpOrderBy&) override;
    void visit(GQL::OpOrderByStatement&) override;
    void visit(GQL::OpPathUnion&) override;
    void visit(GQL::OpRepetition&) override;
    void visit(GQL::OpNode&) override;
    void visit(GQL::OpEdge&) override;
    void visit(GQL::OpLet&) override;
    void visit(GQL::OpGroupBy&) override;

    void visit(GQL::OpEdgeLabel&) override { }
    void visit(GQL::OpNodeLabel&) override { }
};

} // namespace GQL
