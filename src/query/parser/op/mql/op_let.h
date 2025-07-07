#pragma once

#include "query/parser/expr/mql/expr.h"
#include "query/parser/expr/mql/expr_printer.h"
#include "query/parser/op/mql/op.h"

namespace MQL {

class OpLet : public Op {
public:
    using VarExprType = std::vector<std::pair<VarId, std::unique_ptr<Expr>>>;

    VarExprType var_expr;

    OpLet(VarExprType&& var_expr_) :
        var_expr { std::move(var_expr_) }
    {
        assert(!var_expr.empty());
    }

    std::unique_ptr<Op> clone() const override
    {
        VarExprType var_expr_clone;
        var_expr_clone.reserve(var_expr.size());
        for (const auto& [var, expr] : var_expr) {
            var_expr_clone.emplace_back(var, expr->clone());
        }
        return std::make_unique<OpLet>(std::move(var_expr_clone));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;
        for (const auto& [var, expr] : var_expr) {
            res.emplace(var);
            for (const auto& expr_var : expr->get_all_vars()) {
                res.emplace(expr_var);
            }
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        ExprPrinter printer(os);

        os << std::string(indent, ' ') << "OpLet(";

        os << get_query_ctx().get_var_name(var_expr[0].first) << "=";
        var_expr[0].second->accept_visitor(printer);

        for (size_t i = 1; i < var_expr.size(); i++) {
            os << ", ";
            os << get_query_ctx().get_var_name(var_expr[i].first) << "=";
            var_expr[i].second->accept_visitor(printer);
        }

        return os << ")\n";
    }
};
} // namespace MQL
