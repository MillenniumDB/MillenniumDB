#include "quad.h"

#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_object_id.h"
#include "misc/fatal_error.h"
#include "query/optimizer/quad_model/plan/property_plan.h"
#include "query/query_context.h"
#include "storage/index/text_search/utils.h"

namespace TextSearch { namespace Quad {

std::tuple<uint_fast32_t, uint_fast32_t> index_predicate(
    Trie& trie,
    BPlusTree<2>& bpt,
    Table& table,
    TokenizeFuncType* tokenize,
    NormalizeFuncType* normalize,
    const std::string& predicate
)
{
    const auto object_var = get_query_ctx().get_internal_var();
    const auto key_oid = QuadObjectId::get_string(predicate);
    const auto value_var = get_query_ctx().get_internal_var();

    const auto property_plan = PropertyPlan(object_var, key_oid, value_var);
    auto property_plan_iter = property_plan.get_binding_iter();

    Binding property_plan_binding(get_query_ctx().get_var_size());
    property_plan_iter->begin(property_plan_binding);

    uint_fast32_t total_inserted_elements { 0 };
    uint_fast32_t total_inserted_tokens { 0 };
    while (property_plan_iter->next()) {
        const auto object_oid = property_plan_binding[object_var];
        const auto value_oid = property_plan_binding[value_var];
        const auto
            inserted_tokens = index_single(trie, bpt, table, tokenize, normalize, object_oid, value_oid);
        if (inserted_tokens > 0) {
            ++total_inserted_elements;
            total_inserted_tokens += inserted_tokens;
        }
    }

    return { total_inserted_elements, total_inserted_tokens };
}

uint_fast32_t index_single(
    Trie& trie,
    BPlusTree<2>& bpt,
    Table& table,
    TokenizeFuncType* tokenize,
    NormalizeFuncType* normalize,
    ObjectId object_oid,
    ObjectId property_oid
)
{
    const auto gen_t = property_oid.id & ObjectId::GENERIC_TYPE_MASK;
    if (gen_t != ObjectId::MASK_STRING) {
        return 0;
    }

    const auto property_str = MQL::Conversions::to_lexical_str(property_oid);
    const auto table_pointer = table.insert({ object_oid.id, property_oid.id });

    const auto tokens = tokenize(property_str);
    std::unordered_map<std::string, uint64_t> processed_tokens;

    for (const auto& token : tokens) {
        const auto normalized = normalize(token);

        const auto& pair = processed_tokens.emplace(normalized, 1);
        if (!pair.second) {
            // Increment the count if it was previously inserted
            ++pair.first->second;
        }
    }

    for (const auto& [token, count] : processed_tokens) {
        const auto trie_node_id = trie.insert_string(token);

        constexpr double M = static_cast<double>(UINT64_MAX);
        const uint64_t score = M - M * (double(count) / double(tokens.size()));

        const auto [first, second] = compress(trie_node_id, score, table_pointer);
        bpt.insert({ first, second });
    }

    return processed_tokens.size();
}

uint_fast32_t remove_single(
    Trie& trie,
    BPlusTree<2>& bpt,
    Table& table,
    TokenizeFuncType* tokenize,
    NormalizeFuncType* normalize,
    ObjectId object_oid,
    ObjectId property_oid
)
{
    const auto gen_t = property_oid.id & ObjectId::GENERIC_TYPE_MASK;
    if (gen_t != ObjectId::MASK_STRING) {
        return 0;
    }

    const auto property_str = MQL::Conversions::to_lexical_str(property_oid);
    const auto tokens = tokenize(property_str);

    std::unordered_set<std::string> processed_tokens;
    for (const auto& token : tokens) {
        const auto normalized = normalize(token);
        processed_tokens.emplace(normalized);
    }

    // Store { bpt_record, table_row} to remove
    std::vector<std::tuple<Record<2>, uint64_t>> to_remove_data;
    for (const auto& token : processed_tokens) {
        const auto trie_iter = trie.search<SearchType::Match, false>(token);
        while (trie_iter->next()) {
            const auto trie_node_id = trie_iter->get_node_id();

            const auto& [first_min, second_min] = compress(trie_node_id, 0, 0);
            const auto& [first_max, second_max] = compress(trie_node_id, UINT32_MAX, UINT64_MAX);
            bool interruption_requested = false;
            auto bpt_iter = bpt.get_range(
                &interruption_requested,
                { first_min, second_min },
                { first_max, second_max }
            );

            for (auto record = bpt_iter.next(); record != nullptr; record = bpt_iter.next()) {
                const auto [node_id, score, table_pointer] = decompress((*record)[0], (*record)[1]);
                const auto table_row = table.get(table_pointer);

                if (table_row[0] == object_oid.id && table_row[1] == property_oid.id) {
                    to_remove_data.emplace_back(std::move(*record), table_pointer);
                }
            }
        }
    }

    for (const auto& [record, table_pointer] : to_remove_data) {
        bpt.delete_record(record);
        table.remove(table_pointer);
    }

    return to_remove_data.size();
}

std::string oid_to_string(ObjectId oid)
{
    return MQL::Conversions::to_lexical_str(oid);
}
}} // namespace TextSearch::Quad
