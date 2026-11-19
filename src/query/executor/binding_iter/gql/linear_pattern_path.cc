#include "linear_pattern_path.h"
#include "graph_models/gql/conversions.h"

namespace GQL {

LinearPatternPath::LinearPatternPath(
    std::unique_ptr<BindingIter>&& iter,
    std::vector<VarId>&& variables_in_pattern
) :
    child_iter(std::move(iter)),
    variables_in_pattern(variables_in_pattern),
    pattern_size(variables_in_pattern.size())
{ }

void LinearPatternPath::begin(Binding& binding)
{
    parent_binding = &binding;
    child_iter->begin(binding);
}

void LinearPatternPath::reset()
{
    child_iter->reset();
}

void LinearPatternPath::set_left_boundary(const PathElement& boundary)
{
    if (boundary.is_edge && off_path_vars.count(variables_in_pattern.front())) {
        // edge·edge join: the pattern starts with the edge on the left
        parent_binding->add(variables_in_pattern.front(), boundary.start_node());
        required_first_edge = boundary;
    } else {
        // the pattern starts with a node: the node at which the path on the left ends
        parent_binding->add(variables_in_pattern.front(), boundary.end_node());
        required_first_edge.reset();
    }
    // we call reset here, so that the child iter sets the ranges of the bpt iter and
    // the assign nulls function can be called safely
    child_iter->reset();
}

void LinearPatternPath::set_right_boundary(ObjectId boundary)
{
    parent_binding->add(variables_in_pattern.back(), boundary);
    // we call reset here, so that the child iter sets the ranges of the bpt iter and
    // the assign nulls function can be called safely
    child_iter->reset();
}

PathElement LinearPatternPath::node_at(size_t i) const
{
    return PathElement::node((*parent_binding)[variables_in_pattern[i]]);
}

PathElement LinearPatternPath::edge_at(size_t i) const
{
    // the variables around i are: node before the edge, edge, direction, node after the edge
    return PathElement::edge(
        (*parent_binding)[variables_in_pattern[i]],
        (*parent_binding)[variables_in_pattern[i + 1]],
        (*parent_binding)[variables_in_pattern[i - 1]],
        (*parent_binding)[variables_in_pattern[i + 2]]
    );
}

std::unique_ptr<PathBinding> LinearPatternPath::next()
{
    // an edge·edge join only takes the results that start with the edge on the left
    do {
        if (!child_iter->next()) {
            return nullptr;
        }
    } while (required_first_edge && edge_at(1) != *required_first_edge);

    auto path = std::make_unique<PathBinding>();
    path->elements.reserve(pattern_size); // every element has a variable in the pattern
    auto push_node = [&](size_t i) {
        if (!off_path_vars.count(variables_in_pattern[i])) {
            path->push_right(node_at(i), variables_in_pattern[i]);
        }
    };
    push_node(0);
    for (size_t i = 1; i < pattern_size; i += 3) {
        path->push_right(edge_at(i), variables_in_pattern[i]);
        push_node(i + 2);
    }
    return path;
}

void LinearPatternPath::assign_nulls()
{
    for (auto& var : variables_in_pattern) {
        if (!outer_vars.count(var)) {
            parent_binding->add(var, ObjectId::get_null());
        }
    }
}

void LinearPatternPath::set_outer_vars(std::set<VarId>&& vars)
{
    outer_vars = std::move(vars);
}

void LinearPatternPath::set_off_path_vars(std::set<VarId>&& vars)
{
    off_path_vars = std::move(vars);
}

void LinearPatternPath::assign_empty()
{
    for (auto& var : variables_in_pattern) {
        parent_binding->add(var, Conversions::pack_list({}));
    }
}

void LinearPatternPath::print(std::ostream& os, int indent, bool stats)
{
    os << std::string(indent, ' ') << "LinearPatternPath(vars: ";
    bool first = true;
    for (auto& var : variables_in_pattern) {
        if (first) {
            os << var;
            first = false;
        } else {
            os << " " << var;
        }
    }

    os << ")\n";
    child_iter->print(os, indent + 2, stats);
}

} // namespace GQL
