#include "quad.h"

#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_object_id.h"
#include "misc/fatal_error.h"
#include "query/optimizer/quad_model/plan/label_plan.h"
#include "query/optimizer/quad_model/plan/property_plan.h"
#include "query/query_context.h"
#include "storage/index/text_search/utils.h"


namespace TextSearch { namespace Quad {


void index_predicate(Trie&          trie,
                     BPlusTree<2>&  bpt,
                     Table&         table,
                     TokenizeFunc*  tokenize,
                     NormalizeFunc* normalize,
                     std::string    predicate)
{
    auto colon_idx = predicate.find(":");
    if (colon_idx == std::string::npos) {
        FATAL_ERROR("Expected string of form \"label:key\" for Quad Model indexing");
    }

    auto label_str = predicate.substr(0, colon_idx);
    if (label_str.empty()) {
        FATAL_ERROR("Expected string of form \"label:key\" for Quad Model indexing\nHowever, the label was empty");
    }

    auto key_str = predicate.substr(colon_idx + 1);
    if (key_str.empty()) {
        FATAL_ERROR("Expected string of form \"label:key\" for Quad Model indexing\nHowever, the key was empty");
    }

    auto object_var = get_query_ctx().get_internal_var();
    auto label_oid = QuadObjectId::get_string(label_str);
    auto key_oid   = QuadObjectId::get_string(key_str);
    auto value_var = get_query_ctx().get_internal_var();

    Binding label_plan_binding(get_query_ctx().get_var_size());
    Binding property_plan_binding(get_query_ctx().get_var_size());

    auto label_plan = LabelPlan(object_var, label_oid);
    auto label_plan_iter = label_plan.get_binding_iter();

    label_plan_iter->begin(label_plan_binding);

    while (label_plan_iter->next()) {
        auto object_oid = label_plan_binding[object_var];
        auto property_plan = PropertyPlan(object_oid, key_oid, value_var);
        auto property_plan_iter = property_plan.get_binding_iter();
        property_plan_iter->begin(property_plan_binding);

        while (property_plan_iter->next()) {
            auto value_oid = property_plan_binding[value_var];
            auto value_string = MQL::Conversions::to_lexical_str(value_oid);

            // std::cout << "Value: " << value_string << "\n";

            auto table_pointer = table.insert({ object_oid.id, value_oid.id });

            auto tokens = tokenize(value_string);
            std::unordered_map<std::string, uint64_t> processed_tokens;

            for (auto& token : tokens) {
                auto normalized = normalize(token);

                auto pair = processed_tokens.emplace(normalized, 1);
                if (!pair.second) {
                    // Increment the count if it was previously inserted
                    pair.first->second++;
                }
            }

            for (auto& pair : processed_tokens) {
                auto& token = pair.first;
                auto count = pair.second;


                auto trie_node_id = trie.insert_string(token);

                // std::cout << "Insert into BPT: \"" << normalized;
                // std::cout << "\", trie_node_id: " << trie_node_id;
                // std::cout << ", table_pointer: " << table_pointer << "\n";

                constexpr double M = static_cast<double>(UINT64_MAX);
                uint64_t score = M - M * (double(count) / double(tokens.size()));

                // std::cout << "  Token \"" << token << "\", Score \"" << score << "\"\n";

                auto [first, second] = compress( trie_node_id, score, table_pointer);
                bpt.insert({ first, second });
            }
        }
    }
}


std::string oid_to_string(ObjectId oid) {
    return MQL::Conversions::to_lexical_str(oid);
}


}} // namespace TextSearch::RDF
