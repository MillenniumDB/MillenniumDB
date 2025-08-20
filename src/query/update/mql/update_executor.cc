#include "update_executor.h"

#include <cassert>

// #include "graph_models/quad_model/conversions.h"
// #include "graph_models/quad_model/quad_model.h"
// #include "storage/index/bplus_tree/bplus_tree.h"
// #include "storage/index/text_search/text_index.h"
// #include "storage/index/text_search/text_index_manager.h"

using namespace MQL;

UpdateExecutor::~UpdateExecutor()
{
    // TODO: force string file WAL flush?
    // TODO: rollback if necessary?
}

uint64_t UpdateExecutor::execute()
{
    Binding binding(get_query_ctx().get_var_size());

    iter->begin(binding);

    while (iter->next()) {
        for (auto& action : update_actions) {
            action->process(binding, *update_context);
        }
    }

    update_context->update_indexes();

    // TODO: mark commit?

    return 0;
}

// void UpdateExecutor::update_indexes()
// {
    // using Name2InsertsMap = boost::unordered_map<std::string, std::vector<std::tuple<ObjectId, ObjectId>>>;

    // const auto& text_index_predicate2names = quad_model.catalog.text_index_manager.get_predicate2names();
    // Name2InsertsMap text_index_name2inserts;
    // Name2InsertsMap text_index_name2deletes;

    // const auto& hnsw_index_predicate2names = quad_model.catalog.hnsw_index_manager.get_predicate2names();
    // Name2InsertsMap hnsw_index_name2inserts;
    // Name2InsertsMap hnsw_index_name2deletes;

    // // Execute index updates, starting with deletes
    // // DELETES
    // for (const auto& [name, deletes] : text_index_name2deletes) {
    //     auto* text_index_ptr = quad_model.catalog.text_index_manager.get_text_index(name);
    //     assert(text_index_ptr != nullptr && "Text index not found");

    //     uint_fast32_t removed_elements = 0;
    //     uint_fast32_t removed_tokens = 0;
    //     for (const auto& [node, value] : deletes) {
    //         const auto current_deleted_tokens = text_index_ptr->remove_single(node, value);
    //         if (current_deleted_tokens > 0) {
    //             ++removed_elements;
    //             removed_tokens += current_deleted_tokens;
    //         }
    //     }

    //     TextIndexUpdateStats text_index_update_data;
    //     text_index_update_data.index_name = name;
    //     text_index_update_data.removed_elements = removed_elements;
    //     text_index_update_data.removed_tokens = removed_tokens;
    //     insert_text_index_update_data(std::move(text_index_update_data));
    // }

    // for (const auto& [name, deletes] : hnsw_index_name2deletes) {
    //     auto* hnsw_index_ptr = quad_model.catalog.hnsw_index_manager.get_hnsw_index(name);
    //     assert(hnsw_index_ptr != nullptr && "Text index not found");

    //     uint_fast32_t removed_elements = 0;
    //     for (const auto& [node, value] : deletes) {
    //         if (hnsw_index_ptr->remove_single(node, value)) {
    //             ++removed_elements;
    //         }
    //     }

    //     HNSWIndexUpdateStats hnsw_index_update_data;
    //     hnsw_index_update_data.index_name = name;
    //     hnsw_index_update_data.removed_elements = removed_elements;
    //     insert_hnsw_index_update_data(std::move(hnsw_index_update_data));
    // }

    // // INSERTS
    // for (const auto& [name, inserts] : text_index_name2inserts) {
    //     auto* text_index_ptr = quad_model.catalog.text_index_manager.get_text_index(name);
    //     assert(text_index_ptr != nullptr && "Text index not found");

    //     uint_fast32_t inserted_elements = 0;
    //     uint_fast32_t inserted_tokens = 0;
    //     for (const auto& [node, value] : inserts) {
    //         const auto current_inserted_tokens = text_index_ptr->index_single(node, value);
    //         if (current_inserted_tokens > 0) {
    //             ++inserted_elements;
    //             inserted_tokens += current_inserted_tokens;
    //         }
    //     }

    //     TextIndexUpdateStats text_index_update_data;
    //     text_index_update_data.index_name = name;
    //     text_index_update_data.inserted_elements = inserted_elements;
    //     text_index_update_data.inserted_tokens = inserted_tokens;
    //     insert_text_index_update_data(std::move(text_index_update_data));
    // }

    // for (const auto& [name, inserts] : hnsw_index_name2inserts) {
    //     auto* hnsw_index_ptr = quad_model.catalog.hnsw_index_manager.get_hnsw_index(name);
    //     assert(hnsw_index_ptr != nullptr && "Text index not found");

    //     uint_fast32_t inserted_elements = 0;
    //     uint_fast32_t inserted_tokens = 0;
    //     for (const auto& [node, value] : inserts) {
    //         if (hnsw_index_ptr->index_single<true>(node, value)) {
    //             ++inserted_elements;
    //         }
    //     }

    //     TextIndexUpdateStats text_index_update_data;
    //     text_index_update_data.index_name = name;
    //     text_index_update_data.inserted_elements = inserted_elements;
    //     text_index_update_data.inserted_tokens = inserted_tokens;
    //     insert_text_index_update_data(std::move(text_index_update_data));
    // }
// }

// void UpdateExecutor::insert_text_index_update_data(TextIndexUpdateStats&& data)
// {
//     auto it = name2text_index_update_data.find(data.index_name);
//     if (it == name2text_index_update_data.end()) {
//         name2text_index_update_data.emplace(data.index_name, std::move(data));
//     } else {
//         it->second.inserted_elements += data.inserted_elements;
//         it->second.inserted_tokens += data.inserted_tokens;
//         it->second.removed_elements += data.removed_elements;
//         it->second.removed_tokens += data.removed_tokens;
//     }
// }

// void UpdateExecutor::insert_hnsw_index_update_data(HNSWIndexUpdateStats&& data)
// {
//     auto it = name2hnsw_index_update_data.find(data.index_name);
//     if (it == name2hnsw_index_update_data.end()) {
//         name2hnsw_index_update_data.emplace(data.index_name, std::move(data));
//     } else {
//         it->second.inserted_elements += data.inserted_elements;
//         it->second.removed_elements += data.removed_elements;
//     }
// }

void UpdateExecutor::analyze(std::ostream& os, bool print_stats, int indent) const
{
    //     bool has_changes = false;

    //     if (!stats.empty()) {
    //         os << "Graph updates:\n";
    //         os << "  " << stats << '\n';
    //         has_changes = true;
    //     }

    //     if (!name2text_index_update_data.empty()) {
    //         os << "Text Index updates:\n";
    //         for (const auto& [_, text_index_update] : name2text_index_update_data) {
    //             os << "  " << text_index_update << '\n';
    //         }
    //         has_changes = true;
    //     }

    //     if (!name2hnsw_index_update_data.empty()) {
    //         os << "HNSW Index updates:\n";
    //         for (const auto& [_, hnsw_index_update] : name2hnsw_index_update_data) {
    //             os << "  " << hnsw_index_update << '\n';
    //         }
    //         has_changes = true;
    //     }

    //     if (!has_changes) {
    //         os << "No modifications were performed\n";
    //     }
}
