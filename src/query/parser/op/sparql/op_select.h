#pragma once

#include <vector>

#include "query/parser/op/op.h"
#include "query/parser/expr/expr.h"

namespace SPARQL {

class OpSelect : public Op {
public:
    std::unique_ptr<Op>                op;
    std::vector<VarId>                 vars;
    std::vector<std::unique_ptr<Expr>> vars_exprs;

    const bool distinct;

    const uint64_t limit;
    const uint64_t offset;

    const bool is_sub_select;

    OpSelect(
        std::unique_ptr<Op>                op,
        std::vector<VarId>                 vars,
        std::vector<std::unique_ptr<Expr>> vars_exprs,
        bool                               distinct,
        uint64_t                           limit,
        uint64_t                           offset,
        bool                               is_sub_select
    ) :
        op            (std::move(op)),
        vars          (std::move(vars)),
        vars_exprs    (std::move(vars_exprs)),
        distinct      (distinct),
        limit         (limit),
        offset        (offset),
        is_sub_select (is_sub_select) { }

    std::unique_ptr<Op> clone() const override {
        std::vector<VarId> new_vars;
        new_vars.reserve(vars.size());
        for (auto& var : vars) {
            new_vars.push_back(var);
        }

        std::vector<std::unique_ptr<Expr>> new_vars_exprs;
        new_vars_exprs.reserve(vars_exprs.size());
        for (auto& expr : vars_exprs) {
            new_vars_exprs.push_back(expr->clone());
        }

        return std::make_unique<OpSelect>(
            op->clone(),
            std::move(new_vars),
            std::move(new_vars_exprs),
            distinct,
            limit,
            offset,
            is_sub_select
        );
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res = op->get_all_vars();
        for (size_t i = 0; i < vars.size(); i++) {
            auto& var = vars[i];
            auto& expr = vars_exprs[i];
            res.insert(var);
            if (expr) {
                for (auto&v : expr->get_all_vars()) {
                    res.insert(v);
                }
            }
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        std::set<VarId> res;

        for (auto& var : vars) {
            res.insert(var);
        }

        return res;
    }

    std::set<VarId> get_safe_vars() const override {
        return op->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override {
        if (limit != Op::DEFAULT_LIMIT or offset != Op::DEFAULT_OFFSET or distinct) {
            return {};
        }

        for (auto& expr : vars_exprs) {
            if (expr != nullptr and expr->has_aggregation()) {
                return {};
            }
        }

        std::set<VarId> res;
        auto op_fixable = op->get_fixable_vars();

        for (auto& var : vars) {
            if (op_fixable.find(var) != op_fixable.end()) {
                res.insert(var);
            }
        }

        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');

        if (is_sub_select) { os << "OpSubSelect("; }
        else { os << "OpSelect("; }

        if (distinct) {
            os << "DISTINCT ";
        }
        bool first = true;
        for (auto& var : vars) {
            if (!first) {
                os << ", ";
            }
            first = false;
            os << '?' << get_query_ctx().get_var_name(var);
        }
        if (limit != DEFAULT_LIMIT) {
            os << "; LIMIT " << limit;
        }
        if (offset != DEFAULT_OFFSET) {
            os << "; OFFSET " << offset;
        }
        os << ")\n";
        return op->print_to_ostream(os, indent + 2);
    }
};
} // namespace SPARQL
