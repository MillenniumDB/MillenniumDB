#include "update_executor.h"

using namespace MQL;

uint64_t UpdateExecutor::execute()
{
    Binding binding(get_query_ctx().get_var_size());

    iter->begin(binding);

    while (iter->next()) {
        for (auto& action : update_actions) {
            action->process(binding, *update_context);
        }
    }

    auto& catalog = quad_model.catalog;
    auto& ctx = *update_context;

    int64_t total_labels = 0;
    int64_t total_properties = 0;
    int64_t total_equal_from_to_count = 0;
    int64_t total_equal_from_type_count = 0;
    int64_t total_equal_to_type_count = 0;
    int64_t total_equal_from_to_type_count = 0;

    for (auto&& [label, count] : ctx.label2total_count) {
        total_labels += count;
        catalog.label2total_count[label] += count;
    }

    for (auto&& [key, count] : ctx.key2total_count) {
        total_properties += count;
        catalog.key2total_count[key] += count;
    }

    for (auto&& [edge_type, count] : ctx.type2total_count) {
        catalog.type2total_count[edge_type] += count;
    }

    for (auto&& [edge_type, count] : ctx.type2equal_from_to_count) {
        total_equal_from_to_count += count;
        catalog.type2equal_from_to_count[edge_type] += count;
    }
    for (auto&& [edge_type, count] : ctx.type2equal_from_type_count) {
        total_equal_from_type_count += count;
        catalog.type2equal_from_type_count[edge_type] += count;
    }
    for (auto&& [edge_type, count] : ctx.type2equal_to_type_count) {
        total_equal_to_type_count += count;
        catalog.type2equal_to_type_count[edge_type] += count;
    }
    for (auto&& [edge_type, count] : ctx.type2equal_from_to_type_count) {
        total_equal_from_to_type_count += count;
        catalog.type2equal_from_to_type_count[edge_type] += count;
    }

    catalog.max_anon = ctx.current_anon;
    catalog.max_edge = ctx.current_edge;

    catalog.deleted_edges += update_context->deleted_edges;

    assert(total_labels == ctx.new_labels - ctx.deleted_labels);
    assert(total_properties == ctx.new_properties - ctx.deleted_properties);

    catalog.nodes_count += ctx.new_nodes - ctx.deleted_nodes;
    catalog.label_count += total_labels;
    catalog.properties_count += total_properties;

    catalog.equal_from_to_count += total_equal_from_to_count;
    catalog.equal_from_type_count += total_equal_from_type_count;
    catalog.equal_to_type_count += total_equal_to_type_count;
    catalog.equal_from_to_type_count += total_equal_from_to_type_count;

    return 0;
}

void UpdateExecutor::analyze(std::ostream& os, bool print_stats, int) const
{
    if (!print_stats) {
        return;
    }

    char s[2] = "\0";
    os << "{";

    if (update_context->new_nodes) {
        os << s << "\"new_nodes\": " << update_context->new_nodes;
        s[0] = ',';
    }
    if (update_context->new_edges) {
        os << s << "\"new_edges\": " << update_context->new_edges;
        s[0] = ',';
    }
    if (update_context->new_labels) {
        os << s << "\"new_labels\": " << update_context->new_labels;
        s[0] = ',';
    }
    if (update_context->new_properties) {
        os << s << "\"new_properties\": " << update_context->new_properties;
        s[0] = ',';
    }
    if (update_context->deleted_nodes) {
        os << s << "\"deleted_nodes\": " << update_context->deleted_nodes;
        s[0] = ',';
    }
    if (update_context->deleted_edges) {
        os << s << "\"deleted_edges\": " << update_context->deleted_edges;
        s[0] = ',';
    }
    if (update_context->deleted_labels) {
        os << s << "\"deleted_labels\": " << update_context->deleted_labels;
        s[0] = ',';
    }
    if (update_context->deleted_properties) {
        os << s << "\"deleted_properties\": " << update_context->deleted_properties;
        s[0] = ',';
    }
    if (update_context->overwritten_properties) {
        os << s << "\"overwritten_properties\": " << update_context->overwritten_properties;
        s[0] = ',';
    }
    os << "}";
}
