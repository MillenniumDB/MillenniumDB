#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/expr.h"
#include "query/parser/expr/gql_expr_printer.h"
#include "query/parser/op/op.h"

namespace GQL {
class OpFilter : public Op {
public:
    std::unique_ptr<Op> op;
    std::vector<std::unique_ptr<Expr>> exprs;

    OpFilter(std::unique_ptr<Op>&& op, std::vector<std::unique_ptr<Expr>>&& expr_list) :
        op(std::move(op)),
        exprs(std::move(expr_list))
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> and_list_clone;
        and_list_clone.reserve(exprs.size());
        for (auto& child_expr : exprs) {
            and_list_clone.push_back(child_expr->clone());
        }
        return std::make_unique<OpFilter>(op->clone(), std::move(and_list_clone));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res = op->get_all_vars();
        for (auto& expr : exprs) {
            for (auto& var : expr->get_all_vars()) {
                res.insert(var);
            }
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override
    {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override
    {
        return op->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return get_all_vars();
    }

    std::map<VarId, std::unique_ptr<GQL::VarType>> get_var_types() const override
    {
        return op->get_var_types();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpFilter(\n";

        ExprPrinter printer(os);

        for (auto& expr : exprs) {
            os << std::string(indent + 2, ' ');
            expr->accept_visitor(printer);
            os << "\n";
        }

        os << std::string(indent, ' ') << ")\n";
        op->print_to_ostream(os, indent + 2);
        return os;
    }
};
} // namespace GQL
