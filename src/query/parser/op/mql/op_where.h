#pragma once

#include <memory>

#include "query/parser/expr/expr.h"
#include "query/parser/op/op.h"

namespace MQL {

class OpWhere : public Op {
public:
    std::unique_ptr<Op> op;

    std::unique_ptr<Expr> expr;

    OpWhere(
        std::unique_ptr<Op> op,
        std::unique_ptr<Expr> expr
    ) :
        op   (std::move(op)),
        expr (std::move(expr)) { }

    std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpWhere>(
            op->clone(),
            expr->clone()
        );
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        auto res = op->get_all_vars();
        for (auto& var : expr->get_all_vars()) {
            res.insert(var);
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        return op->get_scope_vars();
    }

    std::set<VarId> get_safe_vars() const override {
        return op->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override {
        return { };
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        auto printer = ExprPrinter(os);
        os << "OpWhere";
        expr->accept_visitor(printer);
        os << "\n";
        return op->print_to_ostream(os, indent);
    }
};
} // namespace MQL
