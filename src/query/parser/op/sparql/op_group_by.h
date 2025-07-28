#pragma once

#include <optional>
#include <vector>

#include "query/parser/expr/sparql/expr.h"
#include "query/parser/expr/sparql/expr_printer.h"
#include "query/parser/op/sparql/op.h"

namespace SPARQL {

class OpGroupBy : public Op {
public:
    std::unique_ptr<Op> op;

    // list of <expr, alias>
    std::vector<std::pair<std::unique_ptr<Expr>, std::optional<VarId>>> items;

    OpGroupBy(
        std::unique_ptr<Op> op,
        std::vector<std::pair<std::unique_ptr<Expr>, std::optional<VarId>>> items
    ) :
        op(std::move(op)),
        items(std::move(items))
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<std::pair<std::unique_ptr<Expr>, std::optional<VarId>>> new_items;

        new_items.reserve(items.size());
        for (const auto& [expr, alias] : items) {
            assert(expr != nullptr);
            new_items.emplace_back(expr->clone(), alias);
        }

        return std::make_unique<OpGroupBy>(op->clone(), std::move(new_items));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        auto res = op->get_all_vars();
        for (const auto& [expr, alias] : items) {
            if (alias) {
                res.insert(*alias);
            }
            if (expr) {
                for (auto& v : expr->get_all_vars()) {
                    res.insert(v);
                }
            }
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override
    {
        std::set<VarId> res;
        for (const auto& [expr, alias] : items) {
            if (alias) {
                res.insert(*alias);
            }
        }
        return res;
    }

    std::set<VarId> get_safe_vars() const override
    {
        return op->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ') << "OpGroupBy(";

        ExprPrinter printer(os);

        auto first = true;
        for (auto& [expr, alias] : items) {
            if (first)
                first = false;
            else
                os << ", ";

            expr->accept_visitor(printer);
            if (alias) {
                os << " AS " << get_query_ctx().get_var_name(*alias);
            }
            assert(expr != nullptr);
        }
        os << ")\n";

        for (size_t i = 0; i < printer.ops.size(); i++) {
            os << std::string(indent + 2, ' ') << "_Op_" << i << "_:\n";
            printer.ops[i]->print_to_ostream(os, indent + 4);
        }

        return op->print_to_ostream(os, indent + 2);
    }
};
} // namespace SPARQL
