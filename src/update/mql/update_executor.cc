#include "update_executor.h"

#include <cassert>

#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "query/parser/op/mql/update/op_create_tensor_store.h"
#include "query/parser/op/mql/update/op_create_text_search_index.h"
#include "query/parser/op/mql/update/op_delete_tensors.h"
#include "query/parser/op/mql/update/op_insert.h"
#include "query/parser/op/mql/update/op_insert_tensors.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/random_access_table/random_access_table.h"
#include "storage/index/tensor_store/tensor_store.h"
#include "storage/index/tensor_store/tensor_store_manager.h"
#include "storage/index/text_search/text_search_index.h"
#include "storage/index/text_search/text_search_index_manager.h"

using namespace MQL;

constexpr uint64_t CLEAR_TMP_MASK = ~(ObjectId::MOD_MASK | ObjectId::MASK_EXTERNAL_ID);

UpdateExecutor::~UpdateExecutor()
{
    // TODO: force string file WAL flush?
}

void UpdateExecutor::execute(Op& op)
{
    op.accept_visitor(*this);
}

bool UpdateExecutor::transform_if_tmp(ObjectId& oid)
{
    if (oid.is_tmp()) {
        // we need to materialize the string
        auto tmp_id = oid.id & ObjectId::MASK_EXTERNAL_ID;
        auto& str = tmp_manager.get_str(tmp_id);
        auto new_external_id = string_manager.get_or_create(str.data(), str.size());

        oid.id = (oid.id & CLEAR_TMP_MASK) | ObjectId::MOD_EXTERNAL | new_external_id;
    }
    return true;
}

