#pragma once

#include <string>
#include <vector>

#include "base/query/var.h"
#include "parser/query/op/op.h"

class OpGroupBy : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<Var> items;

    OpGroupBy(std::unique_ptr<Op> op,
              std::vector<Var>&& items) :
        op    (std::move(op)),
        items (std::move(items)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        auto res = op->get_vars();
        for (const auto& var : items) {
            res.insert(var);
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpGroupBy(";
        for (unsigned i = 0; i < items.size(); i++) {
            if (i != 0) {
                os << ", ";
            }
            os << items[i].name;
        }
        os << ")\n";
        return op->print_to_ostream(os, indent + 2);
    }
};
