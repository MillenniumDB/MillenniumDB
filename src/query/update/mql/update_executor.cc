#include "update_executor.h"

#include <cassert>

#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/text_search/text_index.h"
#include "storage/index/text_search/text_index_manager.h"

using namespace MQL;

constexpr uint64_t CLEAR_TMP_MASK = ~(ObjectId::MOD_MASK | ObjectId::MASK_EXTERNAL_ID);

UpdateExecutor::~UpdateExecutor()
{
    // TODO: force string file WAL flush?
    // TODO: rollback if necessary?
}

ObjectId UpdateExecutor::transform_if_tmp(ObjectId oid)
{
    if (oid.is_tmp()) {
        const uint64_t tmp_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        const auto& tmp_str = tmp_manager.get_str(tmp_id);

        const uint64_t gen_t = oid.id & ObjectId::GENERIC_TYPE_MASK;

        uint64_t new_external_id;
        if (gen_t == ObjectId::MASK_TENSOR) {
            new_external_id = tensor_manager.get_or_create_id(tmp_str.data(), tmp_str.size());
        } else {
            new_external_id = string_manager.get_or_create(tmp_str.data(), tmp_str.size());
        }

        oid.id = (oid.id & CLEAR_TMP_MASK) | ObjectId::MOD_EXTERNAL | new_external_id;
    }

    assert(!oid.is_tmp());

    return oid;
}

