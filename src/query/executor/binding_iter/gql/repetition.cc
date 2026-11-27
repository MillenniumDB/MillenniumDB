#include "repetition.h"

#include <algorithm>

Repetition::Repetition(
    std::unique_ptr<PathBindingIter> child_iter,
    uint64_t lower,
    std::optional<uint64_t> upper,
    bool trail,
    bool once
) :
    child_iter(std::move(child_iter)),
    lower(lower),
    // TRAIL needs no cap: no edge repeats, and an iteration that adds nothing goes no further
    upper(upper.value_or(trail ? UINT64_MAX : MAX_REPETITIONS)),
    trail(trail),
    once(trail && once)
{ }

void Repetition::begin(Binding& binding)
{
    parent_binding = &binding;
    child_iter->begin(binding);

    // no search until a boundary is set: one that was left unfinished must not go on
    pending = false;
    queue = {};
    return_zero_length = false;
}

void Repetition::start_search()
{
    pending = false;
    visited.clear();
    visited_paths.clear();
    queue = {};
    return_zero_length = lower == 0;

    // zero iterations: the path that only has the boundary, without variables
    PathBinding start_list;
    start_list.elements.push_back({ parent_boundary, {} });
    queue.push_back({ start_list, 0 });
    visited.insert({ parent_boundary, 0 });
    visited_paths.insert({ start_list, 0 });
}

// set_left_boundary or set_right_boundary must be called before next
void Repetition::set_left_boundary(const PathElement& left_boundary)
{
    parent_boundary = left_boundary;
    child_iter->set_left_boundary(left_boundary);
    start_search();
}

void Repetition::set_right_boundary(ObjectId right_boundary)
{
    parent_boundary = PathElement::node(right_boundary);
    child_iter->set_right_boundary(right_boundary);
    start_search();
}

void Repetition::reset()
{
    child_iter->reset();
    start_search();
}

std::unique_ptr<PathBinding> Repetition::add_iteration(const PathBinding& iteration)
{
    // The search only goes on from an element the first time it is reached. Below the lower bound
    // the number of iterations counts too: a path that is too short does not rule out a longer one.
    // TRAIL goes on from each path instead, the first time it is reached, if it repeats no edge.
    const uint64_t repetitions = cur_repetitions + 1;
    if (!trail && !visited.insert({ iteration.end_element(), std::min(repetitions, lower) }).second) {
        return nullptr;
    }

    auto path = cur_list.clone();
    path->concat(iteration);

    if (trail) {
        if (path->repeats_edge()) {
            return nullptr;
        }
        if (once) {
            // past the lower bound, an iteration that adds nothing gives the same paths again
            const bool same = path->elements.size() == cur_list.elements.size()
                           && path->without_internal_vars() == cur_list.without_internal_vars();
            if (same && cur_repetitions >= lower) {
                return nullptr;
            }
        } else {
            // unions, nested quantifiers and iterations without edges can reach a path more than once
            auto key = std::make_pair(path->without_internal_vars(), std::min(repetitions, lower));
            if (!visited_paths.insert(std::move(key)).second) {
                return nullptr;
            }
        }
    }

    if (repetitions < upper) {
        queue.push_back({ *path, repetitions });
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

        if (auto path = add_iteration(*child_list)) {
            return path;
        }
    }
    pending = false;

    while (!queue.empty()) {
        // with once, the newest path goes on first, so the queue holds a few paths per iteration
        auto& [list, repetitions] = once ? queue.back() : queue.front();
        cur_list = std::move(list);
        cur_repetitions = repetitions;
        if (once) {
            queue.pop_back();
        } else {
            queue.pop_front();
        }

        child_iter->set_left_boundary(cur_list.end_element());

        while (true) {
            auto child_list = child_iter->next();
            if (child_list == nullptr) {
                break;
            }

            if (auto path = add_iteration(*child_list)) {
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
    os << std::string(indent, ' ') << "Repetition(lower:" << lower << ", upper:" << upper;
    os << (trail ? ", trail" : "") << (once ? ", once" : "") << ")\n";
    child_iter->print(os, indent + 2, stats);
}
