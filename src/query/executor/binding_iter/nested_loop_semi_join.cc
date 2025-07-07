#include "nested_loop_semi_join.h"

void NestedLoopSemiJoin::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;

    rhs_binding = std::make_unique<Binding>(parent_binding->size);

    lhs->begin(_parent_binding);
    rhs->begin(*rhs_binding);
}

bool NestedLoopSemiJoin::_next()
{
    while (lhs->next()) {
        rhs_binding->add_all(*parent_binding);
        rhs->reset();
        while (rhs->next()) {
            bool match = true;

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
                    match = false;
                    break;
                }
            }

            if (match) {
                return true;
            }
        }
    }
    return false;
}

void NestedLoopSemiJoin::_reset()
{
    lhs->reset();
}

void NestedLoopSemiJoin::assign_nulls()
{
    lhs->assign_nulls();
    rhs->assign_nulls();
}

void NestedLoopSemiJoin::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "NestedLoopSemiJoin(";

    os << "unsafe_join: [";
    for (auto& var : unsafe_join_vars) {
        os << ' ' << var;
    }

    os << " ])\n";

    lhs->print(os, indent + 2, stats);
    rhs->print(os, indent + 2, stats);
}
