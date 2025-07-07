#pragma once

#include <map>
#include <memory>
#include <set>

#include <boost/unordered/unordered_map.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/executor/binding_iter/aggregation/unordered_agg.h"
#include "storage/ordered_group/ordered_group.h"
#include "third_party/hashes/hash_function_wrapper.h"

class HybridAggregation : public BindingIter {
public:
    struct OIDVectorHasher {
        size_t operator()(const std::vector<ObjectId>& v) const
        {
            return HashFunctionWrapper(v.data(), v.size() * sizeof(ObjectId));
        }
    };

    HybridAggregation(
        std::unique_ptr<BindingIter> child,
        std::map<VarId, std::unique_ptr<Agg>>&& aggregations,
        std::set<VarId>&& group_vars,
        std::set<VarId>&& order_saved_vars
    );

    ~HybridAggregation();

    void _begin(Binding& parent_binding) override;

    void _reset() override;

    bool _next() override;

    void assign_nulls() override;

    void print(std::ostream& os, int indent, bool stats) const override;

private:
    std::unique_ptr<BindingIter> child;

    std::map<VarId, std::unique_ptr<Agg>> aggregations;

    std::map<VarId, std::unique_ptr<UAgg>> u_aggregations;

    // may be empty if when the query uses aggregates without any group
    const std::set<VarId> group_vars;

    Binding* parent_binding;

    Binding child_binding;

    Binding ordered_group_binding;

    boost::unordered_map<std::vector<ObjectId>, char*, OIDVectorHasher> groups;

    boost::unordered_map<std::vector<ObjectId>, char*, OIDVectorHasher>::iterator iter;

    std::vector<ObjectId> key_buf;

    char* buffer;

    // helper for begin/reset
    void prepare();

    OrderedGroup ordered_group;

    bool pending_ordered_group;
};
