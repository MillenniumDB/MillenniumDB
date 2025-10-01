#pragma once

#include <map>

#include "graph_models/object_id.h"
#include "graph_models/quad_model/quad_model.h"
#include "query/exceptions.h"

namespace MQL {

class CreateHNSWIndex;
class CreateTextIndex;

class UpdateContext {
public:
    int64_t new_nodes = 0;
    int64_t new_edges = 0;
    int64_t new_labels = 0;
    int64_t new_properties = 0;
    int64_t deleted_nodes = 0;
    int64_t deleted_edges = 0;
    int64_t deleted_labels = 0;
    int64_t deleted_properties = 0;
    int64_t overwritten_properties = 0;
    int64_t overwritten_edges = 0;

    int64_t hnsw_index_inserts = 0;
    int64_t hnsw_index_deletes = 0;
    int64_t text_index_inserts = 0;
    int64_t text_index_deletes = 0;

    // IMPORTANT: stats may be negative, use int64_t
    boost::unordered_flat_map<uint64_t, int64_t> label2total_count;
    boost::unordered_flat_map<uint64_t, int64_t> key2total_count;
    boost::unordered_flat_map<uint64_t, int64_t> type2total_count;

    boost::unordered_flat_map<uint64_t, int64_t> type2equal_from_to_type_count;
    boost::unordered_flat_map<uint64_t, int64_t> type2equal_from_to_count;
    boost::unordered_flat_map<uint64_t, int64_t> type2equal_from_type_count;
    boost::unordered_flat_map<uint64_t, int64_t> type2equal_to_type_count;

    std::map<uint64_t, std::vector<std::string>> indexed_keys;

    uint64_t current_anon;

    uint64_t current_edge;

    UpdateContext();

    void process_new_property(uint64_t obj, uint64_t key, uint64_t val);

    void process_deleted_property(uint64_t obj, uint64_t key, uint64_t val);

    void create_hnsw_index(CreateHNSWIndex&);

    void create_text_index(CreateTextIndex&);

    void insert_node(uint64_t node)
    {
        if (quad_model.nodes->insert({ node })) {
            new_nodes++;
        }
    }

    void insert_label(uint64_t node, uint64_t label)
    {
        if (quad_model.label_node->insert({ label, node })) {
            quad_model.node_label->insert({ node, label });

            new_labels++;
            label2total_count[label]++;
        }
    }

    void insert_property(uint64_t obj, uint64_t key, uint64_t val)
    {
        bool interruption = false;

        // Check if the node has a property with the same key
        Record<3> min_range = { obj, key, 0 };
        Record<3> max_range = { obj, key, UINT64_MAX };
        auto prop_iter = quad_model.object_key_value->get_range(&interruption, min_range, max_range);
        const auto existing_record = prop_iter.next();

        if (existing_record != nullptr) {
            auto old_obj = (*existing_record)[0];
            auto old_key = (*existing_record)[1];
            auto old_val = (*existing_record)[2];

            // The node has a property with the same key
            if (val == old_val) {
                // The exact same record, nothing to do
                return;
            }

            // Overwrite the old value
            quad_model.object_key_value->delete_record(*existing_record);
            quad_model.key_value_object->delete_record(*existing_record);
            quad_model.object_key_value->insert({ obj, key, val });
            quad_model.key_value_object->insert({ key, val, obj });

            overwritten_properties++;

            process_deleted_property(old_obj, old_key, old_val);
            process_new_property(obj, key, val);
        } else {
            // The node does not have a property with the same key, create a new one
            quad_model.object_key_value->insert({ obj, key, val });
            quad_model.key_value_object->insert({ key, val, obj });

            process_new_property(obj, key, val);
            key2total_count[key]++;
            new_properties++;
        }
    }

    void insert_edge(uint64_t from, uint64_t to, uint64_t type, uint64_t edge)
    {
        // edge is always new
        quad_model.from_to_type_edge->insert({ from, to, type, edge });
        quad_model.to_type_from_edge->insert({ to, type, from, edge });
        quad_model.type_from_to_edge->insert({ type, from, to, edge });
        quad_model.type_to_from_edge->insert({ type, to, from, edge });
        quad_model.edge_from_to_type->insert({ edge, from, to, type });

        new_edges++;
        type2total_count[type]++;

        if (from == to) {
            quad_model.equal_from_to->insert({ from, type, edge });
            quad_model.equal_from_to_inverted->insert({ type, from, edge });
            type2equal_from_to_count[type]++;

            if (from == type) {
                quad_model.equal_from_to_type->insert({ from, edge });
                type2equal_from_to_type_count[type]++;
            }
        }
        if (from == type) {
            quad_model.equal_from_type->insert({ from, to, edge });
            quad_model.equal_from_type_inverted->insert({ to, from, edge });
            type2equal_from_type_count[type]++;
        }
        if (to == type) {
            quad_model.equal_to_type->insert({ to, from, edge });
            quad_model.equal_to_type_inverted->insert({ from, to, edge });
            type2equal_to_type_count[type]++;
        }
    }

