#include "projection_order_exprs.h"

#include "query/executor/binding_iter/binding_expr/binding_expr.h"

void ProjectionOrderExprs::begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    child_id_iter->begin(_parent_binding);
}


void ProjectionOrderExprs::reset() {
    child_id_iter.reset();
    count = 0;
}


bool ProjectionOrderExprs::next() {
    get_query_ctx().blank_node_ids.clear();

    if (child_id_iter->next()) {
        count++;
        // TODO: maybe have an expression vector without nullptrs
        // so that we can iterate directly over expressions, as the
        // current solution is inefficient if we have only or mostly
        // normal variables.
        // If we have no expressions, then this entire Op is unnecessary.
        for (auto & [var_id, expr, is_agg] : projection_order_exprs) {
            if(expr) {
                auto oid = expr->eval(*parent_binding);
                parent_binding->add(var_id, oid);
            }
        }
        return true;
    } else {
        return false;
    }
}


void ProjectionOrderExprs::assign_nulls() {
    child_id_iter->assign_nulls();
}


void ProjectionOrderExprs::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "ProjectionOrderExprs(results: ";
    os << count;

    for (size_t i = 0; i < projection_order_exprs.size(); i++) {
        const auto& [var, expr, is_agg] = projection_order_exprs[i];
        os << " ,?" << get_query_ctx().get_var_name(var);
        if (expr) {
            os << '=' << *expr;
        }
    }
    os << ")\n";
    child_id_iter->analyze(os, indent+2);
}
