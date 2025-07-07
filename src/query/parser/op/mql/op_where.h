#pragma once

#include "query/parser/expr/mql/expr.h"
#include "query/parser/op/mql/op.h"

namespace MQL {

class OpWhere : public Op {
public:
    std::unique_ptr<Op> op;

    std::unique_ptr<Expr> expr;

    OpWhere(std::unique_ptr<Op> op, std::unique_ptr<Expr> expr) :
        op(std::move(op)),
        expr(std::move(expr))
    { }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpWhere>(op->clone(), expr->clone());
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        auto res = op->get_all_vars();
        for (auto& var : expr->get_all_vars()) {
            res.insert(var);
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        auto printer = ExprPrinter(os);
        os << "OpWhere(";
        expr->accept_visitor(printer);
        os << ")\n";
        return op->print_to_ostream(os, indent + 2);
    }
};
} // namespace MQL
