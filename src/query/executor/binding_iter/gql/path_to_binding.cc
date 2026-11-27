#include "path_to_binding.h"

#include <cassert>

#include "graph_models/gql/conversions.h"

PathToBinding::PathToBinding(
    std::unique_ptr<PathBindingIter> iter,
    std::map<VarId, GQL::VarType> types,
    std::optional<VarId> path_var,
    bool trail
) :
    child_iter(std::move(iter)),
    path_var(path_var),
    types(types),
    trail(trail)
{ }

void PathToBinding::_begin(Binding& parent_binding)
{
    this->parent_binding = &parent_binding;
    empty_list = GQL::Conversions::pack_list({});
    child_iter->begin(parent_binding);
}

void PathToBinding::_reset()
{
    restore_overwritten_values();
    child_iter->reset();
}

void PathToBinding::restore_overwritten_values()
{
    for (auto& [var, value] : overwritten_values) {
        parent_binding->add(var, value);
    }
    overwritten_values.clear();
}

bool PathToBinding::_next()
{
    restore_overwritten_values();

    auto list = child_iter->next();
    while (trail && list != nullptr && list->repeats_edge()) {
        list = child_iter->next();
    }
    if (list == nullptr) {
        return false;
    }

    // every group variable gets a list: the empty one, unless the path has elements for it
    for (auto& [var, type] : types) {
        if (type.is_group()) {
            overwritten_values.emplace_back(var, (*parent_binding)[var]);
            parent_binding->add(var, empty_list);
        }
    }

    std::map<VarId, std::vector<ObjectId>> groups;

    for (auto& [element, vars] : list->elements) {
        for (auto& var : vars) {
            if (types[var].is_group()) {
                groups[var].push_back(element.id);
            } else {
                parent_binding->add(var, element.id);
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
    // A GQL path value is node, (edge, direction, node)*
    assert(!list->elements.empty());
    assert(!list->elements.front().element.is_edge && !list->elements.back().element.is_edge);

    std::vector<ObjectId> path_oids;

    for (auto& bound : list->elements) {
        path_oids.push_back(bound.element.id);
        if (bound.element.is_edge) {
            path_oids.push_back(bound.element.direction);
        }
    }

    parent_binding->add(*path_var, GQL::Conversions::pack_path(path_oids));
}

void PathToBinding::assign_nulls()
{
    overwritten_values.clear();
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
