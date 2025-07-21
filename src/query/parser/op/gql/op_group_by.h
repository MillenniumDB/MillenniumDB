#pragma once

#include "query/parser/expr/gql/expr.h"
#include "query/parser/expr/gql/expr_printer.h"
#include "query/parser/op/gql/op.h"

namespace GQL {

class OpGroupBy : public Op {
public:
    std::unique_ptr<Op> op;
    std::vector<std::unique_ptr<Expr>> exprs;

    OpGroupBy(std::unique_ptr<Op> op, std::vector<std::unique_ptr<Expr>> exprs) :
        op(std::move(op)),
        exprs(std::move(exprs))
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> clone_exprs;
        for (auto& expr : exprs) {
            clone_exprs.push_back(expr->clone());
        }
        return std::make_unique<OpGroupBy>(op->clone(), std::move(clone_exprs));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return op->get_all_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpGroupBy(";

        bool first = true;

        ExprPrinter expr_printer(os);

        for (auto& expr : exprs) {
            if (first) {
                first = false;
            } else {
                os << ", ";
            }
            expr->accept_visitor(expr_printer);
        }

        os << ")\n";
        op->print_to_ostream(os, indent + 2);
        return os;
    }
};
} // namespace GQL
