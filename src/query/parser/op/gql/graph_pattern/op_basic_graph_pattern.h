#pragma once

#include <set>
#include <vector>

#include "query/exceptions.h"

#include "query/parser/op/op.h"

namespace GQL {

class OpBasicGraphPattern : public Op {
public:
    std::vector<std::unique_ptr<Op>> patterns;

    OpBasicGraphPattern() { }

    OpBasicGraphPattern(std::vector<std::unique_ptr<Op>>&& patterns) :
        patterns(std::move(patterns))
    { }

    std::unique_ptr<Op> clone() const override
    {
        auto basic_graph_pattern = std::make_unique<OpBasicGraphPattern>();
        for (auto& pattern : patterns) {
            basic_graph_pattern->add_pattern(pattern->clone());
        }
        return basic_graph_pattern;
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void add_pattern(std::unique_ptr<Op>&& pattern)
    {
        patterns.push_back(std::move(pattern));
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;

        for (auto& pattern : patterns) {
            auto pattern_vars = pattern->get_all_vars();
            for (auto& var : pattern_vars) {
                res.insert(var);
            }
        }

        return res;
    }

    std::set<VarId> get_scope_vars() const override
    {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override
    {
        return get_all_vars();
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return get_all_vars();
    }

    std::map<VarId, std::unique_ptr<VarType>> get_var_types() const override
    {
        std::map<VarId, std::unique_ptr<VarType>> types_map;

        for (auto& pattern : patterns) {
            for (auto& [var, type] : pattern->get_var_types()) {
                if (types_map.count(var) && *types_map[var] != *type) {
                    throw QuerySemanticException(
                        "Variable \"" + get_query_ctx().get_var_name(var) + "\" has more than one type."
                    );
                }
                types_map[var] = std::move(type);
            }
        }
        return types_map;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpBasicGraphPattern()\n";

        for (auto& pattern : patterns) {
            pattern->print_to_ostream(os, indent + 2);
        }

        return os;
    }
};
} // namespace GQL
