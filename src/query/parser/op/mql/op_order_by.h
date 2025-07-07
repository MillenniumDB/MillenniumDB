#pragma once

#include <cassert>
#include <string>
#include <vector>

#include "query/parser/expr/mql/expr.h"
#include "query/parser/expr/mql/expr_printer.h"
#include "query/parser/op/mql/op.h"

namespace MQL {

class OpOrderBy : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<std::unique_ptr<Expr>> items;

    std::vector<bool> ascending_order;

    OpOrderBy(
        std::unique_ptr<Op> op,
        std::vector<std::unique_ptr<Expr>>&& _items,
        std::vector<bool>&& _ascending_order
    ) :
        op(std::move(op)),
        items(std::move(_items)),
        ascending_order(std::move(_ascending_order))
    {
        assert(items.size() == ascending_order.size());
    }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> items_clone;

        for (const auto& item : items) {
            items_clone.emplace_back(item->clone());
        }

        auto ascending_order_copy = ascending_order;
        return std::make_unique<OpOrderBy>(
            op->clone(),
            std::move(items_clone),
            std::move(ascending_order_copy)
        );
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res = op->get_all_vars();
        for (const auto& item : items) {
            auto item_vars = item->get_all_vars();
            res.insert(item_vars.begin(), item_vars.end());
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        ExprPrinter printer(os);

        os << std::string(indent, ' ') << "OpOrderBy(";
        for (unsigned i = 0; i < items.size(); i++) {
            if (i != 0) {
                os << ", ";
            }

            items[i]->accept_visitor(printer);

            os << (ascending_order[i] ? " ASC" : " DESC");
        }
        os << ")\n";
        return op->print_to_ostream(os, indent);
    }
};
} // namespace MQL
