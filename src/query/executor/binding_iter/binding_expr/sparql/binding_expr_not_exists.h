#pragma once

#include <memory>

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprNotExists : public BindingExpr {
public:
    std::unique_ptr<BindingIter> op_iter;

private:
    std::set<VarId> op_vars;
    std::unique_ptr<Binding> previous_lhs;
    bool previous_had_a_match;

public:
    BindingExprNotExists(std::unique_ptr<BindingIter> op_iter, std::set<VarId>&& op_vars) :
        op_iter(std::move(op_iter)),
        op_vars(std::move(op_vars))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto subset_binding = std::make_unique<Binding>(binding.size);
        for (auto& var : op_vars) {
            subset_binding->add(var, binding[var]);
        }

        if (has_a_match(*subset_binding)) {
            return Conversions::pack_bool(false);
        } else {
            return Conversions::pack_bool(true);
        }
    }

private:
    bool has_a_match(const Binding& lhs_binding)
    {
        if (previous_lhs == nullptr) {
            previous_lhs = std::make_unique<Binding>(lhs_binding.size);
        } else if (*previous_lhs == lhs_binding) {
            return previous_had_a_match;
        }
        previous_lhs->add_all(lhs_binding);

        auto rhs_binding = std::make_unique<Binding>(lhs_binding.size);
        rhs_binding->add_all(lhs_binding);
        op_iter->begin(*rhs_binding);

        while (op_iter->next()) {
            auto match = true;
            for (auto& var : op_vars) {
                auto lhs_id = lhs_binding[var];
                auto rhs_id = (*rhs_binding)[var];
                if (!rhs_id.is_null() && !lhs_id.is_null() && lhs_id != rhs_id) {
                    match = false;
                    break;
                }
            }
            if (match) {
                previous_had_a_match = true;
                return true;
            }
        }
        previous_had_a_match = false;
        return false;
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "NOT EXISTS(_Op_" << ops.size() << "_)";
        ops.push_back(op_iter.get());
    }
};
} // namespace SPARQL
