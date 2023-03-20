#pragma once

#include <memory>
#include <vector>

#include "parser/query/op/op.h"

namespace MDB {

class OpOptional : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<std::unique_ptr<Op>> optionals;

    // OpOptional(OpOptional&& op_optional) :
    //     op        (std::move(op_optional.op)),
    //     optionals (std::move(op_optional.optionals)) { }

    OpOptional(std::unique_ptr<Op> op, std::vector<std::unique_ptr<Op>>&& optionals) :
        op(std::move(op)), optionals(std::move(optionals)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        auto res = op->get_vars();
        for (auto& child : optionals) {
            for (auto& child_var : child->get_vars()) {
                res.insert(child_var);
            }
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpOptional()\n";
        os << std::string(indent + 2, ' ') << "main pattern:\n";
        op->print_to_ostream(os, indent + 4);
        os << std::string(indent + 2, ' ') << "children:\n";
        for (auto& optional : optionals) {
            optional->print_to_ostream(os, indent + 4);
        }
        return os;
    }
};
} // namespace MDB