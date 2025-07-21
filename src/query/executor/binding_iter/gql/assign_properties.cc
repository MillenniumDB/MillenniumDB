#include "assign_properties.h"

#include "graph_models/gql/gql_model.h"
#include "query/parser/expr/gql/expr.h"
#include "query/parser/expr/gql/expr_property.h"

using namespace GQL;

AssignProperties::AssignProperties(
    std::unique_ptr<BindingIter> child_iter,
    std::vector<ExprProperty> var_properties
) :
    child_iter(std::move(child_iter))
{
    for (auto& e : var_properties) {
        std::array<std::unique_ptr<ScanRange>, 3> ranges { ScanRange::get(e.object, true),
                                                           ScanRange::get(e.key),
                                                           ScanRange::get(e.value, false) };
        if (e.type == VarType::Node) {
            prop_scans.emplace_back(*gql_model.node_key_value, std::move(ranges));
        } else {
            prop_scans.emplace_back(*gql_model.edge_key_value, std::move(ranges));
        }
        property_vars.emplace_back(e.value);
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
