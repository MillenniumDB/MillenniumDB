#pragma once

#include "graph_models/object_id.h"
#include "storage/tuple_collection/tuple_collection.h"

class FixedSizeTuplePriorityQueue {
public:
    using ObjectIdTupleType = std::vector<ObjectId>;

    class CompareTuples {
    public:
        CompareTuples() = delete;

        explicit CompareTuples(const FixedSizeTuplePriorityQueue& fixed_size_pq_) :
            fixed_size_pq { fixed_size_pq_ }
        { }

        inline bool operator()(const ObjectIdTupleType& lhs, const ObjectIdTupleType& rhs) const
        {
            return fixed_size_pq.less(lhs, rhs);
        }

    private:
        const FixedSizeTuplePriorityQueue& fixed_size_pq;
    };

    explicit FixedSizeTuplePriorityQueue(
        const OrderInfo& info_,
        int64_t (*compare_)(ObjectId, ObjectId),
        uint64_t limit_
    ) :
        info { info_ },
        limit { limit_ },
        compare { compare_ },
        priority_queue { CompareTuples(*this) }
    { }

    using PriorityQueueType = std::multiset<ObjectIdTupleType, CompareTuples>;

    // expects a tuple with values corresponding to saved_vars
    // adds a new element making sure that the priority queue does not surpass the limit
    inline void add(const ObjectIdTupleType& tuple)
    {
        priority_queue.emplace(tuple);
        if (priority_queue.size() > limit) {
            const auto last_it = --priority_queue.end();
            priority_queue.erase(last_it);
        }
    }

    PriorityQueueType::iterator begin()
    {
        return priority_queue.begin();
    }

    PriorityQueueType::iterator end()
    {
        return priority_queue.end();
    }

private:
    inline bool less(const std::vector<ObjectId>& lhs, const std::vector<ObjectId>& rhs) const
    {
        for (std::size_t i = 0; i < info.ascending.size(); ++i) {
            const auto cmp = compare(lhs[i], rhs[i]);
            if (cmp < 0) {
                // lhs < rhs
                return info.ascending[i];
            } else if (cmp > 0) {
                // lhs > rhs
                return !info.ascending[i];
            }
        }
        // lhs == rhs
        return false;
    }

    const OrderInfo& info;

    const uint64_t limit;

    int64_t (*compare)(ObjectId, ObjectId);

    PriorityQueueType priority_queue;
};
