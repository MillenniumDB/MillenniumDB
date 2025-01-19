#include "property_type_plan.h"

#include <cassert>

#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/index_scan.h"
#include "query/executor/binding_iter/scan_ranges/assigned_var.h"
#include "query/executor/binding_iter/scan_ranges/term.h"
#include "query/executor/binding_iter/scan_ranges/unassigned_var.h"
#include "storage/index/leapfrog/leapfrog_bpt_iter.h"

using namespace std;

PropertyTypePlan::PropertyTypePlan(VarId object, ObjectId key, VarId value, uint64_t types_bitmap) :
    object(object),
    key(key),
    value(value),
    types_bitmap(types_bitmap),
    object_assigned(false)
{ }

void PropertyTypePlan::print(std::ostream& os, int indent) const
{
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "Property(";
    os << "object: " << object;
    os << ", key: " << key;
    os << ",types: " << printTypesFromBitmap(types_bitmap);
    os << ")";

    os << ",\n";
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "  ↳ Estimated factor: " << estimate_output_size();
}

std::string PropertyTypePlan::printTypesFromBitmap(uint64_t types_bitmap) const
{
    bool first = false;
    std::string types;

    if (types_bitmap & static_cast<uint8_t>(propertyType::TYPE_NULL)) {
        types += "NULL";
        first = false;
    }
    if (types_bitmap & static_cast<uint8_t>(propertyType::TYPE_BOOL)) {
        if (!first)
            types += ", ";
        types += "BOOL";
        first = false;
    }
    if (types_bitmap & static_cast<uint8_t>(propertyType::TYPE_INTEGER)) {
        if (!first)
            types += ", ";
        types += "INTEGER";
        first = false;
    }
    if (types_bitmap & static_cast<uint8_t>(propertyType::TYPE_FLOAT)) {
        if (!first)
            types += ", ";
        types += "FLOAT";
        first = false;
    }
    if (types_bitmap & static_cast<uint8_t>(propertyType::TYPE_STRING)) {
        if (!first)
            types += ", ";
        types += "STRING";
        first = false;
    }

    return types;
}

double PropertyTypePlan::estimate_cost() const
{
    return /*100.0 +*/ estimate_output_size();
}


double PropertyTypePlan::estimate_output_size() const
{
    const auto total_objects = static_cast<double>(
        quad_model.catalog.identifiable_nodes_count + quad_model.catalog.edge_count
    );

    const auto total_properties = static_cast<double>(quad_model.catalog.properties_count);

    if (total_objects == 0) { // To avoid division by 0
        return 0;
    }
    if (object_assigned) {
        return total_properties / total_objects; // key and value not assigned
    } else {
        return total_properties; // nothing assigned
    }
}


void PropertyTypePlan::set_input_vars(const std::set<VarId>& input_vars)
{
    set_input_var(input_vars, object, &object_assigned);
}


std::set<VarId> PropertyTypePlan::get_vars() const
{
    std::set<VarId> result;
    if (!object_assigned) {
        result.insert(object);
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
unique_ptr<BindingIter> PropertyTypePlan::get_binding_iter() const
{
    array<unique_ptr<ScanRange>, 3> ranges;

    if (object_assigned) {
        ranges[0] = std::make_unique<AssignedVar>(object);
        ;
        ranges[1] = std::make_unique<Term>(key);
        ranges[2] = std::make_unique<RangeType>(value, types_bitmap);
        return make_unique<IndexScan<3>>(*quad_model.object_key_value, std::move(ranges));
    } else {
        ranges[0] = std::make_unique<Term>(key);
        ranges[1] = std::make_unique<RangeType>(value, types_bitmap);
        ranges[2] = std::make_unique<UnassignedVar>(object);
        return make_unique<IndexScan<3>>(*quad_model.key_value_object, std::move(ranges));
    }
}


bool PropertyTypePlan::get_leapfrog_iter(
    std::vector<std::unique_ptr<LeapfrogIter>>& /*leapfrog_iters*/,
    vector<VarId>& /*var_order*/,
    uint_fast32_t& /*enumeration_level*/
) const
{
    return false;
}
