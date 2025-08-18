#pragma once

#include "graph_models/object_id.h"
#include "graph_models/quad_model/quad_model.h"

namespace MQL {

class UpdateContext {
public:
    uint_fast32_t new_nodes = 0;
    uint_fast32_t new_edges = 0;
    uint_fast32_t new_labels = 0;
    uint_fast32_t new_properties = 0;
    uint_fast32_t deleted_nodes = 0;
    uint_fast32_t deleted_edges = 0;
    uint_fast32_t deleted_labels = 0;
    uint_fast32_t deleted_properties = 0;
    uint_fast32_t overwritten_properties = 0;

    boost::unordered_flat_map<uint64_t, int64_t> label2total_count;
    boost::unordered_flat_map<uint64_t, int64_t> key2total_count;
    boost::unordered_flat_map<uint64_t, int64_t> type2total_count;

    boost::unordered_flat_map<uint64_t, int64_t> type2equal_from_to_type_count;
    boost::unordered_flat_map<uint64_t, int64_t> type2equal_from_to_count;
    boost::unordered_flat_map<uint64_t, int64_t> type2equal_from_type_count;
    boost::unordered_flat_map<uint64_t, int64_t> type2equal_to_type_count;

    uint64_t current_anon;

    uint64_t current_edge;

    UpdateContext()
    {
        current_anon = quad_model.catalog.max_anon;
        current_edge = quad_model.catalog.max_edge;
    }

    void process_new_property(uint64_t obj, uint64_t key, uint64_t val);
    void process_deleted_property(uint64_t obj, uint64_t key, uint64_t val);

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

    void set_edge_type(uint64_t edge, uint64_t type)
    {
        // TODO:
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

    ObjectId get_new_edge_id()
    {
        return ObjectId(ObjectId::MASK_EDGE | current_edge++);
    }

    ObjectId get_anon_id()
    {
        return ObjectId(ObjectId::MASK_ANON_INLINED | current_edge++);
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