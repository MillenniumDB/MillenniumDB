#pragma once

#include "query/parser/expr/sparql/expr.h"
#include "query/parser/expr/sparql/expr_printer.h"
#include "query/parser/op/sparql/op.h"

namespace SPARQL {

class OpBind : public Op {
public:
    std::unique_ptr<Op> op;
    VarId var;
    std::unique_ptr<Expr> expr;

    OpBind(std::unique_ptr<Op> op, VarId var, std::unique_ptr<Expr> expr) :
        op(std::move(op)),
        var(std::move(var)),
        expr(std::move(expr))
    { }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpBind>(op->clone(), var, expr->clone());
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        auto vars = op->get_all_vars();
        vars.insert(var);

        for (auto& var_ : expr->get_all_vars()) {
            vars.insert(var_);
        }

        return vars;
    }

    virtual std::set<VarId> get_scope_vars() const override
    {
        auto res = op->get_scope_vars();
        res.insert(var);
        return res;
    }

    virtual std::set<VarId> get_safe_vars() const override
    {
        return op->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return op->get_fixable_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ') << "OpBind(?";
        os << get_query_ctx().get_var_name(var) << "=";
        ExprPrinter printer(os);
        expr->accept_visitor(printer);
        os << ")\n";

        for (size_t i = 0; i < printer.ops.size(); i++) {
            os << std::string(indent + 2, ' ') << "_Op_" << i << "_:\n";
            printer.ops[i]->print_to_ostream(os, indent + 4);
        }

        return op->print_to_ostream(os, indent + 2);
    }
};
} // namespace SPARQL
