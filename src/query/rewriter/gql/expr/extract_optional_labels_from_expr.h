#pragma once

#include "query/parser/expr/expr.h"
#include "query/parser/expr/expr_visitor.h"
#include "query/parser/expr/gql_exprs.h"
#include "query/parser/op/gql/graph_pattern/op_opt_labels.h"
#include "query/parser/op/gql/graph_pattern/op_property.h"

using namespace GQL;

class ExtractVarWithLabelsFromExpr : public ExprVisitor {
public:
    std::set<OptLabel> vars;
    bool negation = false;

    void visit(ExprHasNodeLabel& expr)
    {
        if (!negation) {
            return;
        }
        std::string var_name = get_query_ctx().get_var_name(expr.node_id);
        VarId labels_var = get_query_ctx().get_or_create_var("." + var_name + "_labels");
        vars.insert({ expr.node_id, labels_var, VarType::Node });
    }

    void visit(ExprHasEdgeLabel& expr)
    {
        if (!negation) {
            return;
        }
        std::string var_name = get_query_ctx().get_var_name(expr.edge_id);
        VarId labels_var = get_query_ctx().get_or_create_var("." + var_name + "_labels");
        vars.insert({ expr.edge_id, labels_var, VarType::Edge });
    }


    void visit(ExprAnd& expr)
    {
        for (auto& expr : expr.exprs) {
            expr->accept_visitor(*this);
        }
    }

    void visit(ExprNot& expr) {
        negation = !negation;
        expr.expr->accept_visitor(*this);
        negation = !negation;
    }

    void visit(ExprOr&) { }
    void visit(ExprAddition&) { }
    void visit(ExprGreater&) { }
    void visit(ExprXor&) { }
    void visit(ExprConcat&) { }
    void visit(ExprGreaterOrEquals&) { }
    void visit(ExprLessOrEquals&) { }
    void visit(ExprLess&) { }
    void visit(ExprSubtraction&) { }
    void visit(ExprMultiplication&) { }
    void visit(ExprDivision&) { }
    void visit(ExprEquals&) { }
    void visit(ExprNotEquals&) { }
    void visit(ExprTerm&) { }
    void visit(ExprVar&) { }
    void visit(ExprIs&) { }
    void visit(ExprProperty&) { }
    void visit(ExprAbs&) { }
    void visit(ExprModulo&) { }
    void visit(ExprSin&) { }
    void visit(ExprCos&) { }
    void visit(ExprTan&) { }
    void visit(ExprCot&) { }
    void visit(ExprSinh&) { }
    void visit(ExprCosh&) { }
    void visit(ExprTanh&) { }
    void visit(ExprAsin&) { }
    void visit(ExprAcos&) { }
    void visit(ExprAtan&) { }
    void visit(ExprDegrees&) { }
    void visit(ExprRadians&) { }
    void visit(ExprLog&) { }
    void visit(ExprLog10&) { }
    void visit(ExprLn&) { }
    void visit(ExprExp&) { }
    void visit(ExprPower&) { }
    void visit(ExprSqrt&) { }
    void visit(ExprFloor&) { }
    void visit(ExprCeil&) { }
    void visit(ExprLength&) { }
    void visit(ExprSubStr&) { }
    void visit(ExprFold&) { }
    void visit(ExprSingleTrim&) { }
    void visit(ExprMultiTrim&) { }
    void visit(ExprNormalize&) { }
    void visit(ExprNullIf&) { }
    void visit(ExprCoalesce&) { }
    void visit(ExprSimpleCase&) { }
    void visit(ExprSearchedCase&) { }
    void visit(ExprCast&) { }
    void visit(ExprAggCountAll&) { }
    void visit(ExprAggCount&) { }
    void visit(ExprAggAvg&) { }
    void visit(ExprAggMax&) { }
    void visit(ExprAggMin&) { }
    void visit(ExprAggSum&) { }
    void visit(ExprAggStddevPop&) { }
    void visit(ExprAggStddevSamp&) { }
    void visit(ExprAggCollect&) { }
    void visit(ExprAggPercentileCont&) { }
    void visit(ExprAggPercentileDisc&) { }
};
