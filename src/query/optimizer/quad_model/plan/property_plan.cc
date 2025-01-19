#include "property_plan.h"

#include <cassert>

#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/index_scan.h"
#include "storage/index/leapfrog/leapfrog_bpt_iter.h"

using namespace std;

PropertyPlan::PropertyPlan(Id object, Id key, Id value) :
    object          (object),
    key             (key),
    value           (value),
    object_assigned (object.is_OID()),
    key_assigned    (key.is_OID()),
    value_assigned  (value.is_OID()) { }


void PropertyPlan::print(std::ostream& os, int indent) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "Property(";
    os << "object: " << object;
    os << ", key: " << key;
    os << ", value: " << value;
    os << ")";

    os << ",\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "  ↳ Estimated factor: " << estimate_output_size();
}


double PropertyPlan::estimate_cost() const {
    return /*100.0 +*/ estimate_output_size();
}


double PropertyPlan::estimate_output_size() const {
    const auto total_objects    = static_cast<double>(quad_model.catalog.identifiable_nodes_count
                                                      + quad_model.catalog.edge_count);

    const auto total_properties = static_cast<double>(quad_model.catalog.properties_count);

    assert((key_assigned || !value_assigned) && "fixed values with open key is not supported");

    if (total_objects == 0) { // To avoid division by 0
        return 0;
    }

    if (key_assigned) {
        double total_values = 0;
        double key_count = 0;
        if (key.is_OID()) {
            auto it = quad_model.catalog.key2total_count.find(key.get_OID().id);
            if (it != quad_model.catalog.key2total_count.end()) {
                total_values = it->second;
            }

            it = quad_model.catalog.key2total_count.find(key.get_OID().id);
            if (it != quad_model.catalog.key2total_count.end()) {
                key_count = it->second;
            }
        } else {
            // TODO: this case (key is an assigned variable) is not possible yet, but we may need to cover it in the future
            return 0;
        }

        if (total_values == 0) { // To avoid division by 0
            return 0;
        }
        if (value_assigned) {
            if (object_assigned) {
                return key_count / (total_values * total_objects);
            } else {
                return key_count / total_values;
            }
        } else {
            if (object_assigned) {
                return key_count / total_objects;
            } else {
                return key_count;
            }
        }
    } else {
        if (object_assigned) {
            return total_properties / total_objects; // key and value not assigned
        } else {
            return total_properties; // nothing assigned
        }
    }
}


void PropertyPlan::set_input_vars(const std::set<VarId>& input_vars) {
    set_input_var(input_vars, object, &object_assigned);
    set_input_var(input_vars, key,    &key_assigned);
    set_input_var(input_vars, value,  &value_assigned);
}


std::set<VarId> PropertyPlan::get_vars() const {
    std::set<VarId> result;
    if (object.is_var() && !object_assigned) {
        result.insert(object.get_var());
    }
    if (key.is_var() && !key_assigned) {
        result.insert(key.get_var());
    }
    if (value.is_var() && !value_assigned) {
        result.insert(value.get_var());
    }

    return result;
}


/**
 * ╔═╦══════════════╦═══════════════╦══════════════════╦═════════╗
 * ║ ║  KeyAssigned ║ ValueAssigned ║  ObjectAssigned  ║  Index  ║
 * ╠═╬══════════════╬═══════════════╬══════════════════╬═════════╣
 * ║1║      yes     ║      yes      ║        yes       ║   OKV   ║
 * ║2║      yes     ║      yes      ║        no        ║   KVO   ║
 * ║3║      yes     ║      no       ║        yes       ║   OKV   ║
 * ║4║      yes     ║      no       ║        no        ║   KVO   ║
 * ║5║      no      ║      yes      ║        yes       ║  ERROR  ║
 * ║6║      no      ║      yes      ║        no        ║  ERROR  ║
 * ║7║      no      ║      no       ║        yes       ║   OKV   ║
 * ║8║      no      ║      no       ║        no        ║   KVO   ║
 * ╚═╩══════════════╩═══════════════╩══════════════════╩═════════╝
 */
