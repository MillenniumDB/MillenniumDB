#include "repetition.h"

Repetition::Repetition(
    std::unique_ptr<PathBindingIter> child_iter,
    uint64_t lower,
    std::optional<uint64_t> upper
) :
    child_iter(std::move(child_iter)),
    lower(lower),
    upper(upper.value_or(MAX_REPETITIONS))
{ }

void Repetition::begin(Binding& binding)
{
    parent_binding = &binding;
    child_iter->begin(binding);
}

void Repetition::start_search()
{
    pending = false;
    visited.clear();
    queue = {};
    return_zero_length = lower == 0;

    // zero iterations: the path that only has the boundary node
    PathBinding start_list;
    start_list.push_right(PathStep({}, parent_boundary));
    queue.push({ start_list, 0 });
    visited.insert(parent_boundary);
}

// set_left_boundary or set_right_boundary must be called before next
void Repetition::set_left_boundary(ObjectId left_boundary)
{
    parent_boundary = left_boundary;
    child_iter->set_left_boundary(left_boundary);
    start_search();
}

void Repetition::set_right_boundary(ObjectId right_boundary)
{
    parent_boundary = right_boundary;
    child_iter->set_right_boundary(right_boundary);
    start_search();
}

void Repetition::reset()
{
    child_iter->reset();
    start_search();
}

std::unique_ptr<PathBinding> Repetition::add_iteration(const std::unique_ptr<PathBinding>& child_list)
{
    // the search only goes on from a node the first time it is reached
    if (!visited.insert(child_list->steps.back().node_value).second) {
        return nullptr;
    }

    auto path = cur_list.clone();
    path->merge_right(child_list);

    const uint64_t repetitions = cur_repetitions + 1;
    if (repetitions < upper) {
        queue.push({ *path, repetitions });
    }
    if (lower <= repetitions && repetitions <= upper) {
        return path;
    }
    return nullptr;
}

std::unique_ptr<PathBinding> Repetition::next()
{
    if (return_zero_length) {
        return_zero_length = false;
        child_iter->assign_empty();
        return std::make_unique<PathBinding>(queue.front().first);
    }

    // if a node did not visit all its neighbors in the previous call, it continues the visitation
    while (pending) {
        auto child_list = child_iter->next();

        if (child_list == nullptr) {
            break;
        }

        if (auto path = add_iteration(child_list)) {
            return path;
        }
    }
    pending = false;

    while (!queue.empty()) {
        cur_list = queue.front().first;
        cur_repetitions = queue.front().second;
        queue.pop();

        child_iter->set_left_boundary(cur_list.back().node_value);

        while (true) {
            auto child_list = child_iter->next();
            if (child_list == nullptr) {
                break;
            }

            if (auto path = add_iteration(child_list)) {
                pending = true;
                return path;
            }
        }
    }

    return nullptr;
}

void Repetition::assign_nulls()
{
    child_iter->assign_nulls();
}

void Repetition::assign_empty()
{
    child_iter->assign_empty();
}

void Repetition::print(std::ostream& os, int indent, bool stats)
{
    os << std::string(indent, ' ') << "Repetition(lower:" << lower << ", upper:" << upper << ")\n";
    child_iter->print(os, indent + 2, stats);
}
