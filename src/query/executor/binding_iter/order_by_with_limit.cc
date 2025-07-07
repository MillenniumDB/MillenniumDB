#include "order_by_with_limit.h"

OrderByWithLimit::OrderByWithLimit(
    std::unique_ptr<BindingIter> child_iter_,
    const std::set<VarId>& saved_vars_,
    std::vector<VarId>&& order_vars_,
    std::vector<bool>&& ascending_,
    int64_t (*compare_)(ObjectId, ObjectId),
    uint64_t limit_
) :
    child_iter { std::move(child_iter_) },
    order_info { std::move(order_vars_), std::move(ascending_), saved_vars_ },
    compare { compare_ },
    limit { limit_ },
    limited_priority_queue { order_info, compare, limit }
{ }

void OrderByWithLimit::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;
    child_iter->begin(*parent_binding);

    std::vector<ObjectId> object_ids(order_info.saved_vars.size());

    // Insert all the tuples of child_iter into the limited priority queue
    while (child_iter->next()) {
        for (std::size_t i = 0; i < order_info.saved_vars.size(); ++i) {
            object_ids[i] = parent_binding->operator[](order_info.saved_vars[i]);
        }
        limited_priority_queue.add(object_ids);
    }

    limited_priority_queue_it = limited_priority_queue.begin();
}

void OrderByWithLimit::_reset()
{
    limited_priority_queue_it = limited_priority_queue.begin();
}

bool OrderByWithLimit::_next()
{
    if (limited_priority_queue_it != limited_priority_queue.end()) {
        for (std::size_t i = 0; i < order_info.saved_vars.size(); ++i) {
            parent_binding->add(order_info.saved_vars[i], limited_priority_queue_it->operator[](i));
        }
        ++limited_priority_queue_it;
        return true;
    }

    return false;
}

void OrderByWithLimit::assign_nulls()
{
    for (const auto& var : order_info.saved_vars) {
        (*parent_binding)[var] = ObjectId::get_null();
    }
}

void OrderByWithLimit::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "OrderByWithLimit(order_vars: ";
    auto first = true;
    for (size_t i = 0; i < order_info.ascending.size(); i++) {
        if (first)
            first = false;
        else
            os << " ";
        if (order_info.ascending[i])
            os << "ASC ";
        else
            os << "DESC ";
        os << order_info.saved_vars[i];
    }

    os << ", saved_vars: ";
    first = true;
    for (auto var : order_info.saved_vars) {
        if (first)
            first = false;
        else
            os << " ";
        os << var;
    }

    os << ", limit: ";
    os << limit;

    os << ")\n";
    child_iter->print(os, indent + 2, stats);
}
