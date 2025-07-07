#include "nested_loop_left_join.h"

using namespace std;

void NestedLoopLeftJoin::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    lhs_binding = std::make_unique<Binding>(_parent_binding.size);
    rhs_binding = std::make_unique<Binding>(_parent_binding.size);

    // copy assigned_safe_vars from parent_binding into lhs_binding and rhs_binding
    for (const auto& var : parent_safe_vars) {
        lhs_binding->add(var, (*parent_binding)[var]);
        rhs_binding->add(var, (*parent_binding)[var]);
    }

    lhs->begin(*lhs_binding);
    if (lhs->next()) {
        for (const auto& var : safe_join_vars) {
            rhs_binding->add(var, (*lhs_binding)[var]);
        }

        must_return_null = true;
        rhs = original_rhs.get();
    } else {
        must_return_null = false;
        rhs = &empty_iter;
    }
    original_rhs->begin(*rhs_binding);
}

bool NestedLoopLeftJoin::_next()
{
    while (true) {
        if (rhs->next()) {
            auto bindings_match = true;
            for (auto& var : unsafe_join_vars) {
                auto lhs_oid = (*lhs_binding)[var];
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
                for (auto& var : lhs_only_vars) {
                    parent_binding->add(var, (*lhs_binding)[var]);
                }

                for (auto& var : rhs_only_vars) {
                    parent_binding->add(var, (*rhs_binding)[var]);
                }

                for (auto& var : safe_join_vars) {
                    parent_binding->add(var, (*rhs_binding)[var]);
                }

                for (auto& var : unsafe_join_vars) {
                    auto var_oid = (*lhs_binding)[var];
                    if (var_oid.is_null()) {
                        var_oid = (*rhs_binding)[var];
                    }
                    parent_binding->add(var, var_oid);
                }
                must_return_null = false;
                return true;
            }
        } else {
            if (must_return_null) {
                rhs->assign_nulls();

                for (auto& var : lhs_only_vars) {
                    parent_binding->add(var, (*lhs_binding)[var]);
                }

                for (auto& var : rhs_only_vars) {
                    parent_binding->add(var, (*rhs_binding)[var]);
                }

                for (auto& var : safe_join_vars) {
                    parent_binding->add(var, (*lhs_binding)[var]);
                }

                // for (auto& var: unsafe_join_vars) {
                //     auto var_oid = (*lhs_binding)[var];
                //     if (var_oid.is_null()) {
                //         var_oid = (*rhs_binding)[var];
                //     }
                //     parent_binding->add(var, var_oid);
                // }
                for (auto& var : unsafe_join_vars) {
                    parent_binding->add(var, (*lhs_binding)[var]);
                }

                must_return_null = false;
                return true;
            } else {
                if (lhs->next()) {
                    must_return_null = true;
                    for (const auto& var : safe_join_vars) {
                        rhs_binding->add(var, (*lhs_binding)[var]);
                    }
                    rhs->reset();
                } else {
                    must_return_null = false;
                    return false;
                }
            }
        }
    }
}

void NestedLoopLeftJoin::_reset()
{
    // copy assigned_safe_vars from parent_binding into lhs_binding and rhs_binding
    for (const auto& var : parent_safe_vars) {
        lhs_binding->add(var, (*parent_binding)[var]);
        rhs_binding->add(var, (*parent_binding)[var]);
    }

    lhs->reset();
    if (lhs->next()) {
        must_return_null = true;
        rhs = original_rhs.get();
        for (const auto& var : safe_join_vars) {
            rhs_binding->add(var, (*lhs_binding)[var]);
        }
        rhs->reset();
    } else {
        must_return_null = false;
        rhs = &empty_iter;
    }
}

void NestedLoopLeftJoin::assign_nulls()
{
    lhs->assign_nulls();
    original_rhs->assign_nulls();

    for (auto& var : lhs_only_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }

    for (auto& var : rhs_only_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }

    for (auto& var : unsafe_join_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }

    for (const auto& var : safe_join_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
}

void NestedLoopLeftJoin::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "NestedLoopLeftJoin(";

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

    os << " ] lhs_only: [";
    for (auto& var : lhs_only_vars) {
        os << ' ' << var;
    }

    os << " ] rhs_only: [";
    for (auto& var : rhs_only_vars) {
        os << ' ' << var;
    }
    os << " ])\n";

    lhs->print(os, indent + 2, stats);
    original_rhs->print(os, indent + 2, stats);
}
