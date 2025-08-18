#include "update_executor.h"

#include <cassert>

#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/text_search/text_index.h"
#include "storage/index/text_search/text_index_manager.h"

using namespace MQL;


UpdateExecutor::~UpdateExecutor()
{
    // TODO: force string file WAL flush?
    // TODO: rollback if necessary?
}

uint64_t UpdateExecutor::execute()
{
    bool interruption = false;

    Binding binding(get_query_ctx().get_var_size());

    iter->begin(binding);

    while (iter->next()) {
        for (auto& action : update_actions) {
            action->process(binding, *update_context);
        }
    }

    // check deleted nodes have detach or don't appear in edges
    auto to_check = update_context->deleted_objects;

    while (!to_check.empty()) {
        std::set<DeleteObjectInfo> new_deleted_objects;
        for (auto& node_info : to_check) {
            auto node = node_info.obj.id;
            auto node_iter = quad_model.edge_from_to_type->get_range(&interruption, { node }, { node });

            if (node_iter.next()) {
                auto it1 = quad_model.from_to_type_edge->get_range(
                    &interruption,
                    { node, 0, 0, 0 },
                    { node, UINT64_MAX, UINT64_MAX, UINT64_MAX }
                );
                auto it2 = quad_model.to_type_from_edge->get_range(
                    &interruption,
                    { node, 0, 0, 0 },
                    { node, UINT64_MAX, UINT64_MAX, UINT64_MAX }
                );
                auto it3 = quad_model.type_from_to_edge->get_range(
                    &interruption,
                    { node, 0, 0, 0 },
                    { node, UINT64_MAX, UINT64_MAX, UINT64_MAX }
                );

                if (!node_info.detach) {
                    if (it1.next() != nullptr || it2.next() != nullptr || it3.next() != nullptr) {
                        throw QueryException(
                            "Trying to delete node with existing connections (use DETACH DELETE if intended)"
                        );
                    }
                }

                for (auto record = it1.next(); record != nullptr; record = it1.next()) {
                    ObjectId edge((*record)[3]);
                    if (update_context->deleted_objects.emplace(edge, true).second) {
                        new_deleted_objects.emplace(edge, true);
                    }
                }
                for (auto record = it2.next(); record != nullptr; record = it2.next()) {
                    ObjectId edge((*record)[3]);
                    if (update_context->deleted_objects.emplace(edge, true).second) {
                        new_deleted_objects.emplace(edge, true);
                    }
                }
                for (auto record = it3.next(); record != nullptr; record = it3.next()) {
                    ObjectId edge((*record)[3]);
                    if (update_context->deleted_objects.emplace(edge, true).second) {
                        new_deleted_objects.emplace(edge, true);
                    }
                }

                // save all node properties to delete later
                auto prop_iter = quad_model.object_key_value->get_range(
                    &interruption,
                    { node, 0, 0 },
                    { node, UINT64_MAX, UINT64_MAX }
                );

                for (auto record = prop_iter.next(); record != nullptr; record = prop_iter.next()) {
                    auto key = (*record)[1];
                    update_context->deleted_properties.emplace(ObjectId(node), ObjectId(key));
                }
            }
        }
        to_check = new_deleted_objects;
    }

    for (auto& obj_info : update_context->deleted_objects) {
        if (obj_info.obj.get_type() != ObjectId::MASK_EDGE) {
            if (quad_model.nodes->delete_record({ obj_info.obj.id })) {
                stats.deleted_nodes++;
            }
            continue;
        }

        auto edge = obj_info.obj.id;
        Record<4> min_range = { edge, 0, 0, 0 };
        Record<4> max_range = { edge, UINT64_MAX, UINT64_MAX, UINT64_MAX };
        auto edge_iter = quad_model.edge_from_to_type->get_range(&interruption, min_range, max_range);

        if (auto existing_record = edge_iter.next()) {
            auto from = (*existing_record)[1];
            auto to = (*existing_record)[2];
            auto type = (*existing_record)[3];

            quad_model.edge_from_to_type->delete_record({ edge, from, to, type });
            quad_model.from_to_type_edge->delete_record({ from, to, type, edge });
            quad_model.to_type_from_edge->delete_record({ to, type, from, edge });
            quad_model.type_from_to_edge->delete_record({ type, from, to, edge });
            quad_model.type_to_from_edge->delete_record({ type, to, from, edge });

            stats.type2total_count[type]--;
            stats.deleted_edges++;

            if (from == to) {
                quad_model.equal_from_to->delete_record({ from, type, edge });
                quad_model.equal_from_to_inverted->delete_record({ type, from, edge });
                stats.type2equal_from_to_count[type]--;

                if (from == type) {
                    quad_model.equal_from_to_type->delete_record({ from, edge });
                    stats.type2equal_from_to_type_count[type]--;
                }
            }
            if (from == type) {
                quad_model.equal_from_type->delete_record({ from, to, edge });
                quad_model.equal_from_type_inverted->delete_record({ to, from, edge });
                stats.type2equal_from_type_count[type]--;
            }
            if (to == type) {
                quad_model.equal_to_type->delete_record({ to, from, edge });
                quad_model.equal_to_type_inverted->delete_record({ from, to, edge });
                stats.type2equal_to_type_count[type]--;
            }

            // save all edge properties to delete later
            auto prop_iter = quad_model.object_key_value
                                 ->get_range(&interruption, { edge, 0, 0 }, { edge, UINT64_MAX, UINT64_MAX });
            for (auto record = prop_iter.next(); record != nullptr; record = prop_iter.next()) {
                auto key = (*record)[1];
                update_context->deleted_properties.emplace(ObjectId(edge), ObjectId(key));
            }
        }
    }

    update_indexes();

    return 0;
}

void UpdateExecutor::update_indexes()
{
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
}

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
void UpdateExecutor::process_new_property(ObjectId obj, ObjectId key, ObjectId val) { }
void UpdateExecutor::process_deleted_property(ObjectId obj, ObjectId key, ObjectId val) { }

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