    void set_edge_type(uint64_t edge, uint64_t new_type)
    {
        bool interruption = false;

        // Check if the node has a property with the same key
        Record<4> min_range = { edge, 0, 0, 0 };
        Record<4> max_range = { edge, UINT64_MAX, UINT64_MAX, UINT64_MAX };
        auto iter = quad_model.edge_from_to_type->get_range(&interruption, min_range, max_range);

        if (auto existing_record = iter.next()) {
            auto from = (*existing_record)[1];
            auto to = (*existing_record)[2];
            auto old_type = (*existing_record)[3];

            if (old_type == new_type) {
                return;
            }

            quad_model.edge_from_to_type->delete_record({ edge, from, to, old_type });
            quad_model.from_to_type_edge->delete_record({ from, to, old_type, edge });
            quad_model.to_type_from_edge->delete_record({ to, old_type, from, edge });
            quad_model.type_from_to_edge->delete_record({ old_type, from, to, edge });
            quad_model.type_to_from_edge->delete_record({ old_type, to, from, edge });

            quad_model.edge_from_to_type->insert({ edge, from, to, new_type });
            quad_model.from_to_type_edge->insert({ from, to, new_type, edge });
            quad_model.to_type_from_edge->insert({ to, new_type, from, edge });
            quad_model.type_from_to_edge->insert({ new_type, from, to, edge });
            quad_model.type_to_from_edge->insert({ new_type, to, from, edge });

            overwritten_edges++;

            // delete equal cases
            if (from == to) {
                quad_model.equal_from_to->delete_record({ from, old_type, edge });
                quad_model.equal_from_to_inverted->delete_record({ old_type, from, edge });
                type2equal_from_to_count[old_type]--;

                if (from == old_type) {
                    quad_model.equal_from_to_type->delete_record({ from, edge });
                    type2equal_from_to_type_count[old_type]--;
                }
            }
            if (from == old_type) {
                quad_model.equal_from_type->delete_record({ from, to, edge });
                quad_model.equal_from_type_inverted->delete_record({ to, from, edge });
                type2equal_from_type_count[old_type]--;
            }
            if (to == old_type) {
                quad_model.equal_to_type->delete_record({ to, from, edge });
                quad_model.equal_to_type_inverted->delete_record({ from, to, edge });
                type2equal_to_type_count[old_type]--;
            }

            // insert equal cases
            if (from == to) {
                quad_model.equal_from_to->insert({ from, new_type, edge });
                quad_model.equal_from_to_inverted->insert({ new_type, from, edge });
                type2equal_from_to_count[new_type]++;

                if (from == new_type) {
                    quad_model.equal_from_to_type->insert({ from, edge });
                    type2equal_from_to_type_count[new_type]++;
                }
            }
            if (from == new_type) {
                quad_model.equal_from_type->insert({ from, to, edge });
                quad_model.equal_from_type_inverted->insert({ to, from, edge });
                type2equal_from_type_count[new_type]++;
            }
            if (to == new_type) {
                quad_model.equal_to_type->insert({ to, from, edge });
                quad_model.equal_to_type_inverted->insert({ from, to, edge });
                type2equal_to_type_count[new_type]++;
            }
        }
    }

    ObjectId get_new_edge_id()
    {
        return ObjectId(ObjectId::MASK_EDGE | current_edge++);
    }

    ObjectId get_anon_id()
    {
        return ObjectId(ObjectId::MASK_ANON_INLINED | current_edge++);
    }

