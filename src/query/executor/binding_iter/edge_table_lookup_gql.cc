#include "edge_table_lookup_gql.h"

#include <cassert>

#include "macros/likely.h"
#include "query/exceptions.h"

void EdgeTableLookupGQL::accept_visitor(BindingIterVisitor& visitor)
{
    visitor.visit(*this);
}

void EdgeTableLookupGQL::_begin(Binding& parent_binding)
{
    this->parent_binding = &parent_binding;
    already_looked = false;
}

bool EdgeTableLookupGQL::_next()
{
    if (already_looked) {
        return false;
    } else {
        if (MDB_unlikely(get_query_ctx().thread_info.interruption_requested)) {
            throw InterruptedException();
        }
        already_looked = true;
        ++lookups;

        ObjectId edge_assignation = (*parent_binding)[edge];

        if ((ObjectId::TYPE_MASK & edge_assignation.id) != expected_mask) {
            return false;
        }
        auto edge_id = ObjectId::VALUE_MASK & edge_assignation.id;

        auto record = table[edge_id];

        if (record == nullptr)
            return false;

        auto check_id = [](Binding& binding, Id id, bool assigned, ObjectId obj_id) -> bool {
            if (id.is_var()) {
                if (assigned) {
                    return binding[id.get_var()] == obj_id;
                } else {
                    binding.add(id.get_var(), obj_id);
                }
            } else {
                auto value = id.get_OID();
                if (!value.is_null() && value != obj_id) {
                    return false;
                }
            }
            return true;
        };

        // check if assigned variables (not null) have the same value
        if (check_id(*parent_binding, from, from_assigned, ObjectId((*record)[0]))
            && check_id(*parent_binding, to, to_assigned, ObjectId((*record)[1])))
        {
            return true;
        } else {
            return false;
        }
    }
}

void EdgeTableLookupGQL::_reset()
{
    already_looked = false;
}

void EdgeTableLookupGQL::assign_nulls()
{
    // Not assigning edge to null, since the edge is supposed to be assigned
    if (!from_assigned) {
        assert(from.is_var());
        parent_binding->add(from.get_var(), ObjectId::get_null());
    }
    if (!to_assigned) {
        assert(to.is_var());
        parent_binding->add(to.get_var(), ObjectId::get_null());
    }
}
