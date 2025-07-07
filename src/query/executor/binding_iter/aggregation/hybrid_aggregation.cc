#include "hybrid_aggregation.h"

HybridAggregation::HybridAggregation(
    std::unique_ptr<BindingIter> child,
    std::map<VarId, std::unique_ptr<Agg>>&& _aggregations,
    std::set<VarId>&& _group_vars,
    std::set<VarId>&& _order_saved_vars
) :
    child(std::move(child)),
    aggregations(std::move(_aggregations)),
    group_vars(std::move(_group_vars)),
    ordered_group(group_vars, std::move(_order_saved_vars))
{
    for (auto&& [var, agg] : aggregations) {
        u_aggregations.insert({ var, agg->get_uagg() });
    }
    buffer = new char[1 * 1024 * 1024]; // TODO: assuming this is enough
}

HybridAggregation::~HybridAggregation()
{
    delete[] buffer;
}

void HybridAggregation::_begin(Binding& _parent_binding)
{
    parent_binding = &_parent_binding;

    child_binding = Binding(parent_binding->size);
    ordered_group_binding = Binding(parent_binding->size);

    child->begin(child_binding);
    ordered_group.begin(ordered_group_binding);

    key_buf.resize(group_vars.size());

    for (auto&& [var_id, agg] : u_aggregations) {
        agg->set_binding(child_binding);
    }

    for (auto&& [var_id, agg] : aggregations) {
        agg->set_binding(ordered_group_binding);
    }

    prepare();
}

void HybridAggregation::_reset()
{
    iter = groups.begin();
    ordered_group.reset();
    pending_ordered_group = ordered_group.next();
}

void HybridAggregation::prepare()
{
    size_t data_size = 0;

    for (auto&& [var_id, agg] : u_aggregations) {
        data_size += agg->get_offset();
    }

    size_t buffer_size = 1 * 1024 * 1024; // TODO: make class attribute?
    size_t max_buffer_groups = data_size == 0 ? UINT32_MAX : buffer_size / data_size;
    // size_t max_buffer_groups = 1;

    while (child->next()) {
        size_t i = 0;
        for (auto& var_id : group_vars) {
            key_buf[i] = child_binding[var_id];
            i++;
        }

        auto it = groups.find(key_buf);
        char* data;
        if (it == groups.end()) {
            data = buffer + (groups.size() * data_size);
            groups.insert({ key_buf, data });

            size_t offset = 0;
            for (auto&& [var, agg] : u_aggregations) {
                agg->begin(data + offset);
                offset += agg->get_offset();
            }
        } else {
            data = it->second;
        }

        size_t offset = 0;
        for (auto&& [var, agg] : u_aggregations) {
            agg->process(data + offset);
            offset += agg->get_offset();
        }

        if (groups.size() >= max_buffer_groups) {
            goto buffer_full;
        }
    }
    goto all_tuples_in_buffer;

buffer_full:
    while (child->next()) {
        size_t i = 0;
        for (auto& var_id : group_vars) {
            key_buf[i] = child_binding[var_id];
            i++;
        }

        auto it = groups.find(key_buf);
        if (it == groups.end()) {
            ordered_group.add(child_binding);
        } else {
            char* data = it->second;
            size_t offset = 0;
            for (auto&& [var, agg] : u_aggregations) {
                agg->process(data + offset);
                offset += agg->get_offset();
            }
        }
    }
    ordered_group.end_appends();

all_tuples_in_buffer:
    // We have aggregation without grouping and have never returned a binding.
    // But for aggregation without grouping we have to return at least once.
    if (groups.empty() && group_vars.size() == 0) {
        char* data = buffer + (groups.size() * data_size);
        groups.insert({ key_buf, data });
        size_t offset = 0;
        for (auto&& [var, agg] : u_aggregations) {
            agg->begin(data + offset);
            offset += agg->get_offset();
        }
    }

    iter = groups.begin();

    pending_ordered_group = ordered_group.next();
}

bool HybridAggregation::_next()
{
    if (iter != groups.end()) {
        size_t offset = 0;
        for (auto&& [var_id, agg] : u_aggregations) {
            parent_binding->add(var_id, agg->get(iter->second + offset));
            offset += agg->get_offset();
        }
        size_t i = 0;
        for (auto& var_id : group_vars) {
            parent_binding->add(var_id, iter->first[i]);
            i++;
        }
        iter++;
        return true;
    }

    // Tuples from order
    if (pending_ordered_group) {
        // remember group
        for (auto& var_id : group_vars) {
            parent_binding->add(var_id, ordered_group_binding[var_id]);
        }
        for (auto&& [var_id, agg] : aggregations) {
            agg->begin();
            agg->process();
        }
    } else {
        return false;
    }

    pending_ordered_group = false;
    while (ordered_group.next()) {
        bool same_group = true;
        // check if group is changed
        for (auto& var_id : group_vars) {
            if (ordered_group_binding[var_id] != (*parent_binding)[var_id]) {
                same_group = false;
            }
        }
        if (same_group) {
            for (auto&& [var_id, agg] : aggregations) {
                agg->process();
            }
        } else {
            pending_ordered_group = true;
            break;
        }
    }
    for (auto&& [var_id, agg] : aggregations) {
        parent_binding->add(var_id, agg->get());
    }
    return true;
}

void HybridAggregation::assign_nulls()
{
    for (auto&& [var_id, agg] : u_aggregations) {
        parent_binding->add(var_id, ObjectId::get_null());
    }
    child->assign_nulls();
}

void HybridAggregation::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "HybridAggregation(";

    if (group_vars.size() > 0) {
        os << "group vars:";

        for (auto var : group_vars) {
            os << " " << var;
        }
        if (u_aggregations.size() > 0) {
            os << " ";
        }
    }

    if (u_aggregations.size() > 0) {
        os << "aggregations:";

        for (auto& [var, agg] : u_aggregations) {
            os << " " << var << '=' << *agg;
        }
    }
    os << ")\n";
    child->print(os, indent + 2, stats);
}
