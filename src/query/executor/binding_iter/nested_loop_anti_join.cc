#include "nested_loop_anti_join.h"

#include <algorithm>

#include "query/executor/binding_iter/empty_binding_iter.h"
#include "query/var_id.h"

template<bool B>
void NestedLoopAntiJoin<B>::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;

    lhs_binding = std::make_unique<Binding>(parent_binding->size);
    rhs_binding = std::make_unique<Binding>(parent_binding->size);

    // copy parent_safe_vars from parent_binding into lhs_binding and rhs_binding
    for (const auto& var : parent_safe_vars) {
        lhs_binding->add(var, (*parent_binding)[var]);
        rhs_binding->add(var, (*parent_binding)[var]);
    }

    lhs->begin(*lhs_binding);
    rhs->begin(*rhs_binding);
}


template<bool match_without_common_variables>
bool NestedLoopAntiJoin<match_without_common_variables>::_next() {
    while (lhs->next()) {
        // assign values from left binding into the right binding
        for (const auto& var : safe_join_vars) {
            rhs_binding->add(var, (*lhs_binding)[var]);
        }

        rhs->reset();
        auto found_match = false;
        while (rhs->next()) {
            auto bindings_match = true;
            auto variables_in_common = safe_join_vars.size() > 0;
            for (auto& var : unsafe_join_vars) {
                auto lhs_oid = (*lhs_binding)[var];
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

            if (match_without_common_variables) {
                if (bindings_match) {
                    found_match = true;
                    break;
                }
            } else {
                if (bindings_match && variables_in_common) {
                    found_match = true;
                    break;
                }
            }
        }

        if (!found_match) {
            for (auto& var: lhs_only_vars) {
                parent_binding->add(var, (*lhs_binding)[var]);
            }

            for (auto& var: unsafe_join_vars) {
                parent_binding->add(var, (*lhs_binding)[var]);
            }

            for (const auto& var : safe_join_vars) {
                parent_binding->add(var, (*lhs_binding)[var]);
            }
            return true;
        }
    }
    return false;
}


template<bool B>
void NestedLoopAntiJoin<B>::_reset() {
    // copy parent_safe_vars from parent_binding into lhs_binding and rhs_binding
    for (const auto& var : parent_safe_vars) {
        lhs_binding->add(var, (*parent_binding)[var]);
        rhs_binding->add(var, (*parent_binding)[var]);
    }
    lhs->reset();
    rhs->reset();
}


template<bool B>
void NestedLoopAntiJoin<B>::assign_nulls() {
    lhs->assign_nulls();
    rhs->assign_nulls();

    for (auto& var: lhs_only_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }

    for (auto& var: unsafe_join_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }

    for (const auto& var : safe_join_vars) {
        parent_binding->add(var, ObjectId::get_null());
    }
}


template<bool B>
void NestedLoopAntiJoin<B>::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}

template class NestedLoopAntiJoin<true>;
template class NestedLoopAntiJoin<false>;