void UpdateExecutor::execute(UpdateContext& update_context)
{
    bool interruption = false;

    // try to throw exceptions before modifying anything
    for (auto& node_info : update_context.deleted_nodes) {
        auto node = node_info.node.id;
        auto node_iter = quad_model.edge_from_to_type->get_range(&interruption, { node }, { node });

        if (auto existing_record = node_iter.next()) {
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
                auto edge = (*record)[3];
                update_context.deleted_edges.emplace(edge);
            }
            for (auto record = it2.next(); record != nullptr; record = it2.next()) {
                auto edge = (*record)[3];
                update_context.deleted_edges.emplace(edge);
            }
            for (auto record = it3.next(); record != nullptr; record = it3.next()) {
                auto edge = (*record)[3];
                update_context.deleted_edges.emplace(edge);
            }

            // save all node properties to delete later
            auto prop_iter = quad_model.object_key_value
                                 ->get_range(&interruption, { node, 0, 0 }, { node, UINT64_MAX, UINT64_MAX });

            for (auto record = prop_iter.next(); record != nullptr; record = prop_iter.next()) {
                auto key = (*record)[1];
                update_context.deleted_properties.emplace(ObjectId(node), ObjectId(key));
            }
        }
    }

    for (auto& node_info : update_context.new_nodes) {
        auto node = transform_if_tmp(node_info);
        bool is_new_node = quad_model.nodes->insert({ node.id });
        if (is_new_node) {
            // TODO: update stats at the end? save for later
            // ++quad_model.catalog.nodes_count;
            ++graph_update_data.new_nodes;
        }
    }

    for (auto& label_info : update_context.new_labels) {
        auto label = transform_if_tmp(label_info.label);
        auto node = transform_if_tmp(label_info.node);

        bool is_new_label = quad_model.label_node->insert({ label.id, node.id });
        if (is_new_label) {
            quad_model.node_label->insert({ node.id, label.id });
            // TODO: update stats at the end? save for later
            // quad_model.catalog.insert_label(label.id);
            ++graph_update_data.new_labels;
        }
    }

    for (auto& edge_info : update_context.new_edges) {
        // assuming op_label.node is NamedNode
        auto from = transform_if_tmp(edge_info.from);
        auto to = transform_if_tmp(edge_info.to);
        auto type = transform_if_tmp(edge_info.type);
        auto edge = transform_if_tmp(edge_info.edge);

        // edge is always new
        ++graph_update_data.new_edges;

        quad_model.from_to_type_edge->insert({ from.id, to.id, type.id, edge.id });
        quad_model.to_type_from_edge->insert({ to.id, type.id, from.id, edge.id });
        quad_model.type_from_to_edge->insert({ type.id, from.id, to.id, edge.id });
        quad_model.type_to_from_edge->insert({ type.id, to.id, from.id, edge.id });
        quad_model.edge_from_to_type->insert({ edge.id, from.id, to.id, type.id });

        if (from == to) {
            quad_model.equal_from_to->insert({ from.id, type.id, edge.id });
            quad_model.equal_from_to_inverted->insert({ type.id, from.id, edge.id });

            if (from == type) {
                quad_model.equal_from_to_type->insert({ from.id, edge.id });
            }
        }
        if (from == type) {
            quad_model.equal_from_type->insert({ from.id, to.id, edge.id });
            quad_model.equal_from_type_inverted->insert({ to.id, from.id, edge.id });
        }
        if (to == type) {
            quad_model.equal_to_type->insert({ to.id, from.id, edge.id });
            quad_model.equal_to_type_inverted->insert({ from.id, to.id, edge.id });
        }
    }

    for (auto& property_info : update_context.new_properties) {
        auto obj = transform_if_tmp(property_info.obj);
        auto key = transform_if_tmp(property_info.key);
        auto val = transform_if_tmp(property_info.val);

        // Check if the node has a property with the same key
        Record<3> min_range = { obj.id, key.id, 0 };
        Record<3> max_range = { obj.id, key.id, UINT64_MAX };
        auto prop_iter = quad_model.object_key_value->get_range(&interruption, min_range, max_range);
        const auto existing_record = prop_iter.next();

        if (existing_record != nullptr) {
            ObjectId old_obj((*existing_record)[0]);
            ObjectId old_key((*existing_record)[1]);
            ObjectId old_val((*existing_record)[2]);

            // The node has a property with the same key
            if (val == old_val) {
                // The exact same record, nothing to do
                continue;
            }

            // Overwrite the old value
            quad_model.object_key_value->delete_record(*existing_record);
            quad_model.key_value_object->delete_record(*existing_record);
            quad_model.object_key_value->insert({ obj.id, key.id, val.id });
            quad_model.key_value_object->insert({ key.id, val.id, obj.id });

            ++graph_update_data.overwritten_properties;

            process_deleted_property(old_obj, old_key, old_val);
            process_new_property(obj, key, val);
        } else {
            // The node does not have a property with the same key, create a new one
            quad_model.object_key_value->insert({ obj.id, key.id, val.id });
            quad_model.key_value_object->insert({ key.id, val.id, obj.id });

            process_new_property(obj, key, val);
            // quad_model.catalog.insert_property(key.id); // TODO:

            ++graph_update_data.new_properties;
        }
    }

    for (auto& label_info : update_context.deleted_labels) {
        auto node = label_info.node;
        auto label = label_info.label;

        Record<2> min_range = { node.id, label.id };
        Record<2> max_range = { node.id, label.id };
        auto label_iter = quad_model.node_label->get_range(&interruption, min_range, max_range);

        if (label_iter.next()) {
            quad_model.node_label->delete_record({ node.id, label.id });
            quad_model.label_node->delete_record({ label.id, node.id });
        }
    }

    for (auto& edge_oid : update_context.deleted_edges) {
        auto edge = edge_oid.id;
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

            if (from == to) {
                quad_model.equal_from_to->delete_record({ from, type, edge });
                quad_model.equal_from_to_inverted->delete_record({ type, from, edge });

                if (from == type) {
                    quad_model.equal_from_to_type->delete_record({ from, edge });
                }
            }
            if (from == type) {
                quad_model.equal_from_type->delete_record({ from, to, edge });
                quad_model.equal_from_type_inverted->delete_record({ to, from, edge });
            }
            if (to == type) {
                quad_model.equal_to_type->delete_record({ to, from, edge });
                quad_model.equal_to_type_inverted->delete_record({ from, to, edge });
            }

            // save all edge properties to delete later
            auto prop_iter = quad_model.object_key_value
                                 ->get_range(&interruption, { edge, 0, 0 }, { edge, UINT64_MAX, UINT64_MAX });
            for (auto record = prop_iter.next(); record != nullptr; record = prop_iter.next()) {
                auto key = (*record)[1];
                update_context.deleted_properties.emplace(ObjectId(edge), ObjectId(key));
            }
        }
    }

    for (auto& node_info : update_context.deleted_nodes) {
        quad_model.nodes->delete_record({ node_info.node.id });
    }

    for (auto& property_info : update_context.deleted_properties) {
        auto obj = transform_if_tmp(property_info.obj);
        auto key = transform_if_tmp(property_info.key);

        Record<3> min_range = { obj.id, key.id, 0 };
        Record<3> max_range = { obj.id, key.id, UINT64_MAX };
        auto prop_iter = quad_model.object_key_value->get_range(&interruption, min_range, max_range);

        if (auto existing_record = prop_iter.next()) {
            auto value = (*existing_record)[2];
            quad_model.object_key_value->delete_record({ obj.id, key.id, value });
            quad_model.key_value_object->delete_record({ key.id, value, obj.id });
        }
    }

    update_indexes();
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

void UpdateExecutor::print_stats(std::ostream& os)
{
    //     bool has_changes = false;

    //     if (!graph_update_data.empty()) {
    //         os << "Graph updates:\n";
    //         os << "  " << graph_update_data << '\n';
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