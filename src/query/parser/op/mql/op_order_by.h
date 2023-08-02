#pragma once

#include <string>
#include <vector>

#include "query/parser/op/op.h"

namespace MQL {

class OpOrderBy : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<VarId> items;

    std::vector<bool> ascending_order;

    OpOrderBy(
        std::unique_ptr<Op>  op,
        std::vector<VarId>&& items,
        std::vector<bool>&&  ascending_order
    ) :
        op              (std::move(op)),
        items           (std::move(items)),
        ascending_order (std::move(ascending_order)) { }

    virtual std::unique_ptr<Op> clone() const override {
        std::vector<VarId> items_clone;
        for (const auto& item : items) {
            items_clone.push_back(item);
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
        for (auto& var : items) {
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
        os << std::string(indent, ' ');
        os << "OpOrderBy(";
        for (unsigned i = 0; i < items.size(); i++) {
            if (i != 0) {
                os << ", ";
            }
            os << items[i] << (ascending_order[i] ? " ASC" : " DESC");
        }
        os << ")\n";
        return op->print_to_ostream(os, indent);
    }
};
} // namespace MQL