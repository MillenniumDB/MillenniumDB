#pragma once

#include <memory>
#include <set>
#include <vector>

#include <boost/container/flat_set.hpp>

#include "graph_models/object_id.h"
#include "misc/fixed_size_tuple_priority_queue.h"
#include "query/executor/binding_iter.h"
#include "storage/tuple_collection/tuple_collection.h"

// Order by in memory with a priority queue of size limit
class OrderByWithLimit : public BindingIter {
public:
    OrderByWithLimit(
        std::unique_ptr<BindingIter> child_iter,
        const std::set<VarId>& saved_vars,
        std::vector<VarId>&& order_vars,
        std::vector<bool>&& ascending,
        int64_t (*_compare)(ObjectId, ObjectId),
        uint64_t limit
    );

    OrderByWithLimit(const OrderByWithLimit&) = delete;

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override;

    void print(std::ostream& os, int indent, bool stats) const override;

    std::unique_ptr<BindingIter> child_iter;

    OrderInfo order_info;

private:
    Binding* parent_binding;

    int64_t (*compare)(ObjectId, ObjectId);

    const uint64_t limit;

    FixedSizeTuplePriorityQueue limited_priority_queue;

    FixedSizeTuplePriorityQueue::PriorityQueueType::iterator limited_priority_queue_it;
};
