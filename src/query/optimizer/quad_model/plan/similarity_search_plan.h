#pragma once

#include "query/optimizer/plan/plan.h"

class SimilaritySearchPlan : public Plan {
public:
    SimilaritySearchPlan(VarId                     object_var,
                         VarId                     similarity_var,
                         const std::string&        tensor_store_name,
                         int64_t                   k,
                         int64_t                   search_k,
                         const std::vector<float>& query_tensor,
                         ObjectId                  query_object);

    SimilaritySearchPlan(const SimilaritySearchPlan& other) :
        object_var        (other.object_var),
        similarity_var    (other.similarity_var),
        tensor_store_name (other.tensor_store_name),
        k                 (other.k),
        search_k          (other.search_k),
        query_tensor      (other.query_tensor) { }

    std::unique_ptr<Plan> clone() const override {
        return std::make_unique<SimilaritySearchPlan>(*this);
    }

    int relation_size() const override { return 2; }

    double estimate_cost() const override;
    double estimate_output_size() const override;

    std::set<VarId> get_vars() const override;
    void            set_input_vars(const std::set<VarId>& input_vars) override;

    std::unique_ptr<BindingIter> get_binding_iter() const override;

    bool get_leapfrog_iter(std::vector<std::unique_ptr<LeapfrogIter>>& leapfrog_iters,
                           std::vector<VarId>&                         var_order,
                           uint_fast32_t&                              enumeration_level) const override;

    void print(std::ostream& os, int indent) const override;

    VarId get_object_var() const { return object_var; }
    VarId get_similarity_var() const { return similarity_var; }

private:
    VarId              object_var;
    VarId              similarity_var;
    std::string        tensor_store_name;
    int64_t            k;
    int64_t            search_k;
    std::vector<float> query_tensor;
};
