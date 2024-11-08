#include "brute_similarity_search.h"

#include <algorithm>

#include "graph_models/common/conversions.h"
#include "storage/index/tensor_store/lsh/metric.h"
#include "storage/index/tensor_store/tensor_store.h"

using namespace LSH;

BruteSimilaritySearch::BruteSimilaritySearch(std::unique_ptr<BindingIter> child_iter,
                                             VarId                        object_var_,
                                             VarId                        similarity_var_,
                                             std::vector<float>&&         query_tensor_,
                                             const TensorStore&           tensor_store_,
                                             MetricType                   metric_type_,
                                             uint64_t                     k) :
    child_iter(std::move(child_iter)),
    object_var(object_var_),
    similarity_var(similarity_var_),
    query_tensor(std::move(query_tensor_)),
    tensor_store(tensor_store_),
    metric_type(metric_type_),
    k(k) {
    switch (metric_type) {
    case MetricType::ANGULAR: {
        similarity_fn = &Metric::cosine_distance;
        break;
    }
    case MetricType::EUCLIDEAN: {
        similarity_fn = &Metric::euclidean_distance;
        break;
    }
    default: { // MetricType::MANHATTAN
        similarity_fn = &Metric::manhattan_distance;
        break;
    }
    }
    tensor_buffer.resize(tensor_store.tensors_dim);
}


void BruteSimilaritySearch::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}

void BruteSimilaritySearch::build_binding_pq() {
    // Fill up the max priority queue with up to k items
    for (auto i = 0U; i < k; i++) {
        if (!child_iter->next()) {
            break;
        }

        const auto child_object_oid = (*parent_binding)[object_var];

        float similarity;
        if (tensor_store.get(child_object_oid.id, tensor_buffer)) {
            // Tensor found, project similarity
            similarity = similarity_fn(query_tensor, tensor_buffer);
        } else {
            // No tensor found for the current child object id, just project infinity
            similarity = std::numeric_limits<float>::infinity();
        }

        parent_binding->add(similarity_var, Common::Conversions::pack_float(similarity));

        auto binding_copy = std::make_unique<Binding>(parent_binding->size);
        binding_copy->add_all(*parent_binding);
        binding_pq.emplace_back(similarity, std::move(binding_copy));
    }

    // Heapify the priority queue by similarity (in descending order)
    std::make_heap(binding_pq.begin(), binding_pq.end());

    // Update the max priority queue with the rest of the child objects
    while (child_iter->next()) {
        const auto child_object_oid = (*parent_binding)[object_var];
        if (tensor_store.get(child_object_oid.id, tensor_buffer)) {
            // Tensor found, project similarity
            const auto similarity = similarity_fn(query_tensor, tensor_buffer);

            // Replace the current biggest similarity (the worst) if the new one is smaller (better)
            const auto max_similarity = binding_pq.front().first;
            if (similarity < max_similarity) {
                std::pop_heap(binding_pq.begin(), binding_pq.end());
                auto max_binding = std::move(binding_pq.back().second);
                binding_pq.pop_back();

                parent_binding->add(similarity_var, Common::Conversions::pack_float(similarity));
                max_binding->add_all(*parent_binding);
                // Reuse the same binding ptr
                binding_pq.emplace_back(similarity, std::move(max_binding));
                std::push_heap(binding_pq.begin(), binding_pq.end());
            }
        }
    }

    // Sort by similarity in descending order
    std::sort(binding_pq.begin(), binding_pq.end(), [](const auto& a, const auto& b) { return a.first > b.first; });
}

void BruteSimilaritySearch::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    child_iter->begin(*parent_binding);

    build_binding_pq();
}


bool BruteSimilaritySearch::_next() {
    // Yield from priority queue
    if (!binding_pq.empty()) {
        auto&& [_, current_binding] = binding_pq.back();
        parent_binding->add_all(*current_binding);
        binding_pq.pop_back();
        return true;
    }
    return false;
}


void BruteSimilaritySearch::_reset() {
    child_iter->reset();

    binding_pq.clear();
    build_binding_pq();
}


void BruteSimilaritySearch::assign_nulls() {
    child_iter->assign_nulls();
    parent_binding->add(similarity_var, ObjectId::get_null());
}