#pragma once

#include <cstddef>
#include <vector>
#include <variant>

#include "query/parser/expr/sparql/expr.h"
#include "query/parser/expr/sparql/atom/expr_var.h"
#include "query/parser/op/sparql/op.h"


namespace SPARQL {

class OpOrderBy : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<std::variant<VarId, std::unique_ptr<Expr>>> items;

    std::vector<bool> ascending_order;

    OpOrderBy(
        std::unique_ptr<Op>                                       op,
        std::vector<std::variant<VarId, std::unique_ptr<Expr>>>&& _items,
        std::vector<bool>&&                                       ascending_order
    ) :
        op              (std::move(op)),
        ascending_order (std::move(ascending_order))
    {
        for (auto& item : _items) {
            if (std::holds_alternative<std::unique_ptr<Expr>>(item)) {
                auto casted_expr_var = dynamic_cast<ExprVar*>(std::get<std::unique_ptr<Expr>>(item).get());
                if (casted_expr_var) {
                    items.push_back(casted_expr_var->var);
                    continue;
                }
            }
            items.push_back(std::move(item));
        }
    }

    std::unique_ptr<Op> clone() const  override {
        std::vector<std::variant<VarId, std::unique_ptr<Expr>>> new_items;

        new_items.reserve(items.size());
        for (const auto& item : items) {
            if (std::holds_alternative<VarId>(item)) {
                new_items.push_back(std::get<VarId>(item));
            } else {
                new_items.push_back(std::get<std::unique_ptr<Expr>>(item)->clone());
            }
        }

        std::vector<bool> new_ascending_order;
        new_ascending_order.reserve(ascending_order.size());
        for (auto asc : ascending_order) {
            new_ascending_order.push_back(asc);
        }

        return std::make_unique<OpOrderBy>(
            op->clone(),
            std::move(new_items),
            std::move(new_ascending_order)
        );
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res = op->get_all_vars();
        for (auto& item : items) {
            if (std::holds_alternative<VarId>(item)) {
                res.insert(std::get<VarId>(item));
            } else {
                for (auto& v : std::get<std::unique_ptr<Expr>>(item)->get_all_vars()) {
                    res.insert(v);
                }
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
        for (auto& item : items) {
            if (std::holds_alternative<std::unique_ptr<Expr>>(item)) {
                auto expr = std::get<std::unique_ptr<Expr>>(item).get();
                if (expr->has_aggregation()) {
                    return {};
                }
            }
        }
        return op->get_fixable_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpOrderBy(";

        ExprPrinter printer(os);

        for (unsigned i = 0; i < items.size(); i++) {
            if (i != 0) os << ", ";

            os << (ascending_order[i] ? "ASC " : "DESC ");

            auto& item = items[i];

            if (std::holds_alternative<VarId>(item)) {
                os << get_query_ctx().get_var_name(std::get<VarId>(item));
            } else {
                auto expr = std::get<std::unique_ptr<Expr>>(item).get();
                expr->accept_visitor(printer);
            }
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
