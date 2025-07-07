#include "minus.h"

void Minus::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;

    rhs_binding = std::make_unique<Binding>(parent_binding->size);

    for (const auto& var : parent_safe_vars) {
        rhs_binding->add(var, (*parent_binding)[var]);
    }

    lhs->begin(_parent_binding);
    rhs->begin(*rhs_binding);
}

bool Minus::_next()
{
    while (lhs->next()) {
        for (const auto& var : safe_join_vars) {
            rhs_binding->add(var, (*parent_binding)[var]);
        }
        rhs->reset();

        auto found_match = false;
        while (rhs->next()) {
            auto bindings_match = true;
            auto variables_in_common = safe_join_vars.size() > 0;

            for (auto& var : unsafe_join_vars) {
                auto lhs_oid = (*parent_binding)[var];
                if (lhs_oid.is_null()) {
                    continue;
                }

                auto rhs_oid = (*rhs_binding)[var];
                if (rhs_oid.is_null()) {
                    continue;
                }

                variables_in_common = true;

                if (lhs_oid != rhs_oid) {
                    bindings_match = false;
                    break;
                }
            }

            if (bindings_match && variables_in_common) {
                found_match = true;
                break;
            }
        }

        if (!found_match) {
            return true;
        }
    }
    return false;
}

void Minus::_reset()
{
    lhs->reset();
}

void Minus::assign_nulls()
{
    lhs->assign_nulls();
    rhs->assign_nulls();
}

void Minus::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }

    os << std::string(indent, ' ') << "Minus(";

    os << "safe_join: [";
    for (auto& var : safe_join_vars) {
        os << ' ' << var;
    }

    os << " ] unsafe_join: [";
    for (auto& var : unsafe_join_vars) {
        os << ' ' << var;
    }

    os << " ] parent_safe: [";
    for (auto& var : parent_safe_vars) {
        os << ' ' << var;
    }
    os << " ]\n";

    lhs->print(os, indent + 2, stats);
    rhs->print(os, indent + 2, stats);
}
