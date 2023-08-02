#pragma once

#include <cassert>
#include <vector>

#include "query/parser/expr/expr.h"
#include "query/parser/op/op.h"

namespace SPARQL {

class OpValues : public Op {
public:
    std::vector<VarId> vars;
    std::vector<bool> has_undef;
    std::vector<ObjectId> values;

    OpValues(
        std::vector<VarId>&& _vars,
        std::vector<ObjectId>&& _values
    ) :
        vars   (std::move(_vars)),
        values (std::move(_values))
    {
        assert(values.size() % vars.size() == 0);
        has_undef = std::vector<bool>(vars.size(), false);
        for(size_t row = 0; row < values.size() / vars.size(); row++) {
            for (size_t var = 0; var < vars.size(); var++) {
                auto i = row * vars.size() + var;
                if (values[i].is_null()) {
                    has_undef[var] = true;
                }
            }
        }
    }

    std::unique_ptr<Op> clone() const override {
        std::vector<VarId> vars_clone;
        std::vector<ObjectId> values_clone;

        vars_clone.reserve(vars.size());
        for (auto& var : vars) {
            vars_clone.push_back(var);
        }

        values_clone.reserve(values.size());
        for (auto& value : values) {
            values_clone.push_back(value);
        }

        return std::make_unique<OpValues>(
            std::move(vars_clone),
            std::move(values_clone)
        );
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;

        for (auto& var : vars) {
            res.insert(var);
        }

        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override {
        std::set<VarId> res;
        for (size_t var = 0; var < vars.size(); var++) {
            if (!has_undef[var]) {
                res.insert(vars[var]);
            }
        }
        return res;
    }

    std::set<VarId> get_fixable_vars() const override {
        return get_safe_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpValues(";

        auto first = true;
        for (auto& var : vars) {
            if (first) {
                first = false;
            } else {
                os << ", ";
            }
            os << "?" << get_query_ctx().get_var_name(var);
        }
        os << ")\n";
        return os;
    }
};
} // namespace SPARQL
