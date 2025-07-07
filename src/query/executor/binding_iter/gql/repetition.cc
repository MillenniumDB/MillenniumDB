#include "repetition.h"

Repetition::Repetition(
    std::unique_ptr<PathBindingIter> child_iter,
    uint64_t lower,
    std::optional<uint64_t> upper
) :
    child_iter(std::move(child_iter)),
    lower(lower),
    upper(upper.value_or(MAX_REPETITIONS)),
    return_zero_length(lower == 0)
{ }

void Repetition::begin(Binding& binding)
{
    parent_binding = &binding;
    child_iter->begin(binding);
}

// set_left_boundary or set_right_boundary must be called before next
void Repetition::set_left_boundary(ObjectId left_boundary)
{
    parent_boundary = left_boundary;
    child_iter->set_left_boundary(left_boundary);

    pending = false;
    visited.clear();
    queue = {};
    return_zero_length = lower == 0;

    PathBinding start_list;
    PathStep node({}, left_boundary);
    start_list.push_left(node);
    queue.push({ start_list, 0 });
    visited.insert(left_boundary);
}

void Repetition::set_right_boundary(ObjectId right_boundary)
{
    parent_boundary = right_boundary;
    child_iter->set_right_boundary(right_boundary);

    pending = false;
    visited.clear();
    queue = {};
    return_zero_length = lower == 0;

    PathBinding start_list;
    PathStep node({}, right_boundary);
    start_list.push_left(node);
    queue.push({ start_list, 0 });
    visited.insert(right_boundary);
}

void Repetition::reset()
{
    child_iter->reset();

    pending = false;
    visited.clear();
    queue = {};
    return_zero_length = lower == 0;

    PathBinding start_list;
    PathStep node({}, parent_boundary);
    start_list.push_right(node);
    queue.push({ start_list, 0 });
    visited.insert(parent_boundary);
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

        bool continue_visitation = true;

        // the list should not be empty
        if (visited.count(child_list->steps.back().node_value)) {
            continue_visitation = false;
        } else {
            visited.insert(child_list->steps.back().node_value);
        }

        PathBinding cur_list_copy = cur_list;
        cur_list_copy.merge_right(child_list);

        if (continue_visitation && cur_repetitions + 1 < upper) {
            queue.push({ cur_list_copy, cur_repetitions + 1 });
        }

        if (continue_visitation && lower <= cur_repetitions + 1 && cur_repetitions + 1 <= upper) {
            return std::make_unique<PathBinding>(cur_list_copy);
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

            bool continue_visitation = true;

            // the list should not be empty
            if (visited.count(child_list->steps.back().node_value)) {
                continue_visitation = false;
            } else {
                visited.insert(child_list->steps.back().node_value);
            }

            PathBinding cur_list_copy = cur_list;
            cur_list_copy.merge_right(child_list);

            if (continue_visitation && cur_repetitions + 1 < upper) {
                queue.push({ cur_list_copy, cur_repetitions + 1 });
            }

            if (continue_visitation && lower <= cur_repetitions + 1 && cur_repetitions + 1 <= upper) {
                pending = true;
                return std::make_unique<PathBinding>(cur_list_copy);
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
