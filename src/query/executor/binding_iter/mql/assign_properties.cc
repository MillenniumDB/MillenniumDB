#include "assign_properties.h"

#include <cassert>

#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/scan_ranges/scan_range.h"

using namespace MQL;

AssignProperties::AssignProperties(
    std::unique_ptr<BindingIter> child_iter,
    std::vector<ExprVarProperty> var_properties
) :
    child_iter(std::move(child_iter))
{
    assert(!var_properties.empty());
    for (auto& e : var_properties) {
        std::array<std::unique_ptr<ScanRange>, 3> ranges { ScanRange::get(e.var_without_property, true),
                                                           ScanRange::get(e.key),
                                                           ScanRange::get(e.var_with_property, false) };
        prop_scans.emplace_back(*quad_model.object_key_value, std::move(ranges));
        property_vars.emplace_back(e.var_with_property);
    }
}

void AssignProperties::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    child_iter->begin(*parent_binding);
    for (auto& scan : prop_scans) {
        scan.begin(*parent_binding);
    }
}

void AssignProperties::_reset()
{
    child_iter->reset();
}

bool AssignProperties::_next()
{
    if (child_iter->next()) {
        for (auto& scan : prop_scans) {
            scan.assign_nulls();
            scan.reset();
            scan.next();
        }

        return true;
    }
    return false;
}

void AssignProperties::assign_nulls()
{
    child_iter->assign_nulls();
    for (auto& scan : prop_scans) {
        scan.assign_nulls();
    }
}

void AssignProperties::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }

    os << std::string(indent, ' ') << "AssignProperties(";
    os << property_vars[0];
    for (std::size_t i = 1; i < property_vars.size(); ++i) {
        os << ", " << property_vars[i];
    }
    os << ")\n";

    child_iter->print(os, indent + 2, stats);
}
