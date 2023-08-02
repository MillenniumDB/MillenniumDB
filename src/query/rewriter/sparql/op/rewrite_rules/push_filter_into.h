#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "query/var_id.h"
#include "query/parser/op/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "rewrite_rule.h"

namespace SPARQL {
/**
 * According to: (Schmidt, 2010) the following rules hold if
 * vars(R) subseteq safeVars(A1):
 * Filter_R(A1 JOIN A2)  = Filter_R(A1) JOIN A2
 * TODO: Case A1 JOIN Filter_R(A2)
 * Filter_R(A1 OPT A2)   = Filter_R(A1) OPT A2
 * Filter_R(A1 MINUS A2) = Filter_R(A1) MINUS A2
 * This class implements these rewrite rules using a template,
 * the classes that are supported must have lhs and rhs.
 */
template <typename T>
class PushFilterInto : public RewriteRule {
public:
    bool is_possible_to_regroup(std::unique_ptr<Op>& unknown_op) override {
        if (!is_castable_to<OpFilter>(unknown_op))
            return false;
        auto op_filter = dynamic_cast<OpFilter*>(unknown_op.get());
        if (is_castable_to<T>(op_filter->op)) {
            auto op_t = dynamic_cast<T*>(op_filter->op.get());
            std::set<VarId> filter_vars;
            for (auto &filter : op_filter->filters) {
                auto child_filter_vars = filter->get_all_vars();
                filter_vars.insert(child_filter_vars.begin(),
                                   child_filter_vars.end());
            }
            return is_subseteq(filter_vars,
                               op_t->lhs->get_safe_vars());
        }
        return false;
    }

    std::unique_ptr<Op> regroup(std::unique_ptr<Op> unknown_op) override {
        auto op_filter = cast_to<OpFilter>(std::move(unknown_op));
        auto op_t      = cast_to<T>(std::move(op_filter->op));

        auto new_filter = std::make_unique<OpFilter>(
            std::move(op_t->lhs),
            std::move(op_filter->filters)
        );
        op_t->lhs = std::move(new_filter);
        return op_t;
    }

private:
    bool is_subseteq(const std::set<VarId>& lhs, const std::set<VarId>& rhs) {
        for (auto& var : lhs) {
            if (rhs.find(var) == rhs.end())
                return false;
        }
        return true;
    }
};
} // namespace SPARQL
