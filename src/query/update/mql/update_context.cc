#include "update_context.h"

#include "query/update/mql/update_action.h"

using namespace MQL;

void UpdateContext::process_new_property(uint64_t obj, uint64_t key, uint64_t val)
{
    // TODO: index
}

void UpdateContext::process_deleted_property(uint64_t obj, uint64_t key, uint64_t val)
{
    // TODO: index
}

void UpdateContext::update_indexes()
{
    // TODO:
}

void UpdateContext::create_text_index(CreateTextIndex& index_info)
{
    auto& index_manager = quad_model.catalog.text_index_manager;
    const auto& [inserted_elements, inserted_tokens] = index_manager.create_text_search_index(
        index_info.index_name,
        index_info.property,
        index_info.normalize_type,
        index_info.tokenize_type
    );
    // TODO: add stats
}

void UpdateContext::create_hnsw_index(CreateHNSWIndex& index_info)
{
    auto& index_manager = quad_model.catalog.hnsw_index_manager;
    auto inserted_elements = index_manager.create_hnsw_index<Catalog::ModelID::QUAD>(
        index_info.index_name,
        index_info.property,
        index_info.dimension,
        index_info.max_edges,
        index_info.max_candidates,
        index_info.metric_type
    );
    // TODO: add stats
}
