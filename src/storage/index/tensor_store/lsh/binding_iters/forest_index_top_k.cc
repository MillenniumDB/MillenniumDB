#include "forest_index_top_k.h"

#include <vector>

#include "graph_models/common/conversions.h"
#include "storage/index/tensor_store/tensor_store.h"
#include "macros/likely.h"

using namespace LSH;

ForestIndexTopK::ForestIndexTopK(VarId                     object_var_,
                                 VarId                     similarity_var_,
                                 const std::vector<float>& query_tensor_,
                                 int64_t                   k_,
                                 int64_t                   search_k_,
                                 const TensorStore&        tensor_store_) :
    object_var     (object_var_),
    similarity_var (similarity_var_),
    query_tensor   (query_tensor_),
    k              (k_),
    search_k       (search_k_),
    tensor_store   (tensor_store_) { }


void ForestIndexTopK::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


void ForestIndexTopK::_begin(Binding& parent_binding) {
    this->parent_binding = &parent_binding;
    current_index        = 0;
    top_k                = tensor_store.query_top_k(query_tensor, k, search_k);
}


bool ForestIndexTopK::_next() {
    if (MDB_unlikely(get_query_ctx().thread_info.interruption_requested))
        throw InterruptedException();

    if (current_index < top_k.size()) {
        parent_binding->add(object_var, ObjectId(top_k[current_index].first));
        parent_binding->add(similarity_var, Common::Conversions::pack_float(top_k[current_index].second));
        ++current_index;
        return true;
    }
    return false;
}


void ForestIndexTopK::_reset() {
    current_index = 0;
}


void ForestIndexTopK::assign_nulls() {
    parent_binding->add(object_var, ObjectId::get_null());
    parent_binding->add(similarity_var, ObjectId::get_null());
}