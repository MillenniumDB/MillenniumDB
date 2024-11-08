#pragma once

#include "query/executor/binding.h"
#include "query/executor/binding_iter.h"

class TensorStore;

namespace LSH {
enum class MetricType;

class BruteSimilaritySearch : public BindingIter {
public:
    std::unique_ptr<BindingIter> child_iter;
    const VarId                  object_var;
    const VarId                  similarity_var;
    const std::vector<float>     query_tensor;
    const TensorStore&           tensor_store;
    const MetricType             metric_type;
    const uint64_t               k;

    BruteSimilaritySearch(std::unique_ptr<BindingIter> child_iter,
                          VarId                        object_var,
                          VarId                        similarity_var,
                          std::vector<float>&&         query_tensor,
                          const TensorStore&           tensor_store,
                          MetricType                   metric_type,
                          uint64_t                     k);

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

private:
    Binding* parent_binding;

    float (*similarity_fn)(const std::vector<float>&, const std::vector<float>&);

    // Used to prevent multiple allocations
    std::vector<float> tensor_buffer;

    void build_binding_pq();

    std::vector<std::pair<float, std::unique_ptr<Binding>>> binding_pq;
};
} // namespace LSH