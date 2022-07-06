#pragma once

#include <memory>
#include <vector>

#include "base/query/query_element.h"
#include "parser/query/op/op.h"

class OpSet : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<std::pair<Var, QueryElement>> set_items;

    OpSet(std::unique_ptr<Op> op, std::vector<std::pair<Var, QueryElement>>&& set_items) :
        op        (std::move(op)),
        set_items (std::move(set_items)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        auto res = op->get_vars();
        for (auto& set_item : set_items) {
            res.insert(set_item.first);
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpSet()\n";
        return op->print_to_ostream(os, indent);
    };
};
