#pragma once

#include "query/parser/op/op.h"

namespace GQL {

class OpLinearPattern : public Op {
public:
    std::vector<std::unique_ptr<Op>> patterns;
    std::unique_ptr<VarId> start;
    std::unique_ptr<VarId> end;

    OpLinearPattern() { }

    OpLinearPattern(
        std::vector<std::unique_ptr<Op>>&& patterns,
        std::unique_ptr<VarId> start,
        std::unique_ptr<VarId> end
    ) :
        patterns(std::move(patterns)),
        start(std::move(start)),
        end(std::move(end))
    { }

    std::unique_ptr<Op> clone() const override
    {
        auto linear_pattern = std::make_unique<OpLinearPattern>();
        for (auto& pattern : patterns) {
            linear_pattern->add_pattern(pattern->clone());
        }
        return linear_pattern;
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
            res.merge(pattern_vars);
        }

        return res;
    }

    std::set<VarId> get_scope_vars() const override
    {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override
    {
        std::set<VarId> res;
        for (auto& pattern : patterns) {
            res.merge(pattern->get_safe_vars());
        }
        return res;
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
        os << "OpLinearPattern(";
        if (start != nullptr && end != nullptr) {
            os << "start:" << *start << ", end:" << *end;
        } else if (start != nullptr) {
            os << "start:" << *start;
        } else if (end != nullptr) {
            os << "end:" << *end;
        }
        os << ")\n";

        for (auto& pattern : patterns) {
            pattern->print_to_ostream(os, indent + 2);
        }
        os.flush();
        return os;
    }
};
} // namespace GQL
