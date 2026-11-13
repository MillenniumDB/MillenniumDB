#include "path_filter.h"

#include "graph_models/gql/conversions.h"
#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"

using namespace GQL;

PathFilter::PathFilter(
    std::unique_ptr<PathBindingIter> child_iter,
    std::vector<std::unique_ptr<BindingExpr>>&& filters,
    const std::vector<ExprProperty>& properties
) :
    child_iter(std::move(child_iter)),
    filters(std::move(filters))
{
    for (auto& e : properties) {
        std::array<std::unique_ptr<ScanRange>, 3> ranges { ScanRange::get(e.object, true),
                                                           ScanRange::get(e.key),
                                                           ScanRange::get(e.value, false) };
        if (e.type == VarType::Node) {
            prop_scans.emplace_back(*gql_model.node_key_value, std::move(ranges));
        } else {
            prop_scans.emplace_back(*gql_model.edge_key_value, std::move(ranges));
        }
    }
}

void PathFilter::begin(Binding& binding)
{
    parent_binding = &binding;
    child_iter->begin(binding);
    for (auto& scan : prop_scans) {
        scan.begin(binding);
    }
}

void PathFilter::reset()
{
    child_iter->reset();
}

void PathFilter::set_left_boundary(ObjectId boundary)
{
    child_iter->set_left_boundary(boundary);
}

void PathFilter::set_right_boundary(ObjectId boundary)
{
    child_iter->set_right_boundary(boundary);
}

std::unique_ptr<PathBinding> PathFilter::next()
{
    while (auto path = child_iter->next()) {
        results_checked++;
        for (auto& scan : prop_scans) {
            scan.assign_nulls();
            scan.reset();
            scan.next();
        }

        bool pass_filters = true;
        for (auto& filter : filters) {
            if (!Conversions::to_boolean(filter->eval(*parent_binding)).is_true()) {
                pass_filters = false;
                break;
            }
        }
        if (pass_filters) {
            results_passed++;
            return path;
        }
    }
    return nullptr;
}

void PathFilter::assign_nulls()
{
    child_iter->assign_nulls();
    for (auto& scan : prop_scans) {
        scan.assign_nulls();
    }
}

void PathFilter::assign_empty()
{
    child_iter->assign_empty();
}

void PathFilter::print(std::ostream& os, int indent, bool stats)
{
    if (stats) {
        os << std::string(indent, ' ') << "[checked: " << results_checked << " passed: " << results_passed
           << "]\n";
    }
    os << std::string(indent, ' ') << "PathFilter(\n";
    {
        BindingExprPrinter printer(os, indent, stats);
        for (auto& filter : filters) {
            os << std::string(indent + 2, ' ');
            printer.print(*filter);
            os << "\n";
        }
    }
    os << std::string(indent, ' ') << ")\n";
    child_iter->print(os, indent + 2, stats);
}
