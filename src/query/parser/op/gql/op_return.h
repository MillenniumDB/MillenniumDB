#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/gql/expr.h"
#include "query/parser/expr/gql/expr_printer.h"
#include "query/parser/op/gql/op.h"
#include "query/parser/op/gql/op_order_by_statement.h"

namespace GQL {

class OpReturn : public Op {
public:
    struct Item {
        Item(std::unique_ptr<Expr> expr, std::optional<VarId> alias) :
            expr(std::move(expr)),
            alias(alias)
        { }

        std::unique_ptr<Expr> expr;
        std::optional<VarId> alias;
    };

    std::unique_ptr<Op> op;
    std::vector<Item> return_items;
    bool distinct;

    // might be nullptr
    std::unique_ptr<Op> op_order_by;

    OpReturn(
        std::unique_ptr<Op> op,
        std::vector<Item>&& return_items,
        bool distinct,
        std::unique_ptr<Op> op_order_by = nullptr
    ) :
        op(std::move(op)),
        return_items(std::move(return_items)),
        distinct(distinct),
        op_order_by(std::move(op_order_by))
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<Item> return_items_clone;
        return_items_clone.reserve(return_items.size());
        for (auto& item : return_items) {
            return_items_clone.emplace_back(item.expr->clone(), item.alias);
        }

        std::unique_ptr<Op> order_by_clone = op_order_by == nullptr ? nullptr : op_order_by->clone();

        return std::make_unique<OpReturn>(
            op->clone(),
            std::move(return_items_clone),
            distinct,
            op_order_by->clone()
        );
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res = op->get_all_vars();
        for (auto& item : return_items) {
            for (auto& var : item.expr->get_all_vars()) {
                res.insert(var);
            }
        }
        return res;
    }

    std::vector<VarId> get_expr_vars() const
    {
        std::vector<VarId> result;

        for (auto& item : return_items) {
            if (item.alias.has_value()) {
                result.push_back(item.alias.value());
            } else {
                auto expr_variables = item.expr->get_all_vars();
                result.insert(result.end(), expr_variables.begin(), expr_variables.end());
            }
        }
        return result;
    }

    std::map<VarId, GQL::VarType> get_var_types() const override
    {
        return op->get_var_types();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpReturn(\n";
        ExprPrinter printer(os);

        for (auto& item : return_items) {
            os << std::string(indent + 2, ' ');
            item.expr->accept_visitor(printer);
            if (item.alias.has_value()) {
                os << " AS " << item.alias.value();
            }
            os << "\n";
        }

        os << std::string(indent, ' ') << ")";
        os << "\n";
        return op->print_to_ostream(os, indent + 2);
    }
};
} // namespace GQL
