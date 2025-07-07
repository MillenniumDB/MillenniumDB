#pragma once

#include <memory>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/gql/path_binding_iter.h"
#include "query/optimizer/plan/plan.h"
#include "query/parser/expr/gql/expr.h"
#include "query/parser/op/gql/graph_pattern/op_optional_properties.h"
#include "query/parser/op/gql/op_visitor.h"

namespace GQL {

class PathBindingIterConstructor : public OpVisitor {
public:
    PathBindingIterConstructor() = default;

    void handle_order_by(
        std::vector<std::unique_ptr<Expr>>& items,
        std::vector<bool>& ascending_order,
        std::vector<bool>& null_order
    );

    void visit(OpQueryStatements& op) override;
    void visit(OpFilterStatement& op) override;
    void visit(OpLet& op) override;
    void visit(OpOrderBy&) override;
    void visit(OpOrderByStatement&) override;

    void visit(OpReturn&) override;
    void visit(OpGraphPattern&) override;
    void visit(OpBasicGraphPattern&) override;
    void visit(OpGraphPatternList&) override;
    void visit(OpFilter&) override;
    void visit(OpPathUnion&) override;
    void visit(OpRepetition&) override;
    void visit(OpOptProperties&) override;
    void visit(OpOptLabels& op) override;
    void visit(OpLinearPattern& op) override;

    void visit(OpNode&) override;
    void visit(OpNodeLabel&) override;
    void visit(OpEdgeLabel&) override;
    void visit(OpEdge&) override;
    void visit(OpProperty&) override;

    std::unique_ptr<BindingIter> tmp_iter;

    bool grouping = false;
    std::map<VarId, std::unique_ptr<Agg>> aggregations;

private:
    bool is_first_iter = true;
    bool is_first_gp = true;

    std::vector<VarId> return_op_vars;

    std::set<VarId> group_vars;

    std::set<VarId> seen_nodes;
    std::set<VarId> possible_disjoint_nodes;
    std::set<VarId> assigned_vars;
    std::set<VarId> graph_pattern_vars;
    std::vector<VarId> linear_pattern_vars;
    std::set<std::pair<VarId, ObjectId>> setted_vars;
    std::vector<std::unique_ptr<Plan>> base_plans;

    std::unique_ptr<PathBindingIter> tmp_path;
};

} // namespace GQL
