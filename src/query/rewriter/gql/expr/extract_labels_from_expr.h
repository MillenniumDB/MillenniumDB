#pragma once

#include "query/parser/expr/expr.h"
#include "query/parser/expr/gql_exprs.h"
#include "query/parser/op/gql/graph_pattern/op_edge_label.h"
#include "query/parser/op/gql/graph_pattern/op_node_label.h"

namespace GQL {

class ExtractLabelsFromExpr : public ExprVisitor {
public:
    std::vector<LabelOpId> labels;
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
        labels.emplace_back(std::make_unique<OpNodeLabel>(expr.node_id, expr.label_id), expr.node_id);
    }

    void visit(ExprHasEdgeLabel& expr)
    {
        labels.emplace_back(std::make_unique<OpEdgeLabel>(expr.edge_id, expr.label_id), expr.edge_id);
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

    void visit(GQL::ExprAbs& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprModulo& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprSin& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprCos& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprTan& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprCot& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprSinh& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprCosh& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprTanh& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAsin& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAcos& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAtan& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprDegrees& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprRadians& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprLog& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprLog10& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprLn& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprExp& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprPower& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprSqrt& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprFloor& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprCeil& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprLength& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprSubStr& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprFold& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprSingleTrim& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprMultiTrim& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprNormalize& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprNullIf& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprCoalesce& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprSimpleCase& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprSearchedCase& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprCast& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAggCountAll& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAggCount& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAggAvg& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAggMax& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAggMin& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAggSum& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAggStddevPop& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAggStddevSamp& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAggCollect& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAggPercentileCont& expr)
    {
        tmp = expr.clone();
    }

    void visit(GQL::ExprAggPercentileDisc& expr)
    {
        tmp = expr.clone();
    }
};
} // namespace GQL
