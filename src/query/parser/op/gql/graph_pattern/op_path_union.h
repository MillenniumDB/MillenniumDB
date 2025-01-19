#pragma once

#include <memory>
#include <vector>

#include "query/parser/op/op.h"
#include "query/rewriter/gql/op/var_type.h"

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
        std::map<VarId, std::unique_ptr<VarType>> res;

        for (auto& op : op_list) {
            std::map<VarId, std::unique_ptr<VarType>> op_types = op->get_var_types();

            for (auto& [var, type] : op_types) {
                if (res.count(var) && *res[var] != *type) {
                    throw QuerySemanticException(
                        "Variable \"" + get_query_ctx().get_var_name(var)
                        + "\" appears in union and it has more than one type."
                    );
                }

                if (res.count(var) && !type->is_singleton()) {
                    throw QuerySemanticException(
                        "Variable \"" + get_query_ctx().get_var_name(var)
                        + "\" appears in more than one term in an alternation and it is not singleton."
                    );
                }
                res[var] = std::move(type);
            }
        }

        for (auto& [var, type] : res) {
            if (!type->is_conditional()) {
                res[var] = std::make_unique<Maybe>(std::move(type));
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
