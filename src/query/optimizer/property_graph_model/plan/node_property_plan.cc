#include "node_property_plan.h"

#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/index_scan.h"

NodePropertyPlan::NodePropertyPlan(Id object, Id key, Id value) :
    object(object),
    key(key),
    value(value),
    object_assigned(object.is_OID()),
    key_assigned(key.is_OID()),
    value_assigned(value.is_OID())
{ }

double NodePropertyPlan::estimate_cost() const
{
    return estimate_output_size();
}

double NodePropertyPlan::estimate_output_size() const
{
    if (gql_model.catalog.node_key2total_count.count(key.get_OID().get_value())) {
        return gql_model.catalog.node_key2total_count[key.get_OID().get_value()];
    }
    return 0;
}

std::set<VarId> NodePropertyPlan::get_vars() const
{
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

void NodePropertyPlan::set_input_vars(const std::set<VarId>& input_vars)
{
    set_input_var(input_vars, object, &object_assigned);
    set_input_var(input_vars, key, &key_assigned);
    set_input_var(input_vars, value, &value_assigned);
}

std::unique_ptr<BindingIter> NodePropertyPlan::get_binding_iter() const
{
    std::array<std::unique_ptr<ScanRange>, 3> ranges;

    if (object_assigned) {
        ranges[0] = ScanRange::get(object, object_assigned);
        ranges[1] = ScanRange::get(key, key_assigned);
        ranges[2] = ScanRange::get(value, value_assigned);
        return std::make_unique<IndexScan<3>>(*gql_model.node_key_value, std::move(ranges));
    } else {
        ranges[0] = ScanRange::get(key, key_assigned);
        ranges[1] = ScanRange::get(value, value_assigned);
        ranges[2] = ScanRange::get(object, object_assigned);
        return std::make_unique<IndexScan<3>>(*gql_model.key_value_node, std::move(ranges));
    }
}

bool NodePropertyPlan::get_leapfrog_iter(
    std::vector<std::unique_ptr<LeapfrogIter>>& /* leapfrog_iters */,
    std::vector<VarId>& /* var_order */,
    uint_fast32_t& /* enumeration_level */
) const
{
    return false;
}

void NodePropertyPlan::print(std::ostream& os, int indent) const
{
    os << std::string(indent, ' ');
    os << "NodeProperty(";
    os << "object: " << object;
    os << ", key: " << key;
    os << ", value: " << value;
    os << ")";
}
