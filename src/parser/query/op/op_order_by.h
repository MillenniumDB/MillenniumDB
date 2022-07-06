#pragma once

#include <string>
#include <vector>

#include "parser/query/op/op.h"
#include "parser/query/return_item/return_item.h"

class OpOrderBy : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<std::unique_ptr<ReturnItem>> items;

    std::vector<bool> ascending_order;

    OpOrderBy(std::unique_ptr<Op> op,
              std::vector<std::unique_ptr<ReturnItem>>&& items,
              std::vector<bool>&& ascending_order) :
        op              (std::move(op)),
        items           (std::move(items)),
        ascending_order (std::move(ascending_order)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        auto res = op->get_vars();
        for (const auto& item : items) {
            res.insert(item->get_var());
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpOrderBy(";
        for (unsigned i = 0; i < items.size(); i++) {
            if (i != 0) {
                os << ", ";
            }
            os << *items[i] << (ascending_order[i] ? " ASC" : " DESC");
        }
        os << ")\n";
        return op->print_to_ostream(os, indent);
    }
};
