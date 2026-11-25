#include "union.h"

void Union::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    iters[0]->begin(_parent_binding);
    begun.assign(iters.size(), false);
    begun[0] = true;
}

bool Union::_next()
{
    while (current_iter < iters.size()) {
        if (iters[current_iter]->next()) {
            return true;
        } else {
            iters[current_iter]->assign_nulls();
            ++current_iter;
            if (current_iter < iters.size()) {
                // an iter is begun once; then a reset starts it again
                if (!begun[current_iter]) {
                    iters[current_iter]->begin(*parent_binding);
                    begun[current_iter] = true;
                } else {
                    iters[current_iter]->reset();
                }
            }
        }
    }
    return false;
}

void Union::_reset()
{
    if (current_iter < iters.size()) {
        iters[current_iter]->assign_nulls();
    }
    current_iter = 0;
    // later iters are reset when the union reaches them: a reset may bind variables
    iters[0]->reset();
}

void Union::assign_nulls()
{
    if (current_iter < iters.size()) {
        iters[current_iter]->assign_nulls();
    }
}

void Union::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "Union()\n";
    for (const auto& iter : iters) {
        iter->print(os, indent + 2, stats);
    }
}