void UpdateExecutor::visit(OpInsert& op)
{
    // Store all the inserts and deletes that will be necessary for the text search index
    // There could be inserts because when a existing property is inserted, it will be replaced
    const auto& predicate2names = quad_model.catalog.text_search_index_manager.get_predicate2names();
    boost::unordered_map<std::string, std::vector<std::tuple<ObjectId, ObjectId>>>
        text_search_index_name2deletes;
    boost::unordered_map<std::string, std::vector<std::tuple<ObjectId, ObjectId>>>
        text_search_index_name2inserts;

    auto& bgp = *op.bgp;

    // These are not only disjoint terms, they are any node from the query
    for (auto& disjoint_term : bgp.disjoint_terms) {
        auto node = disjoint_term.term;
        auto original_node = node;
        if (transform_if_tmp(node)) {
            created_ids.insert({ original_node, node });
        }
        bool is_new_node = quad_model.nodes->insert({ node.id });
        if (is_new_node) {
            quad_model.catalog.identifiable_nodes_count++;
        }
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
            ++graph_update_data.new_labels;
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
        ++graph_update_data.new_edges;

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

        bool is_new_node_type = quad_model.nodes->insert({ type.id });
        if (is_new_node_type) {
            quad_model.catalog.identifiable_nodes_count++;
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

        // Check if the node has a property with the same key
        bool interruption_requested = false;
        const Record<3> min_range = { node.id, key.id, 0 };
        const Record<3> max_range = { node.id, key.id, UINT64_MAX };
        auto object_key_value_iter = quad_model.object_key_value
                                         ->get_range(&interruption_requested, min_range, max_range);
        const auto existing_record = object_key_value_iter.next();

        if (existing_record != nullptr) {
            // The node has a property with the same key
            if (value.id == (*existing_record)[2]) {
                // The exact same record, nothing to do
                continue;
            }
            // Overwrite the old value
            quad_model.object_key_value->delete_record(*existing_record);
            quad_model.key_value_object->delete_record(*existing_record);
            quad_model.object_key_value->insert({ node.id, key.id, value.id });
            quad_model.key_value_object->insert({ key.id, value.id, node.id });

            if (!predicate2names.empty()) {
                const auto predicate_str = MQL::Conversions::unpack_string(key);
                auto it = predicate2names.find(predicate_str);
                if (it != predicate2names.end()) {
                    for (const auto& name : it->second) {
                        text_search_index_name2deletes[name].emplace_back(node, (*existing_record)[2]);
                        text_search_index_name2inserts[name].emplace_back(node, value);
                    }
                }
            }
        } else {
            // The node does not have a property with the same key, create a new one
            quad_model.object_key_value->insert({ node.id, key.id, value.id });
            quad_model.key_value_object->insert({ key.id, value.id, node.id });
            quad_model.catalog.insert_property(key.id);

            if (!predicate2names.empty()) {
                const auto predicate_str = MQL::Conversions::unpack_string(key);
                auto it = predicate2names.find(predicate_str);
                if (it != predicate2names.end()) {
                    for (const auto& name : it->second) {
                        text_search_index_name2inserts[name].emplace_back(node, value);
                    }
                }
            }
        }

        ++graph_update_data.new_properties;
    }

    // Execute text search index updates, starting with deletes
    TextSearch::TextSearchIndex* text_search_index;

    for (const auto& [name, deletes] : text_search_index_name2deletes) {
        [[maybe_unused]] const auto found = quad_model.catalog.text_search_index_manager
                                                .get_text_search_index(name, &text_search_index);
        assert(found && "Text search index not found");

        uint_fast32_t removed_elements { 0 };
        uint_fast32_t removed_tokens { 0 };
        for (const auto& [node, value] : deletes) {
            const auto current_deleted_tokens = text_search_index->remove_single(node, value);
            if (current_deleted_tokens > 0) {
                ++removed_elements;
                removed_tokens += current_deleted_tokens;
            }
        }

        TextSearchIndexUpdateData tsi_update;
        tsi_update.text_search_index_name = name;
        tsi_update.removed_elements = removed_elements;
        tsi_update.removed_tokens = removed_tokens;
        text_search_index_updates.emplace_back(std::move(tsi_update));
    }

    for (const auto& [name, inserts] : text_search_index_name2inserts) {
        [[maybe_unused]] const auto found = quad_model.catalog.text_search_index_manager
                                                .get_text_search_index(name, &text_search_index);
        assert(found && "Text search index not found");

        uint_fast32_t inserted_elements { 0 };
        uint_fast32_t inserted_tokens { 0 };
        for (const auto& [node, value] : inserts) {
            const auto current_inserted_tokens = text_search_index->index_single(node, value);
            if (current_inserted_tokens > 0) {
                ++inserted_elements;
                inserted_tokens += current_inserted_tokens;
            }
        }

        TextSearchIndexUpdateData tsi_update;
        tsi_update.text_search_index_name = name;
        tsi_update.inserted_elements = inserted_elements;
        tsi_update.inserted_tokens = inserted_tokens;
        text_search_index_updates.emplace_back(std::move(tsi_update));
    }
}

void UpdateExecutor::visit(OpInsertTensors& op)
{
    assert(!op.inserts.empty());
    assert(
        std::all_of(
            op.inserts.begin(),
            op.inserts.end(),
            [&](const auto& insert) -> bool {
                return std::get<1>(insert).size() == std::get<1>(op.inserts[0]).size();
            }
        )
        && "Tensors must not come with different sizes at this point!"
    );

    TensorStore* tensor_store;
    const bool found = tensor_store_manager.get_tensor_store(op.tensor_store_name, &tensor_store);
    if (!found) {
        throw QueryExecutionException("TensorStore \"" + op.tensor_store_name + "\" does not exist");
    }

    const auto inserted_tensors_dimension = std::get<1>(op.inserts[0]).size();
    if (tensor_store->tensors_dim() != std::get<1>(op.inserts[0]).size()) {
        throw QueryExecutionException(
            "TensorStore \"" + op.tensor_store_name + "\" has dimension "
            + std::to_string(tensor_store->tensors_dim()) + " but the inserted tensors has dimension "
            + std::to_string(inserted_tensors_dimension)
        );
    }

    tensor_update_data.tensor_store_name = op.tensor_store_name;

    for (const auto& insert : op.inserts) {
        // Insert node if it does not exist
        auto node = std::get<0>(insert);
        auto original_node = node;
        if (transform_if_tmp(node)) {
            created_ids.insert({ original_node, node });
        }
        bool is_new_node = quad_model.nodes->insert({ node.id });
        if (is_new_node) {
            quad_model.catalog.identifiable_nodes_count++;
        }

        // Insert into tensor store
        const bool new_key = tensor_store->insert(node, std::get<1>(insert).data());
        if (new_key) {
            ++tensor_update_data.new_entries;
        } else {
            ++tensor_update_data.overwritten_entries;
        }
    }
}

void UpdateExecutor::visit(OpDeleteTensors& op)
{
    assert(!op.deletes.empty());

    TensorStore* tensor_store;
    const bool found = tensor_store_manager.get_tensor_store(op.tensor_store_name, &tensor_store);
    if (!found) {
        throw QueryExecutionException("TensorStore \"" + op.tensor_store_name + "\" does not exist");
    }

    tensor_update_data.tensor_store_name = op.tensor_store_name;

    for (const auto& object_id : op.deletes) {
        const bool removed = tensor_store->remove(object_id);
        if (removed) {
            ++tensor_update_data.deleted_entries;
        }
    }
}

void UpdateExecutor::visit(OpCreateTensorStore& op)
{
    assert(op.tensors_dim > 0);

    try {
        tensor_store_manager.create_tensor_store(op.tensor_store_name, op.tensors_dim);

        tensor_update_data.tensor_store_name = op.tensor_store_name;
        tensor_update_data.created = true;
    } catch (const std::exception& e) {
        // Rethrow any exception wrapped by a QueryExecutionException
        throw QueryExecutionException(e.what());
    }
}

void UpdateExecutor::visit(OpCreateTextSearchIndex& op)
{
    try {
        auto& text_search_index_manager = quad_model.catalog.text_search_index_manager;
        const auto& [inserted_elements, inserted_tokens] = text_search_index_manager.create_text_search_index(
            op.text_search_index_name,
            op.property,
            op.normalize_type,
            op.tokenize_type
        );

        TextSearchIndexUpdateData tsi_update;
        tsi_update.text_search_index_name = op.text_search_index_name;
        tsi_update.inserted_elements = inserted_elements;
        tsi_update.inserted_tokens = inserted_tokens;
        text_search_index_updates.emplace_back(std::move(tsi_update));
    } catch (const std::exception& e) {
        // Rethrow any exception wrapped by a QueryExecutionException
        throw QueryExecutionException(e.what());
    }
}

void UpdateExecutor::print_stats(std::ostream& os)
{
    bool has_changes = false;
    if (!graph_update_data.empty()) {
        // The graph was updated
        os << graph_update_data;
        has_changes = true;
    }

    if (!tensor_update_data.empty()) {
        // A TensorStore was updated
        os << tensor_update_data;
        has_changes = true;
    }

    if (!text_search_index_updates.empty()) {
        // A TextSearchIndex was updated
        os << "TextSearchIndex updates:\n";
        for (const auto& text_search_update_data : text_search_index_updates) {
            os << text_search_update_data;
            os << '\n';
        }
        has_changes = true;
    }

    if (!has_changes) {
        os << "No modifications were performed\n";
    }
}
