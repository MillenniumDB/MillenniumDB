#include "update_executor.h"

#include <cassert>

#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "query/parser/op/mql/update/op_create_hnsw_index.h"
#include "query/parser/op/mql/update/op_create_text_index.h"
#include "query/parser/op/mql/update/op_insert.h"
#include "storage/index/bplus_tree/bplus_tree.h"
#include "storage/index/text_search/text_index.h"
#include "storage/index/text_search/text_index_manager.h"

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
    return true;
}

void UpdateExecutor::visit(OpInsert& op)
{
    // Store all the inserts and deletes (for replace) that will be necessary for the indexes
    using Name2InsertsMap = boost::unordered_map<std::string, std::vector<std::tuple<ObjectId, ObjectId>>>;

    const auto& text_index_predicate2names = quad_model.catalog.text_index_manager.get_predicate2names();
    Name2InsertsMap text_index_name2inserts;
    Name2InsertsMap text_index_name2deletes;

    const auto& hnsw_index_predicate2names = quad_model.catalog.hnsw_index_manager.get_predicate2names();
    Name2InsertsMap hnsw_index_name2inserts;
    Name2InsertsMap hnsw_index_name2deletes;

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
            ++quad_model.catalog.nodes_count;
            ++graph_update_data.new_nodes;
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
            quad_model.node_label->insert({ node.id, label.id });
            quad_model.catalog.insert_label(label.id);
            ++graph_update_data.new_labels;
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

        bool is_new_node_type = quad_model.nodes->insert({ type.id });
        if (is_new_node_type) {
            quad_model.catalog.nodes_count++;
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

            ++graph_update_data.overwritten_properties;

            // Overwrite the old value in the indexes
            if (!text_index_predicate2names.empty()) {
                const auto predicate_str = MQL::Conversions::unpack_string(key);
                const auto it = text_index_predicate2names.find(predicate_str);
                if (it != text_index_predicate2names.end()) {
                    for (const auto& name : it->second) {
                        text_index_name2deletes[name].emplace_back(node, (*existing_record)[2]);
                        text_index_name2inserts[name].emplace_back(node, value);
                    }
                }
            }

            if (!hnsw_index_predicate2names.empty()) {
                const auto predicate_str = MQL::Conversions::unpack_string(key);
                const auto it = hnsw_index_predicate2names.find(predicate_str);
                if (it != hnsw_index_predicate2names.end()) {
                    for (const auto& name : it->second) {
                        hnsw_index_name2deletes[name].emplace_back(node, (*existing_record)[2]);
                        hnsw_index_name2inserts[name].emplace_back(node, value);
                    }
                }
            }
        } else {
            // The node does not have a property with the same key, create a new one
            quad_model.object_key_value->insert({ node.id, key.id, value.id });
            quad_model.key_value_object->insert({ key.id, value.id, node.id });
            quad_model.catalog.insert_property(key.id);

            ++graph_update_data.new_properties;

            // Insert into the indexes
            if (!text_index_predicate2names.empty()) {
                const auto predicate_str = MQL::Conversions::unpack_string(key);
                const auto it = text_index_predicate2names.find(predicate_str);
                if (it != text_index_predicate2names.end()) {
                    for (const auto& name : it->second) {
                        text_index_name2inserts[name].emplace_back(node, value);
                    }
                }
            }

            if (!hnsw_index_predicate2names.empty()) {
                const auto predicate_str = MQL::Conversions::unpack_string(key);
                const auto it = hnsw_index_predicate2names.find(predicate_str);
                if (it != hnsw_index_predicate2names.end()) {
                    for (const auto& name : it->second) {
                        hnsw_index_name2inserts[name].emplace_back(node, value);
                    }
                }
            }
        }
    }

    // Execute index updates, starting with deletes
    // DELETES
    for (const auto& [name, deletes] : text_index_name2deletes) {
        auto* text_index_ptr = quad_model.catalog.text_index_manager.get_text_index(name);
        assert(text_index_ptr != nullptr && "Text index not found");

        uint_fast32_t removed_elements { 0 };
        uint_fast32_t removed_tokens { 0 };
        for (const auto& [node, value] : deletes) {
            const auto current_deleted_tokens = text_index_ptr->remove_single(node, value);
            if (current_deleted_tokens > 0) {
                ++removed_elements;
                removed_tokens += current_deleted_tokens;
            }
        }

        TextIndexUpdateData text_index_update_data;
        text_index_update_data.index_name = name;
        text_index_update_data.removed_elements = removed_elements;
        text_index_update_data.removed_tokens = removed_tokens;
        insert_text_index_update_data(std::move(text_index_update_data));
    }

    for (const auto& [name, deletes] : hnsw_index_name2deletes) {
        auto* hnsw_index_ptr = quad_model.catalog.hnsw_index_manager.get_hnsw_index(name);
        assert(hnsw_index_ptr != nullptr && "Text index not found");

        uint_fast32_t removed_elements { 0 };
        for (const auto& [node, value] : deletes) {
            if (hnsw_index_ptr->remove_single(node, value)) {
                ++removed_elements;
            }
        }

        HNSWIndexUpdateData hnsw_index_update_data;
        hnsw_index_update_data.index_name = name;
        hnsw_index_update_data.removed_elements = removed_elements;
        insert_hnsw_index_update_data(std::move(hnsw_index_update_data));
    }

    // INSERTS
    for (const auto& [name, inserts] : text_index_name2inserts) {
        auto* text_index_ptr = quad_model.catalog.text_index_manager.get_text_index(name
        );
        assert(text_index_ptr != nullptr && "Text index not found");

        uint_fast32_t inserted_elements { 0 };
        uint_fast32_t inserted_tokens { 0 };
        for (const auto& [node, value] : inserts) {
            const auto current_inserted_tokens = text_index_ptr->index_single(node, value);
            if (current_inserted_tokens > 0) {
                ++inserted_elements;
                inserted_tokens += current_inserted_tokens;
            }
        }

        TextIndexUpdateData text_index_update_data;
        text_index_update_data.index_name = name;
        text_index_update_data.inserted_elements = inserted_elements;
        text_index_update_data.inserted_tokens = inserted_tokens;
        insert_text_index_update_data(std::move(text_index_update_data));
    }

    for (const auto& [name, inserts] : hnsw_index_name2inserts) {
        auto* hnsw_index_ptr = quad_model.catalog.hnsw_index_manager.get_hnsw_index(name
        );
        assert(hnsw_index_ptr != nullptr && "Text index not found");

        uint_fast32_t inserted_elements { 0 };
        uint_fast32_t inserted_tokens { 0 };
        for (const auto& [node, value] : inserts) {
            if (hnsw_index_ptr->index_single<true>(node, value)) {
                ++inserted_elements;
            }
        }

        TextIndexUpdateData text_index_update_data;
        text_index_update_data.index_name = name;
        text_index_update_data.inserted_elements = inserted_elements;
        text_index_update_data.inserted_tokens = inserted_tokens;
        insert_text_index_update_data(std::move(text_index_update_data));
    }
}

