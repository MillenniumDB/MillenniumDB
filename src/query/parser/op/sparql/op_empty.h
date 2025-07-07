#pragma once

#include <memory>
#include <set>
#include <optional>

#include "query/parser/op/sparql/op.h"

namespace SPARQL {

class OpEmpty : public Op {
public:
    std::optional<std::unique_ptr<Op>> deleted_op;

    OpEmpty() { }

    OpEmpty(std::unique_ptr<Op>& op) {
        deleted_op = std::move(op);
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::unique_ptr<Op> clone() const override {
        if (deleted_op.has_value()) {
            auto op = deleted_op.value()->clone();
            return std::make_unique<OpEmpty>(op);
        }
        return std::make_unique<OpEmpty>();
    }

    std::set<VarId> get_all_vars() const override {
        if (deleted_op.has_value()) {
            return deleted_op.value()->get_all_vars();
        }
        return {};
    }

    std::set<VarId> get_scope_vars() const override {
        return {};
    }

    std::set<VarId> get_safe_vars() const override {
        return {};
    }

    std::set<VarId> get_fixable_vars() const override {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        if (deleted_op.has_value()) {
            os << "OpEmpty(Deleted)\n";
            deleted_op.value()->print_to_ostream(os, indent + 2);
        }
        else {
            os << "OpEmpty()\n";
        }
        return os;
    }
};
} // namespace SPARQL
