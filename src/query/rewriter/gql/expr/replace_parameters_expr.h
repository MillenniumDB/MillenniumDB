#pragma once

#include "query/parser/expr/gql/exprs.h"

namespace GQL {

class ReplaceParametersExpr : public ExprVisitor {
public:
    ReplaceParametersExpr(const std::map<VarId, ObjectId>& parameters_) :
        parameters { parameters_ }
    { }

    void visit_or_replace_parameter(std::unique_ptr<Expr>& expr)
    {
        if (auto expr_var = dynamic_cast<ExprVar*>(expr.get())) {
            const auto it = parameters.find(expr_var->id);
            if (it != parameters.end()) {
                // replace the variable by its parameter
                expr = std::make_unique<ExprTerm>(it->second);
            }
        } else {
            expr->accept_visitor(*this);
        }
    }

private:
    void visit(GQL::ExprAnd& expr) override
    {
        for (auto& e : expr.exprs) {
            visit_or_replace_parameter(e);
        }
    }

    void visit(GQL::ExprOr& expr) override
    {
        for (auto& e : expr.exprs) {
            visit_or_replace_parameter(e);
        }
    }

    void visit(GQL::ExprNot& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprHasNodeLabel& expr) override
    {
        // TODO:
    }

    void visit(GQL::ExprHasEdgeLabel& expr) override
    {
        // TODO:
    }

    void visit(GQL::ExprWildcardLabel& expr) override
    {
        // TODO:
    }

    void visit(GQL::ExprEquals& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprVar&) override
    {
        // should have been handled in visit_or_replace_parameter()
        assert(false);
    }

    void visit(GQL::ExprProperty&) override
    {
        // TODO:
    }

    void visit(GQL::ExprXor& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprConcat& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprGreaterOrEquals& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprGreater& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprIs& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprLessOrEquals& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprLess& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprNotEquals& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprTerm&) override { }

    void visit(GQL::ExprAddition& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprSubtraction& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprMultiplication& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprDivision& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprAbs& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprModulo& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprSin& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprCos& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprTan& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprCot& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprSinh& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprCosh& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprTanh& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprAsin& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprAcos& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprAtan& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprDegrees& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprRadians& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprLog& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprLog10& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprLn& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprExp& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprPower& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprSqrt& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprFloor& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprCeil& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprLength& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprSubStr& expr) override
    {
        visit_or_replace_parameter(expr.expr);
        visit_or_replace_parameter(expr.str_len);
    }

    void visit(GQL::ExprFold& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprSingleTrim& expr) override
    {
        visit_or_replace_parameter(expr.str);
        if (expr.single_char) {
            visit_or_replace_parameter(expr.single_char);
        }
    }

    void visit(GQL::ExprMultiTrim& expr) override
    {
        visit_or_replace_parameter(expr.trim_src);
        if (expr.delim_str) {
            visit_or_replace_parameter(expr.delim_str);
        }
    }

    void visit(GQL::ExprNormalize& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprNullIf& expr) override
    {
        visit_or_replace_parameter(expr.lhs);
        visit_or_replace_parameter(expr.rhs);
    }

    void visit(GQL::ExprCoalesce& expr) override
    {
        for (auto& e : expr.exprs) {
            visit_or_replace_parameter(e);
        }
    }

    void visit(GQL::ExprSimpleCase& expr) override
    {
        // TODO:
    }

    void visit(GQL::ExprSearchedCase&) override
    {
        // TODO:
    }

    void visit(GQL::ExprCast& expr) override
    {
        visit_or_replace_parameter(expr.expr);
    }

    void visit(GQL::ExprAggCountAll& expr) override
    {
        // TODO:
    }

    void visit(GQL::ExprAggCount&) override
    {
        // TODO:
    }

    void visit(GQL::ExprAggAvg&) override
    {
        // TODO:
    }
    void visit(GQL::ExprAggMax&) override
    {
        // TODO:
    }

    void visit(GQL::ExprAggMin&) override
    {
        // TODO:
    }

    void visit(GQL::ExprAggSum&) override
    {
        // TODO:
    }

    void visit(GQL::ExprAggStddevPop&) override
    {
        // TODO:
    }

    void visit(GQL::ExprAggStddevSamp&) override
    {
        // TODO:
    }

    void visit(GQL::ExprAggCollect&) override
    {
        // TODO:
    }

    void visit(GQL::ExprAggPercentileCont&) override
    {
        // TODO:
    }

    void visit(GQL::ExprAggPercentileDisc&) override
    {
        // TODO:
    }

    void visit(GQL::ExprLabels&) override
    {
        // TODO:
    }

    void visit(GQL::ExprProperties&) override
    {
        // TODO:
    }

    const std::map<VarId, ObjectId>& parameters;
};

} // namespace GQL