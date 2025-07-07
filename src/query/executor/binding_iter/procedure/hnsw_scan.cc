#include "hnsw_scan.h"

#include "graph_models/common/conversions.h"
#include "storage/index/hnsw/hnsw_entry.h"

using namespace Procedure;

void HNSWScan::_begin(Binding& parent_binding)
{
    this->parent_binding = &parent_binding;

    _reset();
}

bool HNSWScan::_next()
{
    if (!valid_arguments) {
        return false;
    }

    if (hnsw_query_iterator->next()) {
        const HNSW::Entry& entry = hnsw_query_iterator->get_current();
        const HNSW::HNSWNode& hnsw_node = hnsw_index_ptr->get_node(entry.node_id);

        parent_binding->add(yield_vars[0], hnsw_node.object_oid);
        parent_binding->add(yield_vars[1], Common::Conversions::pack_float(entry.distance));
        return true;
    }

    return false;
}

void HNSWScan::_reset()
{
    eval_arguments();

    if (valid_arguments) {
        set_iter();
    }
}

void HNSWScan::assign_nulls()
{
    for (const auto& yield_var : yield_vars) {
        parent_binding->add(yield_var, ObjectId::get_null());
    }
}

void HNSWScan::print(std::ostream& os, int indent, bool stats) const
{
    if (stats) {
        print_generic_stats(os, indent);
    }
    os << std::string(indent, ' ') << "HNSWScan(";
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

void HNSWScan::eval_arguments()
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

void HNSWScan::set_iter()
{
    hnsw_query_iterator = hnsw_index_ptr->query_iterator(
        &get_query_ctx().thread_info.interruption_requested,
        std::move(query),
        num_neighbors,
        num_candidates
    );
    hnsw_query_iterator->begin();
}
