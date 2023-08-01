#include "edge_table_lookup.h"

#include <cassert>

#include "query/exceptions.h"

void EdgeTableLookup::analyze(std::ostream& os, int indent) const {
    os << std::string(indent, ' ');
    os << "EdgeTableLookup(lookups: " << lookups << ", found: " << results << ")";
}


void EdgeTableLookup::begin(Binding& parent_binding) {
    this->parent_binding = &parent_binding;
    already_looked = false;
}


bool EdgeTableLookup::next() {
    if (already_looked) {
        return false;
    } else {
        if (__builtin_expect(!!(get_query_ctx().thread_info.interruption_requested), 0)) {
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

        auto check_id = [] (Binding& binding, Id id, ObjectId obj_id) -> bool {
            if (id.is_var()) {
                auto binding_value = binding[id.get_var()];
                if (!binding_value.is_null() && binding_value != obj_id) {
                    return false;
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
        if (   check_id(*parent_binding, from, ObjectId((*record)[0]))
            && check_id(*parent_binding, to,   ObjectId((*record)[1]))
            && check_id(*parent_binding, type, ObjectId((*record)[2])))
        {
            ++results;
            return true;
        } else {
            return false;
        }
    }
}


void EdgeTableLookup::reset() {
    already_looked = false;
}


void EdgeTableLookup::assign_nulls() {
    if (edge.is_var()) {
        parent_binding->add(edge.get_var(), ObjectId::get_null());
    }
    if (from.is_var()) {
        parent_binding->add(from.get_var(), ObjectId::get_null());
    }
    if (to.is_var()) {
        parent_binding->add(to.get_var(), ObjectId::get_null());
    }
    if (type.is_var()) {
        parent_binding->add(type.get_var(), ObjectId::get_null());
    }
}