void UpdateExecutor::visit(OpCreateHNSWIndex& op)
{
    try {
        auto& hnsw_index_manager = quad_model.catalog.hnsw_index_manager;
        const auto& [inserted_elements] = hnsw_index_manager.create_hnsw_index<Catalog::ModelID::QUAD>(
            op.index_name,
            op.property,
            op.dimension,
            op.max_edges,
            op.max_candidates,
            op.metric_type
        );

        HNSWIndexUpdateData hnsw_index_update_data;
        hnsw_index_update_data.index_name = op.index_name;
        hnsw_index_update_data.created = true;
        hnsw_index_update_data.inserted_elements = inserted_elements;
        insert_hnsw_index_update_data(std::move(hnsw_index_update_data));
    } catch (const std::exception& e) {
        // Rethrow any exception wrapped by a QueryExecutionException
        throw QueryExecutionException(e.what());
    }
}

void UpdateExecutor::visit(OpCreateTextIndex& op)
{
    try {
        auto& text_index_manager = quad_model.catalog.text_index_manager;
        const auto& [inserted_elements, inserted_tokens] = text_index_manager.create_text_search_index(
            op.index_name,
            op.property,
            op.normalize_type,
            op.tokenize_type
        );

        TextIndexUpdateData text_index_update_data;
        text_index_update_data.index_name = op.index_name;
        text_index_update_data.created = true;
        text_index_update_data.inserted_elements = inserted_elements;
        text_index_update_data.inserted_tokens = inserted_tokens;
        insert_text_index_update_data(std::move(text_index_update_data));
    } catch (const std::exception& e) {
        // Rethrow any exception wrapped by a QueryExecutionException
        throw QueryExecutionException(e.what());
    }
}

void UpdateExecutor::print_stats(std::ostream& os)
{
    bool has_changes = false;

    if (!graph_update_data.empty()) {
        os << "Graph updates:\n";
        os << "  " << graph_update_data << '\n';
        has_changes = true;
    }

    if (!name2text_index_update_data.empty()) {
        os << "Text Index updates:\n";
        for (const auto& [_, text_index_update] : name2text_index_update_data) {
            os << "  " << text_index_update << '\n';
        }
        has_changes = true;
    }

    if (!name2hnsw_index_update_data.empty()) {
        os << "HNSW Index updates:\n";
        for (const auto& [_, hnsw_index_update] : name2hnsw_index_update_data) {
            os << "  " << hnsw_index_update << '\n';
        }
        has_changes = true;
    }

    if (!has_changes) {
        os << "No modifications were performed\n";
    }
}

void UpdateExecutor::insert_text_index_update_data(TextIndexUpdateData&& data)
{
    auto it = name2text_index_update_data.find(data.index_name);
    if (it == name2text_index_update_data.end()) {
        name2text_index_update_data.emplace(data.index_name, std::move(data));
    } else {
        it->second.inserted_elements += data.inserted_elements;
        it->second.inserted_tokens += data.inserted_tokens;
        it->second.removed_elements += data.removed_elements;
        it->second.removed_tokens += data.removed_tokens;
    }
}

void UpdateExecutor::insert_hnsw_index_update_data(HNSWIndexUpdateData&& data)
{
    auto it = name2hnsw_index_update_data.find(data.index_name);
    if (it == name2hnsw_index_update_data.end()) {
        name2hnsw_index_update_data.emplace(data.index_name, std::move(data));
    } else {
        it->second.inserted_elements += data.inserted_elements;
        it->second.removed_elements += data.removed_elements;
    }
}
