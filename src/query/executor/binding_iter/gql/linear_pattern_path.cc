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

void LinearPatternPath::set_left_boundary(ObjectId boundary)
{
    parent_binding->add(variables_in_pattern.front(), boundary);
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

std::unique_ptr<PathBinding> LinearPatternPath::next()
{
    if (!child_iter->next()) {
        return nullptr;
    }

    PathBinding list;
    bool first = true;

    for (auto it = variables_in_pattern.begin(); it != variables_in_pattern.end(); ++it) {
        if (first) {
            // add the node only
            PathStep elem({ *it }, (*parent_binding)[*it]);
            list.push_right(elem);
            first = false;
        } else {
            // add the node, the edge and the direction
            VarId edge_var = *it;
            ObjectId edge_value = (*parent_binding)[*it];
            it++;
            ObjectId edge_direction = (*parent_binding)[*it];
            it++;

            PathStep elem({ *it }, (*parent_binding)[*it], edge_var, edge_value, edge_direction);
            list.push_right(elem);
        }
    }
    return std::make_unique<PathBinding>(list);
}

void LinearPatternPath::assign_nulls()
{
    for (auto& var : variables_in_pattern) {
        parent_binding->add(var, ObjectId::get_null());
    }
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