    void delete_object(uint64_t obj, bool detach)
    {
        bool interruption = false;

        const Record<4> min_range = { obj, 0, 0, 0 };
        const Record<4> max_range = { obj, UINT64_MAX, UINT64_MAX, UINT64_MAX };
        if (!detach) {

            auto it1 = quad_model.from_to_type_edge->get_range(&interruption, min_range, max_range);
            auto it2 = quad_model.to_type_from_edge->get_range(&interruption, min_range, max_range);
            auto it3 = quad_model.type_from_to_edge->get_range(&interruption, min_range, max_range);
            if (it1.next() != nullptr || it2.next() != nullptr || it3.next() != nullptr) {
                throw QueryException(
                    "Trying to delete object with existing connections (use DETACH DELETE if intended)"
                );
            }
        }

        if (ObjectId(obj).get_type() == ObjectId::MASK_EDGE) {
            auto iter = quad_model.edge_from_to_type->get_range(&interruption, min_range, max_range);

            if (auto existing_record = iter.next()) {
                deleted_edges++;

                auto from = (*existing_record)[1];
                auto to = (*existing_record)[2];
                auto type = (*existing_record)[3];

                quad_model.edge_from_to_type->delete_record({ obj, from, to, type });
                quad_model.from_to_type_edge->delete_record({ from, to, type, obj });
                quad_model.to_type_from_edge->delete_record({ to, type, from, obj });
                quad_model.type_from_to_edge->delete_record({ type, from, to, obj });
                quad_model.type_to_from_edge->delete_record({ type, to, from, obj });

                // delete equal cases
                if (from == to) {
                    quad_model.equal_from_to->delete_record({ from, type, obj });
                    quad_model.equal_from_to_inverted->delete_record({ type, from, obj });
                    type2equal_from_to_count[type]--;

                    if (from == type) {
                        quad_model.equal_from_to_type->delete_record({ from, obj });
                        type2equal_from_to_type_count[type]--;
                    }
                }
                if (from == type) {
                    quad_model.equal_from_type->delete_record({ from, to, obj });
                    quad_model.equal_from_type_inverted->delete_record({ to, from, obj });
                    type2equal_from_type_count[type]--;
                }
                if (to == type) {
                    quad_model.equal_to_type->delete_record({ to, from, obj });
                    quad_model.equal_to_type_inverted->delete_record({ from, to, obj });
                    type2equal_to_type_count[type]--;
                }
            }
        } else {
            if (quad_model.nodes->delete_record({ obj })) {
                deleted_nodes++;
            }
        }

        // save here to delete later, because delete while iterating is a bad idea
        std::set<uint64_t> edges_to_delete;

        auto it1 = quad_model.from_to_type_edge->get_range(&interruption, min_range, max_range);
        for (auto record = it1.next(); record != nullptr; record = it1.next()) {
            edges_to_delete.insert((*record)[3]);
        }
        auto it2 = quad_model.to_type_from_edge->get_range(&interruption, min_range, max_range);
        for (auto record = it2.next(); record != nullptr; record = it2.next()) {
            edges_to_delete.insert((*record)[3]);
        }
        auto it3 = quad_model.type_from_to_edge->get_range(&interruption, min_range, max_range);
        for (auto record = it3.next(); record != nullptr; record = it3.next()) {
            edges_to_delete.insert((*record)[3]);
        }

        // save here to delete later, because delete while iterating is a bad idea
        std::set<std::pair<uint64_t, uint64_t>> props_to_delete;

        auto prop_iter = quad_model.object_key_value
                             ->get_range(&interruption, { obj, 0, 0 }, { obj, UINT64_MAX, UINT64_MAX });

        for (auto record = prop_iter.next(); record != nullptr; record = prop_iter.next()) {
            auto key = (*record)[1];
            auto value = (*record)[2];

            props_to_delete.emplace(key, value);
        }

        for (auto edge : edges_to_delete) {
            delete_object(edge, true);
        }

        for (auto&& [k, v] : props_to_delete) {
            quad_model.object_key_value->delete_record({ obj, k, v });
            quad_model.key_value_object->delete_record({ k, v, obj });

            process_deleted_property(obj, k, v);
            deleted_properties++;
            key2total_count[k]--;
        }
    }

    void delete_label(uint64_t node, uint64_t label)
    {
        if (quad_model.node_label->delete_record({ node, label })) {
            quad_model.label_node->delete_record({ label, node });

            label2total_count[label]--;
            deleted_labels++;
        }
    }

    void delete_property(uint64_t obj, uint64_t key)
    {
        bool interruption = false;
        Record<3> min_range = { obj, key, 0 };
        Record<3> max_range = { obj, key, UINT64_MAX };
        auto prop_iter = quad_model.object_key_value->get_range(&interruption, min_range, max_range);

        if (auto existing_record = prop_iter.next()) {
            auto value = (*existing_record)[2];

            quad_model.object_key_value->delete_record({ obj, key, value });
            quad_model.key_value_object->delete_record({ key, value, obj });

            process_deleted_property(obj, key, value);
            deleted_properties++;
            key2total_count[key]--;
        }
    }
};
} // namespace MQL
