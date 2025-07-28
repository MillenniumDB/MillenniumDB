#include "expr_evaluator.h"

#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

void ExprEvaluator::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    child_iter->begin(*parent_binding);
}

void ExprEvaluator::_reset()
{
    this->child_iter->reset();
}

bool ExprEvaluator::_next()
{
    // we can have more than one ExprEvaluator per query
    // but executing this more than once has no effect
    // This line is useful only if a BNODE expr is present
    get_query_ctx().blank_node_ids.clear();

    if (child_iter->next()) {
        for (auto& [var_id, expr] : exprs) {
            auto res = expr->eval(*parent_binding);
            parent_binding->add(var_id, res);
        }
        return true;
    }
    return false;
}

void ExprEvaluator::assign_nulls()
{
    for (auto& [var_id, expr] : exprs) {
        parent_binding->add(var_id, ObjectId::get_null());
    }
    this->child_iter->assign_nulls();
}

void ExprEvaluator::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "ExprEvaluator(exprs: ";

    {
        BindingExprPrinter printer(os, indent, stats);
        auto first = true;
        for (auto& [var, expr] : exprs) {
            if (first)
                first = false;
            else
                os << ", ";

            os << var;
            if (expr) {
                os << '=';
                printer.print(*expr);
            }
        }

        os << ")\n";
    }

    child_iter->print(os, indent + 2, stats);
}
