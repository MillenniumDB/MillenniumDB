#pragma once

#include <set>
#include <vector>

#include "query/exceptions.h"

#include "query/parser/op/gql/op.h"

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

    std::map<VarId, VarType> get_var_types() const override
    {
        std::map<VarId, VarType> result;

        for (auto& pattern : patterns) {
            for (auto& [var, type] : pattern->get_var_types()) {
                if (result.count(var) && result[var].type != type.type) {
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
        os << "OpBasicGraphPattern()\n";

        for (auto& pattern : patterns) {
            pattern->print_to_ostream(os, indent + 2);
        }

        return os;
    }
};
} // namespace GQL
