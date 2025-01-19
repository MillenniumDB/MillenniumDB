#pragma once

#include <cassert>
#include <string>
#include <vector>

#include "query/parser/op/op.h"
#include "query/parser/expr/mql/atom_expr/expr_var.h"
#include "query/parser/expr/expr.h"

namespace MQL {

class OpOrderBy : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<std::variant<VarId, std::unique_ptr<Expr>>> items;

    std::vector<bool> ascending_order;

    OpOrderBy(
        std::unique_ptr<Op>  op,
        std::vector<std::variant<VarId, std::unique_ptr<Expr>>>&& _items,
        std::vector<bool>&& _ascending_order
    ) :
        op              (std::move(op)),
        ascending_order (std::move(_ascending_order))
    {
        assert(_items.size() == ascending_order.size());

        for (auto& item : _items) {
            if (std::holds_alternative<std::unique_ptr<Expr>>(item)) {
                auto casted_expr_var = dynamic_cast<ExprVar*>(std::get<std::unique_ptr<Expr>>(item).get());
                if (casted_expr_var) {
                    items.emplace_back(casted_expr_var->var);
                    continue;
                }
            }

            items.emplace_back(std::move(item));
        }
    }

    virtual std::unique_ptr<Op> clone() const override {
        std::vector<std::variant<VarId, std::unique_ptr<Expr>>> items_clone;
        items_clone.reserve(items.size());

        for (const auto& item : items) {
            if (std::holds_alternative<VarId>(item)) {
                items_clone.emplace_back(std::get<VarId>(item));
            } else {
                items_clone.emplace_back(std::get<std::unique_ptr<Expr>>(item)->clone());
            }
        }

        auto ascending_order_copy = ascending_order;
        return std::make_unique<OpOrderBy>(
            op->clone(),
            std::move(items_clone),
            std::move(ascending_order_copy)
        );
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res = op->get_all_vars();
        for (const auto& item : items) {
            if (std::holds_alternative<VarId>(item)) {
                res.insert(std::get<VarId>(item));
            } else {
                auto item_vars = std::get<std::unique_ptr<Expr>>(item)->get_all_vars();
                res.insert(item_vars.begin(), item_vars.end());
            }
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
        // for (auto& item : items) {
        //     if (std::holds_alternative<std::unique_ptr<Expr>>(item)) {
        //         auto expr = std::get<std::unique_ptr<Expr>>(item).get();
        //         if (expr->has_aggregation()) {
        //             return {};
        //         }
        //     }
        // }
        return op->get_fixable_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        ExprPrinter printer(os);

        os << std::string(indent, ' ');
        os << "OpOrderBy(";
        for (unsigned i = 0; i < items.size(); i++) {
            if (i != 0) {
                os << ", ";
            }

            if (std::holds_alternative<VarId>(items[i])) {
                os << get_query_ctx().get_var_name(std::get<VarId>(items[i]));
            } else {
                auto expr = std::get<std::unique_ptr<Expr>>(items[i]).get();
                expr->accept_visitor(printer);
            }

            os << (ascending_order[i] ? " ASC" : " DESC");
        }
        os << ")\n";
        return op->print_to_ostream(os, indent);
    }
};
} // namespace MQL