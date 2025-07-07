#include "extend_right.h"

namespace GQL {

ExtendRight::ExtendRight(std::unique_ptr<PathBindingIter>&& left, std::unique_ptr<PathBindingIter>&& right) :
    left(std::move(left)),
    right(std::move(right))
{ }

void ExtendRight::begin(Binding& binding)
{
    parent_binding = &binding;
    left->begin(binding);

    left_list = left->next();
    left_has_value = left_list != nullptr;

    right->begin(binding);
    if (left_has_value) {
        right->set_left_boundary(left_list->steps.back().node_value);
    }
}

void ExtendRight::reset()
{
    left->reset();

    left_list = left->next();
    left_has_value = left_list != nullptr;

    if (left_has_value) {
        right->set_left_boundary(left_list->steps.back().node_value);
    }
}

void ExtendRight::set_left_boundary(ObjectId boundary)
{
    left->set_left_boundary(boundary);

    left_list = left->next();
    left_has_value = left_list != nullptr;

    if (left_has_value) {
        right->set_left_boundary(left_list->steps.back().node_value);
    }
}

void ExtendRight::set_right_boundary(ObjectId boundary)
{
    right->set_right_boundary(boundary);
}

std::unique_ptr<PathBinding> ExtendRight::next()
{
    while (left_has_value) {
        right_list = right->next();
        if (right_list != nullptr) {
            auto final_list = left_list->clone();
            final_list->merge_right(right_list);
            return final_list;
        } else {
            left_list = left->next();
            left_has_value = left_list != nullptr;

            if (left_has_value) {
                right->set_left_boundary(left_list->steps.back().node_value);
            }
        }
    }
    return nullptr;
}

void ExtendRight::assign_nulls()
{
    left->assign_nulls();
    right->assign_nulls();
}

void ExtendRight::assign_empty()
{
    left->assign_empty();
    right->assign_empty();
}

void ExtendRight::print(std::ostream& os, int indent, bool stats)
{
    os << std::string(indent, ' ');
    os << "ExtendRight()\n";
    left->print(os, indent + 2, stats);
    right->print(os, indent + 2, stats);
}

} // namespace GQL
