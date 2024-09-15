#pragma once

#include "query/executor/binding_iter.h"

class TensorStore;

namespace LSH {
enum class MetricType;

/* 
 * This BindingIter will read the value from object_var, get the corresponding
 * tensor from the TensorStore, and compute its similarity to the query_tensor, 
 * then write the similarity to similarity_var.
 */
class ProjectTensorSimilarity : public BindingIter {
public:
    std::unique_ptr<BindingIter> child_iter;
    const VarId                  object_var;
    const VarId                  similarity_var;
    const std::vector<float>     query_tensor;
    const TensorStore&           tensor_store;
    const MetricType             metric_type;

    ProjectTensorSimilarity(std::unique_ptr<BindingIter> child_iter,
                            VarId                        object_var,
                            VarId                        similarity_var,
                            std::vector<float>&&         query_tensor,
                            const TensorStore&           tensor_store,
                            MetricType                   metric_type);

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
};
} // namespace LSH