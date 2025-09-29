#include "update_context.h"

#include "graph_models/quad_model/conversions.h"
#include "query/update/mql/update_action.h"

using namespace MQL;

UpdateContext::UpdateContext()
{
    current_anon = quad_model.catalog.max_anon;
    current_edge = quad_model.catalog.max_edge;

    for (auto&& [predicate, names] : quad_model.catalog.text_index_manager.get_predicate2names()) {
        auto predicate_id = Conversions::pack_string(predicate).id;
        for (auto& name : names) {
            indexed_keys[predicate_id].push_back(name);
        }
    }

    for (auto&& [predicate, names] : quad_model.catalog.hnsw_index_manager.get_predicate2names()) {
        auto predicate_id = Conversions::pack_string(predicate).id;
        for (auto& name : names) {
            indexed_keys[predicate_id].push_back(name);
        }
    }
}

void UpdateContext::process_new_property(uint64_t obj, uint64_t key, uint64_t val)
{
    auto keys = indexed_keys.find(key);
    if (keys == indexed_keys.end()) {
        return;
    }

    for (auto& index_name : keys->second) {
        if (auto text_index = quad_model.catalog.text_index_manager.get_text_index(index_name)) {
            auto inserted_tokens = text_index->index_single(ObjectId(obj), ObjectId(val));
            if (inserted_tokens > 0) {
                text_index_inserts += inserted_tokens;
            }
        }

        if (auto hnsw_index = quad_model.catalog.hnsw_index_manager.get_hnsw_index(index_name)) {
            auto inserted_elements = hnsw_index->index_single<true>(ObjectId(obj), ObjectId(val));
            if (inserted_elements > 0) {
                hnsw_index_inserts += inserted_elements;
            }
        }
    }
}

void UpdateContext::process_deleted_property(uint64_t obj, uint64_t key, uint64_t val)
{
    auto keys = indexed_keys.find(key);
    if (keys == indexed_keys.end()) {
        return;
    }

    for (auto& index_name : keys->second) {
        if (auto text_index = quad_model.catalog.text_index_manager.get_text_index(index_name)) {
            auto deleted_tokens = text_index->remove_single(ObjectId(obj), ObjectId(val));
            if (deleted_tokens > 0) {
                text_index_deletes += deleted_tokens;
            }
        }

        if (auto hnsw_index = quad_model.catalog.hnsw_index_manager.get_hnsw_index(index_name)) {
            auto deleted_elements = hnsw_index->remove_single(ObjectId(obj), ObjectId(val));
            if (deleted_elements > 0) {
                hnsw_index_deletes += deleted_elements;
            }
        }
    }
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
    hnsw_index_inserts += inserted_elements;
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
    text_index_inserts += inserted_elements;
}
