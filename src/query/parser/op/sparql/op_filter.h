#pragma once

#include <vector>

#include "query/parser/expr/sparql/expr.h"
#include "query/parser/expr/sparql/expr_printer.h"
#include "query/parser/op/sparql/op.h"

namespace SPARQL {

class OpFilter : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<std::unique_ptr<Expr>> filters;

    OpFilter(std::unique_ptr<Op> op, std::vector<std::unique_ptr<Expr>> filters) :
        op(std::move(op)),
        filters(std::move(filters))
    { }

    std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> new_filters;
        for (auto& expr : filters) {
            new_filters.push_back(expr->clone());
        }
        return std::make_unique<OpFilter>(op->clone(), std::move(new_filters));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        auto res = op->get_all_vars();
        for (auto& expr : filters) {
            auto vars = expr->get_all_vars();
            res.insert(vars.begin(), vars.end());
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override
    {
        return op->get_scope_vars();
    }

    std::set<VarId> get_safe_vars() const override
    {
        return op->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return op->get_fixable_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ') << "OpFilter(\n";
        ExprPrinter printer(os);
        for (auto& filter : filters) {
            os << std::string(indent + 2, ' ');
            filter->accept_visitor(printer);
            os << "\n";
        }

        for (size_t i = 0; i < printer.ops.size(); i++) {
            os << std::string(indent + 2, ' ') << "_Op_" << i << "_:\n";
            printer.ops[i]->print_to_ostream(os, indent + 4);
        }

        os << std::string(indent, ' ') << ")\n";

        op->print_to_ostream(os, indent + 2);
        return os;
    }
};
} // namespace SPARQL
