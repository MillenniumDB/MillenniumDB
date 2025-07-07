#include "not_exists.h"

void NotExists::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;

    rhs_binding = std::make_unique<Binding>(parent_binding->size);

    lhs->begin(_parent_binding);
    rhs->begin(*rhs_binding);
}

bool NotExists::_next()
{
    while (lhs->next()) {
        rhs_binding->add_all(*parent_binding);
        rhs->reset();
        auto found_match = false;

        while (rhs->next()) {
            auto bindings_match = true;

            for (auto& var : unsafe_join_vars) {
                auto lhs_oid = (*parent_binding)[var];
                if (lhs_oid.is_null()) {
                    continue;
                }

                auto rhs_oid = (*rhs_binding)[var];
                if (rhs_oid.is_null()) {
                    continue;
                }

                if (lhs_oid != rhs_oid) {
                    bindings_match = false;
                    break;
                }
            }

            if (bindings_match) {
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

void NotExists::_reset()
{
    lhs->reset();
}

void NotExists::assign_nulls()
{
    lhs->assign_nulls();
    rhs->assign_nulls();
}

void NotExists::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "NotExists(";

    os << "unsafe_join: [";
    for (auto& var : unsafe_join_vars) {
        os << ' ' << var;
    }

    os << " ])\n";

    lhs->print(os, indent + 2, stats);
    rhs->print(os, indent + 2, stats);
}