unique_ptr<BindingIter> PropertyPlan::get_binding_iter() const {
    array<unique_ptr<ScanRange>, 3> ranges;

    assert((key_assigned || !value_assigned) && "fixed values with open key is not supported");

    if (object_assigned) {
        ranges[0] = ScanRange::get(object, object_assigned);
        ranges[1] = ScanRange::get(key, key_assigned);
        ranges[2] = ScanRange::get(value, value_assigned);
        return make_unique<IndexScan<3>>(*quad_model.object_key_value, std::move(ranges));
    } else {
        ranges[0] = ScanRange::get(key, key_assigned);
        ranges[1] = ScanRange::get(value, value_assigned);
        ranges[2] = ScanRange::get(object, object_assigned);
        return make_unique<IndexScan<3>>(*quad_model.key_value_object, std::move(ranges));
    }
}


bool PropertyPlan::get_leapfrog_iter(std::vector<std::unique_ptr<LeapfrogIter>>& leapfrog_iters,
                                     vector<VarId>&                              var_order,
                                     uint_fast32_t&                              enumeration_level) const
{
    vector<unique_ptr<ScanRange>> initial_ranges;
    vector<VarId> intersection_vars;
    vector<VarId> enumeration_vars;

    // index = INT32_MAX means enumeration, index = -1 means term
    int_fast32_t obj_index, key_index, value_index;

    // Assign obj_index
    if (object.is_OID() || object_assigned) {
        obj_index = -1;
    } else {
        obj_index = INT32_MAX;
    }

    // Assign key_index
    if (key.is_OID() || key_assigned) {
        key_index = -1;
    } else {
        key_index = INT32_MAX;
    }

    // Assign value_index
    if (value.is_OID() || value_assigned) {
        value_index = -1;
    } else {
        value_index = INT32_MAX;
    }

    // search for vars marked as enumeration (INT32_MAX) that are intersection
    // and assign them the correct index
    for (size_t i = 0; i < enumeration_level; i++) {
        if (obj_index == INT32_MAX && object.get_var() == var_order[i]) {
            obj_index = i;
        }
        if (key_index == INT32_MAX && key.get_var() == var_order[i]) {
            key_index = i;
        }
        if (value_index == INT32_MAX && value.get_var() == var_order[i]) {
            value_index = i;
        }
    }

    auto assign = [&initial_ranges, &enumeration_vars, &intersection_vars]
                  (int_fast32_t& index, Id id)
                  -> void
    {
        if (index == -1) {
            initial_ranges.push_back(ScanRange::get(id, true));
        } else if (index == INT32_MAX) {
            enumeration_vars.push_back(id.get_var());
        } else {
            intersection_vars.push_back(id.get_var());
        }
    };

    // object_key_value
    if (obj_index <= key_index && key_index <= value_index) {
        assign(obj_index,   object);
        assign(key_index,   key);
        assign(value_index, value);

        leapfrog_iters.push_back(make_unique<LeapfrogBptIter<3>>(
            &get_query_ctx().thread_info.interruption_requested,
            *quad_model.object_key_value,
            std::move(initial_ranges),
            std::move(intersection_vars),
            std::move(enumeration_vars)
        ));
        return true;
    }
    // key_value_object
    else if (key_index <= value_index && value_index <= obj_index) {
        assign(key_index,   key);
        assign(value_index, value);
        assign(obj_index,   object);

        leapfrog_iters.push_back(make_unique<LeapfrogBptIter<3>>(
            &get_query_ctx().thread_info.interruption_requested,
            *quad_model.key_value_object,
            std::move(initial_ranges),
            std::move(intersection_vars),
            std::move(enumeration_vars)
        ));
        return true;
    } else {
        return false;
    }
}
