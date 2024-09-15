#pragma once

#include <optional>
#include <vector>

#include "query/parser/expr/expr.h"
#include "query/parser/op/op.h"

namespace MQL {

class OpGroupBy : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<VarId> vars;

    OpGroupBy(
        std::unique_ptr<Op> op,
        std::vector<VarId>&& vars
    ) :
        op   (std::move(op)),
        vars (std::move(vars)) { }

    virtual std::unique_ptr<Op> clone() const override {
        std::vector<VarId> vars_clone = vars;

        return std::make_unique<OpGroupBy>(
            op->clone(),
            std::move(vars_clone)
        );
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        auto res = op->get_all_vars();
        for (const auto& var : vars) {
            res.insert(var);
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        std::set<VarId> res;
        for (const auto& var : vars) {
            res.insert(var);
        }
        return res;
    }

    std::set<VarId> get_safe_vars() const override {
        return op->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpGroupBy(";

        ExprPrinter printer(os);

        auto first = true;
        for (auto&var : vars) {
            if (first) first = false; else os << ", ";

            os << '?' << get_query_ctx().get_var_name(var);
        }
        os << ")\n";

        for (size_t i = 0; i < printer.ops.size(); i++) {
            os << std::string(indent + 2, ' ') << "_Op_" << i << "_:\n";
            printer.ops[i]->print_to_ostream(os, indent + 4);
        }

        return op->print_to_ostream(os, indent + 2);
    }
};
} // namespace MQL
