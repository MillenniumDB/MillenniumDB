#include "sequence.h"

void Sequence::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    if (!iters.empty()) {
        iters[0]->begin(_parent_binding);
    }
}

bool Sequence::_next()
{
    while (true) {
        set_start_variable();

        if (iters[current_iter]->next()) {
            if (current_iter + 1 < iters.size()) {
                store_boundary_variable();
                current_iter++;
                iters[current_iter]->begin(*parent_binding);
            } else {
                return true;
            }
        } else {
            if (current_iter > 0) {
                end_vars_stack.pop();
                iters[current_iter]->reset();
                current_iter--;
            } else {
                return false;
            }
        }
    }
}

void Sequence::set_start_variable()
{
    if (!end_vars_stack.empty()) {
        bool found;
        VarId end_var = get_query_ctx().get_var(".end", &found);

        ObjectId end_value = end_vars_stack.top();
        parent_binding->add(end_var, end_value);
    }
}

void Sequence::store_boundary_variable()
{
    bool found;
    VarId end_var = get_query_ctx().get_var(".end", &found);

    auto end_value = (*parent_binding)[end_var];
    end_vars_stack.push(end_value);
}

void Sequence::_reset()
{
    current_iter = 0;
    for (auto& iter : iters) {
        iter->reset();
    }
    end_vars_stack = std::stack<ObjectId>();
}

void Sequence::assign_nulls()
{
    for (auto& iter : iters) {
        iter->assign_nulls();
    }
}

void Sequence::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "Sequence()\n";
    for (auto& iter : iters) {
        iter->print(os, indent + 2, stats);
    }
}
