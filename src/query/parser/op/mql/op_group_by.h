#pragma once

#include <vector>

#include "query/parser/expr/mql/expr.h"
#include "query/parser/expr/mql/expr_printer.h"
#include "query/parser/op/mql/op.h"

namespace MQL {

class OpGroupBy : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<std::unique_ptr<Expr>> exprs;

    OpGroupBy(std::unique_ptr<Op> op, std::vector<std::unique_ptr<Expr>>&& exprs) :
        op(std::move(op)),
        exprs(std::move(exprs))
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> exprs_clone;

        for (auto& e : exprs) {
            exprs_clone.push_back(e->clone());
        }

        return std::make_unique<OpGroupBy>(op->clone(), std::move(exprs_clone));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        auto res = op->get_all_vars();
        for (const auto& e : exprs) {
            for (auto var : e->get_all_vars()) {
                res.insert(var);
            }
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ') << "OpGroupBy(";

        ExprPrinter printer(os);

        auto first = true;
        for (const auto& e : exprs) {
            if (first)
                first = false;
            else
                os << ", ";

            e->accept_visitor(printer);
        }
        os << ")\n";

        for (size_t i = 0; i < printer.ops.size(); i++) {
            os << std::string(indent + 2, ' ') << "_Op_" << i << "_:\n";
            printer.ops[i]->print_to_ostream(os, indent + 4);
        }

        return op->print_to_ostream(os, indent + 2);
    }
};
} // namespace MQL
