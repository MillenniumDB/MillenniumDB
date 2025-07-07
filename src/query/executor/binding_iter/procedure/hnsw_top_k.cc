#include "hnsw_top_k.h"

#include "graph_models/common/conversions.h"
#include "graph_models/common/datatypes/tensor/tensor.h"
#include "storage/index/hnsw/hnsw_entry.h"

using namespace Procedure;

void HNSWTopK::_begin(Binding& parent_binding)
{
    this->parent_binding = &parent_binding;

    _reset();
}

bool HNSWTopK::_next()
{
    if (!valid_arguments) {
        return false;
    }

    if (top_k_heap.empty()) {
        return false;
    }

    const HNSW::Entry& entry = top_k_heap.get_min();
    const HNSW::HNSWNode& hnsw_node = hnsw_index_ptr->get_node(entry.node_id);

    parent_binding->add(yield_vars[0], hnsw_node.object_oid);
    parent_binding->add(yield_vars[1], Common::Conversions::pack_float(entry.distance));

    top_k_heap.pop_min();

    return true;
}

void HNSWTopK::_reset()
{
    eval_arguments();

    if (valid_arguments) {
        eval_query();
    }
}

void HNSWTopK::assign_nulls()
{
    for (const auto& yield_var : yield_vars) {
        parent_binding->add(yield_var, ObjectId::get_null());
    }
}

void HNSWTopK::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "HNSWTopK(";
    auto binding_expr_printer = get_query_ctx().create_binding_expr_printer(os);
    if (!argument_binding_exprs.empty()) {
        argument_binding_exprs[0]->accept_visitor(*binding_expr_printer);
    }
    for (std::size_t i = 1; i < argument_binding_exprs.size(); ++i) {
        os << ", ";
        argument_binding_exprs[i]->accept_visitor(*binding_expr_printer);
    }

    os << ") -> (";

    os << yield_vars[0];
    for (std::size_t i = 1; i < yield_vars.size(); ++i) {
        os << ", ";
        os << yield_vars[i];
    }
    os << ")\n";
}

void HNSWTopK::eval_arguments()
{
    valid_arguments = false;
    const auto name_oid = argument_binding_exprs[0]->eval(*parent_binding);
    const auto name_gen_t = name_oid.id & ObjectId::GENERIC_TYPE_MASK;
    if (name_gen_t != ObjectId::MASK_STRING) {
        return;
    }
    const std::string name = unpack_string_func_ptr(name_oid);
    hnsw_index_ptr = hnsw_index_manager.get_hnsw_index(name);
    if (hnsw_index_ptr == nullptr) {
        // index does not exist
        return;
    }

    const auto query_oid = argument_binding_exprs[1]->eval(*parent_binding);
    const auto query_gen_t = query_oid.id & ObjectId::GENERIC_TYPE_MASK;
    if (query_gen_t != ObjectId::MASK_TENSOR) {
        return;
    }
    query = Common::Conversions::to_tensor<float>(query_oid);
    if (query.size() != hnsw_index_ptr->get_params().dimensions) {
        // dimension mismatch
        return;
    }

    const auto num_neighbors_oid = argument_binding_exprs[2]->eval(*parent_binding);
    const auto num_neighbors_gen_t = num_neighbors_oid.id & ObjectId::GENERIC_TYPE_MASK;
    if (num_neighbors_gen_t != ObjectId::MASK_INT) {
        return;
    }
    num_neighbors = unpack_int_func_ptr(num_neighbors_oid);

    const auto num_candidates_oid = argument_binding_exprs[3]->eval(*parent_binding);
    const auto num_candidates_gen_t = num_candidates_oid.id & ObjectId::GENERIC_TYPE_MASK;
    if (num_candidates_gen_t != ObjectId::MASK_INT) {
        return;
    }
    num_candidates = unpack_int_func_ptr(num_candidates_oid);

    valid_arguments = true;
}

void HNSWTopK::eval_query()
{
    top_k_heap = hnsw_index_ptr->query(std::move(query), num_neighbors, num_candidates, nullptr, nullptr);
}
