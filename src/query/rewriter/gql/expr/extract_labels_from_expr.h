#pragma once

#include "query/parser/expr/expr.h"
#include "query/parser/expr/gql_exprs.h"
#include "query/parser/op/gql/graph_pattern/op_edge_label.h"
#include "query/parser/op/gql/graph_pattern/op_node_label.h"

namespace GQL {

class ExtractLabelsFromExpr : public ExprVisitor {
public:
    std::vector<std::unique_ptr<Op>> labels;
    std::unique_ptr<Expr> tmp;

    void clear_labels()
    {
        labels.clear();
    }

    void visit(ExprAnd& and_expr)
    {
        std::vector<std::unique_ptr<Expr>> and_exprs;
        for (auto& expr : and_expr.exprs) {
            expr->accept_visitor(*this);

            if (tmp != nullptr) {
                and_exprs.push_back(std::move(tmp));
            }
        }

        if (and_exprs.size() > 1) {
            tmp = std::make_unique<ExprAnd>(std::move(and_exprs));
        } else if (and_exprs.size() == 1) {
            tmp = std::move(and_exprs[0]);
        }
    }

    void visit(ExprHasNodeLabel& expr)
    {
        labels.push_back(std::make_unique<OpNodeLabel>(expr.node_id, expr.label_id));
    }

    void visit(ExprHasEdgeLabel& expr)
    {
        labels.push_back(std::make_unique<OpEdgeLabel>(expr.edge_id, expr.label_id));
    }

    void visit(ExprNot& expr)
    {
        tmp = expr.clone();
    }

    void visit(ExprOr& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprEquals& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprVar& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprProperty& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprXor& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprConcat& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprGreaterOrEquals& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprGreater& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprIs& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprLessOrEquals& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprLess& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprNotEquals& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprTerm& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAddition& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprSubtraction& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprMultiplication& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprDivision& expr)
    {
        tmp = expr.clone();
    }
};
} // namespace GQL
