#include "extend_left.h"

namespace GQL {

ExtendLeft::ExtendLeft(std::unique_ptr<PathBindingIter>&& left, std::unique_ptr<PathBindingIter>&& right) :
    left(std::move(left)),
    right(std::move(right))
{ }

void ExtendLeft::begin(Binding& binding)
{
    parent_binding = &binding;
    right->begin(binding);

    right_list = right->next();
    right_has_value = right_list != nullptr;

    left->begin(binding);
    if (right_has_value) {
        left->set_right_boundary(right_list->steps.front().node_value);
    }
}

void ExtendLeft::reset()
{
    right->reset();

    right_list = right->next();
    right_has_value = right_list != nullptr;

    if (right_has_value) {
        left->set_right_boundary(right_list->steps.front().node_value);
    }
}

void ExtendLeft::set_left_boundary(ObjectId boundary)
{
    left->set_left_boundary(boundary);

    left_list = left->next();
    right_has_value = right_list != nullptr;

    if (right_has_value) {
        left->set_right_boundary(right_list->steps.front().node_value);
    }
}

void ExtendLeft::set_right_boundary(ObjectId boundary)
{
    right->set_right_boundary(boundary);

    left_list = left->next();
    right_has_value = right_list != nullptr;

    if (right_has_value) {
        left->set_right_boundary(right_list->steps.front().node_value);
    }
}

std::unique_ptr<PathBinding> ExtendLeft::next()
{
    while (right_has_value) {
        left_list = left->next();
        if (left_list != nullptr) {
            auto final_list = right_list->clone();
            final_list->merge_right(left_list);
            return final_list;
        } else {
            right_list = right->next();
            right_has_value = right_list != nullptr;

            if (right_has_value) {
                left->set_right_boundary(right_list->steps.front().node_value);
            }
        }
    }

    return nullptr;
}

void ExtendLeft::assign_nulls()
{
    left->assign_nulls();
    right->assign_nulls();
}

void ExtendLeft::assign_empty()
{
    left->assign_empty();
    right->assign_empty();
}

void ExtendLeft::print(std::ostream& os, int indent, bool stats)
{
    os << std::string(indent, ' ');
    os << "ExtendLeft()\n";
    left->print(os, indent + 2, stats);
    right->print(os, indent + 2, stats);
}

} // namespace GQL
