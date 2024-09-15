#pragma once

#include <vector>

#include "query/parser/expr/expr.h"
#include "query/parser/op/op.h"

namespace SPARQL {

class OpHaving : public Op {
public:
    std::unique_ptr<Op>                op;
    std::vector<std::unique_ptr<Expr>> exprs;

    OpHaving(
        std::unique_ptr<Op>                  op,
        std::vector<std::unique_ptr<Expr>>&& exprs
    ) :
        op    (std::move(op)),
        exprs (std::move(exprs)) { }

    std::unique_ptr<Op> clone() const override {
        std::vector<std::unique_ptr<Expr>> new_exprs;
        new_exprs.reserve(exprs.size());
        for (auto& expr : exprs) {
            new_exprs.push_back(expr->clone());
        }

        return std::make_unique<OpHaving>(op->clone(), std::move(new_exprs));
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        auto res = op->get_all_vars();
        for (auto& expr : exprs) {
            for (auto& var: expr->get_all_vars()) {
                res.insert(var);
            }
        }
        return res;
    }

    std::set<VarId> get_safe_vars() const override {
        return op->get_safe_vars();
    }

    std::set<VarId> get_scope_vars() const override {
        return op->get_scope_vars();
    }

    std::set<VarId> get_fixable_vars() const override {
        for (auto& expr : exprs) {
            if (expr->has_aggregation()) {
                return {};
            }
        }
        return op->get_fixable_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpHaving(";

        ExprPrinter printer(os);

        auto first = true;
        for (auto& expr : exprs) {
            if (first) first = false; else os << ", ";
            expr->accept_visitor(printer);
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
