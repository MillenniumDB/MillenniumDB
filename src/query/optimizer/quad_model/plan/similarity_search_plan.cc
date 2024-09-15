#include "similarity_search_plan.h"

#include <cassert>
#include <iostream>

#include "query/exceptions.h"
#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "storage/index/leapfrog/leapfrog_similarity_search_iter.h"
#include "storage/index/tensor_store/lsh/binding_iters/forest_index_top_k.h"
#include "storage/index/tensor_store/lsh/binding_iters/forest_index_top_all.h"
#include "storage/index/tensor_store/lsh/forest_index_query_iter.h"

SimilaritySearchPlan::SimilaritySearchPlan(VarId                     object_var_,
                                           VarId                     similarity_var_,
                                           const std::string&        tensor_store_name_,
                                           int64_t                   k_,
                                           int64_t                   search_k_,
                                           const std::vector<float>& query_tensor_,
                                           ObjectId                  query_object) :
    object_var(object_var_),
    similarity_var(similarity_var_),
    tensor_store_name(tensor_store_name_),
    k(k_),
    search_k(search_k_),
    query_tensor(query_tensor_) {
    assert((query_tensor.empty() ^ (query_object.id == ObjectId::NULL_ID))
           && "Exactly one of the following must be non-null: query_tensor, query_object");

    if (!quad_model.catalog().name2tensor_store.contains(tensor_store_name))
        throw QueryException("Tensor store \"" + tensor_store_name + "\" does not exist");

    TensorStore& tensor_store = *quad_model.catalog().name2tensor_store.at(tensor_store_name);

    if (query_tensor.empty()) {
        // It is necessary to read from the tensor store
        query_tensor.resize(tensor_store.tensors_dim);
        bool found = tensor_store.get(query_object.id, query_tensor);
        if (!found)
            throw QueryException("ObjectId for " + MQL::Conversions::to_lexical_str(query_object) + " not found in tensor store");
    }

    if (tensor_store.tensors_dim != query_tensor.size()) {
        throw QueryException("Input tensor must have dimension " + std::to_string(tensor_store.tensors_dim));
    }

    assert(!query_tensor.empty() && "query_tensor is empty");
}


void SimilaritySearchPlan::print(std::ostream& os, int indent) const {
    for (int i = 0; i < indent; ++i) {
        os << ' ';
    }
    os << "SimilaritySearchPlan(";
    os << "object_var: " << object_var;
    os << ", similarity_var: " << similarity_var;
    os << ", tensor_store_name: " << tensor_store_name;
    os << ", k: " << k;
    os << ", search_k: " << search_k;
    os << ", query_tensor.size(): " << query_tensor.size();
    for (int i = 0; i < indent; ++i)
        os << ' ';
    os << "  ↳ Estimated factor: " << estimate_output_size();
}


double SimilaritySearchPlan::estimate_cost() const {
    return /*100.0 +*/ estimate_output_size();
}


double SimilaritySearchPlan::estimate_output_size() const {
    auto store_size = quad_model.catalog().name2tensor_store.at(tensor_store_name)->size();
    return ((k > 0) && (uint64_t(k) < store_size)) ? k : store_size;
}


void SimilaritySearchPlan::set_input_vars(const std::set<VarId>&) {
    // no need to do anything
}


std::set<VarId> SimilaritySearchPlan::get_vars() const {
    return { object_var, similarity_var };
}


std::unique_ptr<BindingIter> SimilaritySearchPlan::get_binding_iter() const {
    if (k < 0) {
        auto query_iter = quad_model.catalog().name2tensor_store.at(tensor_store_name)->query_iter(query_tensor);
        return std::make_unique<LSH::ForestIndexTopAll>(object_var, similarity_var, std::move(query_iter));
    } else {
        const auto& tensor_store = *quad_model.catalog().name2tensor_store.at(tensor_store_name);
        return std::make_unique<LSH::ForestIndexTopK>(object_var, similarity_var, query_tensor, k, search_k, tensor_store);
    }
}


bool SimilaritySearchPlan::get_leapfrog_iter(std::vector<std::unique_ptr<LeapfrogIter>>& leapfrog_iters,
                                             std::vector<VarId>&                         /*var_order*/,
                                             uint_fast32_t&                              /*enumeration_level*/) const {
    // Cannot get leapfrog iter with unbounded k
    if (k < 0)
        return false;

    std::vector<std::unique_ptr<ScanRange>> initial_ranges = { };
    std::vector<VarId> intersection_vars = { object_var };
    std::vector<VarId> enumeration_vars  = { similarity_var };

    const auto& tensor_store = *quad_model.catalog().name2tensor_store.at(tensor_store_name);

    leapfrog_iters.push_back(std::make_unique<LeapfrogSimilaritySearchIter>(
        &get_query_ctx().thread_info.interruption_requested,
        std::move(initial_ranges),
        std::move(intersection_vars),
        std::move(enumeration_vars),
        query_tensor,
        k,
        search_k,
        tensor_store
    ));

    return true;
}