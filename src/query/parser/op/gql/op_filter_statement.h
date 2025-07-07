#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/gql/expr.h"
#include "query/parser/expr/gql/expr_printer.h"
#include "query/parser/op/gql/op.h"

namespace GQL {
class OpFilterStatement : public Op {
public:
    std::vector<std::unique_ptr<Expr>> exprs;

    OpFilterStatement(std::vector<std::unique_ptr<Expr>>&& expr_list) :
        exprs(std::move(expr_list))
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> and_list_clone;
        and_list_clone.reserve(exprs.size());
        for (auto& child_expr : exprs) {
            and_list_clone.push_back(child_expr->clone());
        }
        return std::make_unique<OpFilterStatement>(std::move(and_list_clone));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpFilterStatement(\n";

        ExprPrinter printer(os);

        for (auto& expr : exprs) {
            os << std::string(indent + 2, ' ');
            expr->accept_visitor(printer);
            os << "\n";
        }

        os << std::string(indent, ' ') << ")\n";
        return os;
    }
};
} // namespace GQL
