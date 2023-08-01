#pragma once

#include <vector>

#include "graph_models/object_id.h"
#include "query/parser/op/op.h"

namespace MQL {

class OpSet : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<std::pair<VarId, ObjectId>> set_items;

    OpSet(
        std::unique_ptr<Op>                       op,
        std::vector<std::pair<VarId, ObjectId>>&& set_items
    ) :
        op        (std::move(op)),
        set_items (std::move(set_items)) { }

    std::unique_ptr<Op> clone() const override {
        auto set_items_clone = set_items;
        return std::make_unique<OpSet>(op->clone(), std::move(set_items_clone));
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        auto res = op->get_all_vars();
        for (auto& set_item : set_items) {
            res.insert(set_item.first);
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
        return op->get_fixable_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpSet()\n";
        return op->print_to_ostream(os, indent);
    }
};
} // namespace MQL
