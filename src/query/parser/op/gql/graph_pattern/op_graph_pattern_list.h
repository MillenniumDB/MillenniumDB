#pragma once

#include "query/parser/op/gql/op.h"

namespace GQL {

class OpGraphPatternList : public Op {
public:
    std::vector<std::unique_ptr<Op>> patterns;

    OpGraphPatternList(std::vector<std::unique_ptr<Op>>&& patterns) :
        patterns(std::move(patterns))
    { }

    std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Op>> patterns_clone;
        patterns_clone.reserve(patterns.size());
        for (auto& pattern : patterns) {
            patterns_clone.push_back(pattern->clone());
        }
        return std::make_unique<OpGraphPatternList>(std::move(patterns_clone));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;
        for (auto& pattern : patterns) {
            res.merge(pattern->get_all_vars());
        }
        return res;
    }

    std::map<VarId, GQL::VarType> get_var_types() const override
    {
        std::map<VarId, GQL::VarType> result;

        for (auto& pattern : patterns) {
            auto pattern_types = pattern->get_var_types();

            for (auto& [var, type] : pattern_types) {
                if (result.count(var) && (result[var] != type || !result[var].is_singleton())) {
                    throw QuerySemanticException(
                        "Variable \"" + get_query_ctx().get_var_name(var) + "\" has more than one type."
                    );
                }
                result[var] = type;
            }
        }

        return result;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpGraphPatternList(\n";
        for (auto& pattern : patterns) {
            pattern->print_to_ostream(os, indent + 2);
        }
        os << std::string(indent, ' ') << ")\n";
        return os;
    }
};
} // namespace GQL
