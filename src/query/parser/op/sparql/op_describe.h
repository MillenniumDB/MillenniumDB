#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "query/parser/op/op.h"

namespace SPARQL {

class OpDescribe : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<VarId> vars;
    std::vector<std::string> iris;

    const uint64_t offset;
    const uint64_t limit;

    OpDescribe(
        std::unique_ptr<Op>        op,
        std::vector<VarId>&&       vars,
        std::vector<std::string>&& iris,
        uint64_t                   offset,
        uint64_t                   limit
    ) :
        op     (std::move(op)),
        vars   (std::move(vars)),
        iris   (std::move(iris)),
        offset (offset),
        limit  (limit) { }

    std::unique_ptr<Op> clone() const override {
        std::unique_ptr<Op> op_clone;
        if (op) {
            op_clone = op->clone();
        } else {
            op_clone = nullptr;
        }
        auto vars_clone = vars;
        auto iris_clone = iris;

        return std::make_unique<OpDescribe>(
            std::move(op_clone),
            std::move(vars_clone),
            std::move(iris_clone),
            offset,
            limit
        );
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;

        if (op) {
            res = op->get_all_vars();
        }

        for (auto& var : vars) {
            res.insert(var);
        }

        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        return {};
    }

    std::set<VarId> get_safe_vars() const override {
        return {};
    }

    std::set<VarId> get_fixable_vars() const override {
        if (op) {
            return op->get_fixable_vars();
        } else {
            return {};
        }
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpDescribe(";

        auto first = true;
        for (auto& var : vars) {
            if (first) first = false; else os << ", ";

            os << "?" << get_query_ctx().get_var_name(var);
        }

        for (auto& iri : iris) {
            if (first) first = false; else os << ", ";
            os << iri;
        }

        if (limit != Op::DEFAULT_LIMIT) {
            os << ", limit " << limit;
        }
        if (offset != Op::DEFAULT_OFFSET) {
            os << ", offset " << offset;
        }

        os << ")\n";

        if (op) {
            op->print_to_ostream(os, indent + 2);
        }

        return os;
    }
};
} // namespace SPARQL
