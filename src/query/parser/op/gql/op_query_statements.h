#pragma once

#include "query/parser/op/gql/op.h"

namespace GQL {

class OpQueryStatements : public Op {
public:
    std::vector<std::unique_ptr<Op>> ops;

    OpQueryStatements(std::vector<std::unique_ptr<Op>> ops) :
        ops(std::move(ops))
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Op>> result;

        for (auto& op : ops) {
            result.push_back(op->clone());
        }
        return std::make_unique<OpQueryStatements>(std::move(result));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> result;

        for (auto& op : ops) {
            result.merge(op->get_all_vars());
        }
        return result;
    }

    std::map<VarId, VarType> get_var_types() const override
    {
        std::map<VarId, VarType> result;

        for (auto& op : ops) {
            auto op_types = op->get_var_types();

            for (auto&& [var, type] : op_types) {
                if (result.count(var) && result[var] != type) {
                    throw QuerySemanticException(
                        "Variable \"" + get_query_ctx().get_var_name(var) + "\" has more than one type."
                    );
                }
                result[var] = std::move(type);
            }
        }

        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpQueryStatements()\n";

        for (auto& op : ops) {
            op->print_to_ostream(os, indent + 2);
        }
        os << "\n";
        return os;
    }
};
} // namespace GQL
