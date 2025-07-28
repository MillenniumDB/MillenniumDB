#pragma once

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/var_id.h"
#include "storage/index/hnsw/hnsw_index.h"
#include "storage/index/hnsw/hnsw_index_manager.h"

namespace Procedure {

// Gives the top k ANN-objects
class HNSWTopK : public BindingIter {
public:
    using UnpackStringFuncType = std::string(ObjectId);
    using UnpackIntFuncType = int64_t(ObjectId);

    HNSWTopK(
        std::vector<std::unique_ptr<BindingExpr>>&& argument_binding_exprs_,
        std::vector<VarId>&& yield_vars_,
        HNSW::HNSWIndexManager& hnsw_index_manager_,
        UnpackStringFuncType* unpack_string_func_ptr_,
        UnpackIntFuncType* unpack_int_func_ptr_
    ) :
        argument_binding_exprs { std::move(argument_binding_exprs_) },
        yield_vars { std::move(yield_vars_) },
        hnsw_index_manager { hnsw_index_manager_ },
        unpack_string_func_ptr { unpack_string_func_ptr_ },
        unpack_int_func_ptr { unpack_int_func_ptr_ }
    {
        assert(argument_binding_exprs.size() == 4);
        assert(yield_vars.size() == 2);
    }

    void print(std::ostream& os, int indent, bool stats) const override;

    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

private:
    const std::vector<std::unique_ptr<BindingExpr>> argument_binding_exprs;
    const std::vector<VarId> yield_vars;
    HNSW::HNSWIndexManager& hnsw_index_manager;
    UnpackStringFuncType* unpack_string_func_ptr;
    UnpackIntFuncType* unpack_int_func_ptr;

    // evaluated arguments
    HNSW::HNSWIndex* hnsw_index_ptr;
    tensor::Tensor<float> query;
    uint64_t num_neighbors;
    uint64_t num_candidates;

    // wheter the argument evaluation is valid for this procedure
    bool valid_arguments;

    HNSW::HNSWHeap top_k_heap;

    Binding* parent_binding;

    // sets valid_arguments to true if the argument evaluation is valid for
    // this procedure, false otherwise
    void eval_arguments();

    // sets top_k_heap
    void eval_query();
};
} // namespace Procedure
