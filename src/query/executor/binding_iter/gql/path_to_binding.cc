#include "path_to_binding.h"
#include "graph_models/gql/conversions.h"

PathToBinding::PathToBinding(
    std::unique_ptr<PathBindingIter> iter,
    std::map<VarId, GQL::VarType> types,
    std::optional<VarId> path_var
) :
    child_iter(std::move(iter)),
    path_var(path_var),
    types(types)
{ }

void PathToBinding::_begin(Binding& parent_binding)
{
    this->parent_binding = &parent_binding;
    child_iter->begin(parent_binding);
}

void PathToBinding::_reset()
{
    child_iter->reset();
}

bool PathToBinding::_next()
{
    auto list = child_iter->next();
    if (list == nullptr) {
        return false;
    }

    std::map<VarId, std::vector<ObjectId>> groups;

    bool first = true;
    for (auto it = list->steps.begin(); it != list->steps.end(); ++it) {
        // first element of the list does not contain an edge
        if (first) {
            first = false;
        } else {
            if (types[it->edge_var].is_group()) {
                groups[it->edge_var].push_back(it->edge_value);
            } else {
                parent_binding->add(it->edge_var, it->edge_value);
            }
        }
        for (auto& node_var : it->node_vars) {
            if (types[node_var].is_group()) {
                groups[node_var].push_back(it->node_value);
            } else {
                parent_binding->add(node_var, it->node_value);
            }
        }
    }

    for (auto&& [var, oids] : groups) {
        parent_binding->add(var, GQL::Conversions::pack_list(oids));
    }
    if (path_var.has_value()) {
        assign_path_var(list);
    }

    return true;
}

void PathToBinding::assign_path_var(std::unique_ptr<PathBinding>& list)
{
    std::vector<ObjectId> path_oids;

    bool first = true;
    for (auto elem : list->steps) {
        if (first) {
            path_oids.push_back(elem.node_value);
            first = false;
        } else {
            path_oids.push_back(elem.edge_value);
            path_oids.push_back(elem.edge_direction);
            path_oids.push_back(elem.node_value);
        }
    }

    parent_binding->add(*path_var, GQL::Conversions::pack_path(path_oids));
}

void PathToBinding::assign_nulls()
{
    child_iter->assign_nulls();
}

void PathToBinding::print(std::ostream& os, int indent, bool stats) const
{
    os << std::string(indent, ' ');

    if (path_var.has_value()) {
        os << "PathToBinding(" << *path_var << ")\n";
    } else {
        os << "PathToBinding()\n";
    }

    child_iter->print(os, indent + 2, stats);
}
