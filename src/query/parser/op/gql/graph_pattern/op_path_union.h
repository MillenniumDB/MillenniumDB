#pragma once

#include <algorithm>

#include "query/exceptions.h"
#include "query/parser/op/gql/op.h"

namespace GQL {
class OpPathUnion : public Op {
public:
    std::vector<std::unique_ptr<Op>> op_list;

    OpPathUnion(std::vector<std::unique_ptr<Op>>&& op_list) :
        op_list(std::move(op_list))
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Op>> op_list_clone;
        op_list_clone.reserve(op_list.size());
        for (auto& child_expr : op_list) {
            op_list_clone.push_back(child_expr->clone());
        }
        return std::make_unique<OpPathUnion>(std::move(op_list_clone));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;
        for (auto& op : op_list) {
            for (auto& var : op->get_all_vars()) {
                res.insert(var);
            }
        }
        return res;
    }

    std::map<VarId, VarType> get_var_types() const override
    {
        std::map<VarId, VarType> res;
        std::map<VarId, size_t> branches;

        for (auto& op : op_list) {
            for (auto& [var, type] : op->get_var_types()) {
                branches[var]++;
                auto it = res.find(var);
                if (it == res.end()) {
                    res[var] = type;
                    continue;
                }
                if (it->second.type != type.type) {
                    throw QuerySemanticException(
                        "Variable \"" + get_query_ctx().get_var_name(var) + "\" has more than one type."
                    );
                }
                // whatever the order of the branches: group over conditional over unconditional
                it->second.degree = std::max(it->second.degree, type.degree);
            }
        }
        // a variable that some branch does not bind is a conditional singleton
        for (auto& [var, count] : branches) {
            if (count < op_list.size() && res[var].is_unconditional()) {
                res[var].degree = VarType::Conditional;
            }
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpPathUnion(\n";

        for (auto& op : op_list) {
            op->print_to_ostream(os, indent + 2);
        }

        os << std::string(indent, ' ') << ")\n";
        return os;
    }
};
} // namespace GQL
