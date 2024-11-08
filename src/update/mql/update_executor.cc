#include "update_executor.h"

#include "graph_models/quad_model/quad_model.h"
#include "query/parser/op/mql/update/op_insert.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/random_access_table/random_access_table.h"

using namespace MQL;

constexpr uint64_t CLEAR_TMP_MASK = ~(ObjectId::MOD_MASK | ObjectId::MASK_EXTERNAL_ID);

UpdateExecutor::~UpdateExecutor() {
    // TODO: force string file WAL flush?
}


void UpdateExecutor::execute(Op& op) {
    op.accept_visitor(*this);
}

bool UpdateExecutor::transform_if_tmp(ObjectId& oid) {
    if (oid.is_tmp()) {
        // we need to materialize the string
        auto tmp_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        auto& str = tmp_manager.get_str(tmp_id);
        auto new_external_id = string_manager.get_or_create(str.data(), str.size());

        oid.id = (oid.id & CLEAR_TMP_MASK) | ObjectId::MOD_EXTERNAL | new_external_id;
    }
    return true;
}


void UpdateExecutor::visit(OpInsert& op) {
    auto& bgp = *op.bgp;

    for (auto& disjoint_term : bgp.disjoint_terms) {
        quad_model.nodes->insert({ disjoint_term.term.id });
    }

    for (auto& op_label : bgp.labels) {
        assert(op_label.label.is_OID());
        assert(op_label.node.is_OID());

        // assuming op_label.node is NamedNode
        auto label = op_label.label.get_OID();
        auto node = op_label.node.get_OID();

        auto original_label = label;
        auto original_node = node;

        if (transform_if_tmp(label)) {
            created_ids.insert({ original_label, label });
        }
        if (transform_if_tmp(node)) {
            created_ids.insert({ original_node, node });
        }

        assert(!label.is_tmp());
        assert(!node.is_tmp());

        bool is_new_label_record = quad_model.label_node->insert({ label.id, node.id });
        if (is_new_label_record) {
            stat_new_labels++;
            quad_model.node_label->insert({ node.id, label.id });
            quad_model.catalog.insert_label(label.id);
        }
    }

    // need to remember for edge properties
    std::map<VarId, ObjectId> assigned_edges;

    for (auto& op_edge : bgp.edges) {
        assert(op_edge.from.is_OID());
        assert(op_edge.to.is_OID());
        assert(op_edge.type.is_OID());
        assert(op_edge.edge.is_var());

        // assuming op_label.node is NamedNode
        auto from = op_edge.from.get_OID();
        auto to = op_edge.to.get_OID();
        auto type = op_edge.type.get_OID();

        auto original_from = from;
        auto original_to = to;
        auto original_type = type;

        if (transform_if_tmp(from)) {
            created_ids.insert({ original_from, from });
        }
        if (transform_if_tmp(to)) {
            created_ids.insert({ original_to, to });
        }
        if (transform_if_tmp(type)) {
            created_ids.insert({ original_type, type });
        }

        // edge is always new
        stat_new_edges++;

        ObjectId edge(ObjectId::MASK_EDGE | quad_model.catalog.insert_new_edge(from.id, to.id, type.id));
        quad_model.from_to_type_edge->insert({ from.id, to.id, type.id, edge.id });
        quad_model.to_type_from_edge->insert({ to.id, type.id, from.id, edge.id });
        quad_model.type_from_to_edge->insert({ type.id, from.id, to.id, edge.id });
        quad_model.type_to_from_edge->insert({ type.id, to.id, from.id, edge.id });

        // TODO: only works when always appending, if reusing deleted edge ids this need to be changed
        quad_model.edge_table->append_record({ from.id, to.id, type.id });

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

        assigned_edges.insert({ op_edge.edge.get_var(), edge });
    }


    for (auto& op_property : bgp.properties) {
        assert(op_property.value.is_OID());

        auto node = op_property.node.is_OID() ? op_property.node.get_OID()
                                              : assigned_edges[op_property.node.get_var()];
        auto key = op_property.key;
        auto value = op_property.value.get_OID();

        auto original_node = node;
        auto original_key = key;
        auto original_value = value;

        if (transform_if_tmp(node)) {
            created_ids.insert({ original_node, node });
        }
        if (transform_if_tmp(key)) {
            created_ids.insert({ original_key, key });
        }
        if (transform_if_tmp(value)) {
            created_ids.insert({ original_value, value });
        }

        assert(!node.is_tmp());
        assert(!key.is_tmp());
        assert(!value.is_tmp());

        bool is_new_property_record = quad_model.key_value_object->insert({ key.id, value.id, node.id });
        if (is_new_property_record) {
            stat_new_properties++;
            quad_model.object_key_value->insert({ node.id, key.id, value.id });
            quad_model.catalog.insert_property(key.id);
        }
    }
}

void UpdateExecutor::print_stats(std::ostream& os) {
    if (stat_new_edges + stat_new_labels + stat_new_properties == 0) {
        os << "no changes made\n";
        return;
    }

    if (stat_new_edges > 0) {
        os << "Created " << stat_new_edges << " new edges\n";
    }
    if (stat_new_labels > 0) {
        os << "Created " << stat_new_labels << " new labels\n";
    }
    if (stat_new_properties > 0) {
        os << "Created " << stat_new_properties << " new properties\n";
    }
}
