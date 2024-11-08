#pragma once

#include "query/executor/binding_iter.h"

class TensorStore;

namespace LSH {

class ForestIndexTopK : public BindingIter {
public:
    const VarId              object_var;
    const VarId              similarity_var;
    const std::vector<float> query_tensor;
    const int64_t            k;
    const int64_t            search_k;
    const TensorStore&       tensor_store;

    ForestIndexTopK(VarId                     object_var,
                    VarId                     similarity_var,
                    const std::vector<float>& query_tensor,
                    int64_t                   k,
                    int64_t                   search_k,
                    const TensorStore&        tensor_store);

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

private:
    uint64_t current_index;

    Binding* parent_binding;

    std::vector<std::pair<uint64_t, float>> top_k;
};
} // namespace LSH