#include "project_tensor_similarity.h"

#include "graph_models/common/conversions.h"
#include "storage/index/tensor_store/lsh/metric.h"
#include "storage/index/tensor_store/tensor_store.h"

using namespace LSH;

ProjectTensorSimilarity::ProjectTensorSimilarity(std::unique_ptr<BindingIter> child_iter,
                                                 VarId                        object_var_,
                                                 VarId                        similarity_var_,
                                                 std::vector<float>&&         query_tensor_,
                                                 const TensorStore&           tensor_store_,
                                                 MetricType                   metric_type_) :
    child_iter     (std::move(child_iter)),
    object_var     (object_var_),
    similarity_var (similarity_var_),
    query_tensor   (std::move(query_tensor_)),
    tensor_store   (tensor_store_),
    metric_type    (metric_type_) {
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


void ProjectTensorSimilarity::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


void ProjectTensorSimilarity::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    child_iter->begin(*parent_binding);
}


bool ProjectTensorSimilarity::_next() {
    if (child_iter->next()) {
        const auto child_object_oid = (*parent_binding)[object_var];
        if (tensor_store.get(child_object_oid.id, tensor_buffer)) {
            // Tensor found, project similarity
            const auto similarity = similarity_fn(query_tensor, tensor_buffer);
            parent_binding->add(similarity_var, Common::Conversions::pack_float(similarity));
        } else {
            // No tensor found for the current child object id, just project infinity
            parent_binding->add(similarity_var, Common::Conversions::pack_float(std::numeric_limits<float>::infinity()));
        }
        return true;
    }
    return false;
}


void ProjectTensorSimilarity::_reset() {
    child_iter->reset();
}


void ProjectTensorSimilarity::assign_nulls() {
    child_iter->assign_nulls();
    parent_binding->add(similarity_var, ObjectId::get_null());
}