#include "rdf.h"

#include "graph_models/rdf_model/conversions.h"
#include "graph_models/rdf_model/rdf_object_id.h"
#include "query/optimizer/rdf_model/plan/triple_plan.h"
#include "query/query_context.h"
#include "storage/index/text_search/utils.h"


namespace TextSearch { namespace RDF {


void index_predicate(Trie&          trie,
                     BPlusTree<2>&  bpt,
                     Table&         table,
                     TokenizeFunc*  tokenize,
                     NormalizeFunc* normalize,
                     std::string    predicate)
{
    auto subject_var   = get_query_ctx().get_internal_var();
    auto predicate_val = SPARQL::Conversions::pack_iri(predicate);
    auto object_var    = get_query_ctx().get_internal_var();

    auto triple_plan  = SPARQL::TriplePlan(subject_var, predicate_val, object_var);
    auto binding_iter = triple_plan.get_binding_iter();

    Binding binding(get_query_ctx().get_var_size());
    binding_iter->begin(binding);

    while (binding_iter->next()) {
        auto subject_oid = binding[subject_var];
        auto object_oid  = binding[object_var];

        auto gen_t = RDF_OID::get_generic_type(object_oid);
        if (gen_t == RDF_OID::GenericType::STRING) {
            auto string = SPARQL::Conversions::to_lexical_str(object_oid);
            // std::cout << "Indexing \"" << string << "\"\n";

            auto table_pointer = table.insert({ subject_oid.id, object_oid.id });

            auto tokens = tokenize(string);
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
    return SPARQL::Conversions::to_lexical_str(oid);
}


}} // namespace TextSearch::RDF
