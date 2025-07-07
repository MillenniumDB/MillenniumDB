#include "union_path.h"

namespace GQL {

UnionPath::UnionPath(std::vector<std::unique_ptr<PathBindingIter>>&& iters) :
    iters(std::move(iters))
{ }

void UnionPath::begin(Binding& binding)
{
    parent_binding = &binding;
    iters[0]->begin(binding);
}

void UnionPath::reset()
{
    if (current_iter < iters.size()) {
        iters[current_iter]->assign_nulls();
    }

    current_iter = 0;
    iters[0]->reset();
}

void UnionPath::set_left_boundary(ObjectId boundary)
{
    parent_boundary = boundary;

    if (current_iter < iters.size()) {
        iters[current_iter]->assign_nulls();
    }
    current_iter = 0;
    iters[0]->set_left_boundary(boundary);
}

void UnionPath::set_right_boundary(ObjectId boundary)
{
    parent_boundary = boundary;

    if (current_iter < iters.size()) {
        iters[current_iter]->assign_nulls();
    }
    current_iter = 0;
    iters[0]->set_right_boundary(boundary);
}

std::unique_ptr<PathBinding> UnionPath::next()
{
    while (current_iter < iters.size()) {
        auto list = iters[current_iter]->next();
        if (list != nullptr) {
            return list;
        } else {
            iters[current_iter]->assign_nulls();
            current_iter++;
            if (current_iter < iters.size()) {
                iters[current_iter]->begin(*parent_binding);
                iters[current_iter]->set_left_boundary(parent_boundary);
            }
        }
    }
    return nullptr;
}

void UnionPath::assign_nulls()
{
    if (current_iter < iters.size()) {
        iters[current_iter]->assign_nulls();
    }
}

void UnionPath::assign_empty()
{
    if (current_iter < iters.size()) {
        iters[current_iter]->assign_empty();
    }
}

void UnionPath::print(std::ostream& os, int indent, bool stats)
{
    os << std::string(indent, ' ');
    os << "UnionPath()\n";
    for (auto& iter : iters) {
        iter->print(os, indent + 2, stats);
    }
}
} // namespace GQL
