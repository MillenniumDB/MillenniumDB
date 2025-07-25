#include "no_free_variable_minus.h"

void NoFreeVariableMinus::_begin(Binding& parent_binding)
{
    rhs->begin(parent_binding);
    lhs->begin(parent_binding);
    has_result = rhs->next();
}

bool NoFreeVariableMinus::_next()
{
    if (!has_result) {
        auto result = lhs->next();
        if (result) {
        }
        return result;
    }
    return false;
}

void NoFreeVariableMinus::_reset()
{
    rhs->reset();
    if (rhs->next()) {
        has_result = true;
    } else {
        has_result = false;
        lhs->reset();
    }
}

void NoFreeVariableMinus::assign_nulls()
{
    lhs->assign_nulls();
    // Rhs is not in the current scope, so we don't assign nulls here.
}

void NoFreeVariableMinus::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "NoFreeVariableMinus()\n";

    lhs->print(os, indent + 2, stats);
    rhs->print(os, indent + 2, stats);
}
