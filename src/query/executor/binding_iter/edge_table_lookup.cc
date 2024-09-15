#include "edge_table_lookup.h"

#include <cassert>

#include "macros/likely.h"
#include "query/exceptions.h"

void EdgeTableLookup::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


void EdgeTableLookup::_begin(Binding& parent_binding) {
    this->parent_binding = &parent_binding;
    already_looked = false;
}


bool EdgeTableLookup::_next() {
    if (already_looked) {
        return false;
    } else {
        if (MDB_unlikely(get_query_ctx().thread_info.interruption_requested)) {
            throw InterruptedException();
        }
        already_looked = true;
        ++lookups;

        ObjectId edge_assignation;
        if (edge.is_var()) {
            edge_assignation = (*parent_binding)[edge.get_var()];
        } else {
            edge_assignation = edge.get_OID();
        }
        if ( (ObjectId::TYPE_MASK & edge_assignation.id) != ObjectId::MASK_EDGE) {
            return false;
        }
        auto edge_id = ObjectId::VALUE_MASK & edge_assignation.id;
        assert(edge_id > 0);

        auto record = table[edge_id - 1]; // first edge has the id 1, and its inserted at pos 0 in the table

        if (record == nullptr) return false;

        auto check_id = [] (Binding& binding, Id id, bool assigned, ObjectId obj_id) -> bool {
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
        if (   check_id(*parent_binding, from, from_assigned, ObjectId((*record)[0]))
            && check_id(*parent_binding, to,   to_assigned,   ObjectId((*record)[1]))
            && check_id(*parent_binding, type, type_assigned, ObjectId((*record)[2])))
        {
            return true;
        } else {
            return false;
        }
    }
}


void EdgeTableLookup::_reset() {
    already_looked = false;
}


void EdgeTableLookup::assign_nulls() {
    if (edge.is_var()) {
        parent_binding->add(edge.get_var(), ObjectId::get_null());
    }
    if (!from_assigned) {
        assert(from.is_var());
        parent_binding->add(from.get_var(), ObjectId::get_null());
    }
    if (!to_assigned) {
        assert(to.is_var());
        parent_binding->add(to.get_var(), ObjectId::get_null());
    }
    if (!type_assigned) {
        assert(type.is_var());
        parent_binding->add(type.get_var(), ObjectId::get_null());
    }
}
