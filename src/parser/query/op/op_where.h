#pragma once

#include <memory>

#include "parser/query/op/op.h"
#include "parser/query/expr/expr.h"

class OpWhere : public Op {
public:
    std::unique_ptr<Op> op;

    std::unique_ptr<Expr> expr;

    OpWhere(std::unique_ptr<Op> op, std::unique_ptr<Expr> expr) :
        op   (std::move(op)),
        expr (std::move(expr)) { }


    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        auto res = op->get_vars();
        for (auto& var : expr->get_vars()) {
            res.insert(var);
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpWhere(" << *expr << ")\n";
        return op->print_to_ostream(os, indent);
    }
};
