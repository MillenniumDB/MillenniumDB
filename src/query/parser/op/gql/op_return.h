#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/expr.h"
#include "query/parser/expr/gql_expr_printer.h"
#include "query/parser/op/op.h"

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

    OpReturn(std::unique_ptr<Op> op, std::vector<Item>&& return_items) :
        op(std::move(op)),
        return_items(std::move(return_items))
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<Item> return_items_clone;
        return_items_clone.reserve(return_items.size());
        for (auto& item : return_items) {
            return_items_clone.emplace_back(item.expr->clone(), item.alias);
        }
        return std::make_unique<OpReturn>(op->clone(), std::move(return_items_clone));
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

    std::set<VarId> get_expr_vars() const
    {
        std::set<VarId> result;

        for (auto& item : return_items) {
            if (item.alias.has_value()) {
                result.insert(item.alias.value());
            } else {
                result.merge(item.expr->get_all_vars());
            }
        }
        return result;
    }

    std::set<VarId> get_scope_vars() const override
    {
        return {};
    }

    std::set<VarId> get_safe_vars() const override
    {
        return {};
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return {};
    }

    std::map<VarId, std::unique_ptr<GQL::VarType>> get_var_types() const override
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
