#include "show_executor.h"

#include "graph_models/rdf_model/rdf_catalog.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "storage/index/text_search/text_index_manager.h"

#include <stdexcept>

using namespace SPARQL;

template<ResponseType ret, OpShow::Type type>
ShowExecutor<ret, type>::ShowExecutor()
{
    if constexpr (type == OpShow::Type::HNSW_INDEX) {
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("name"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("property"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("metric"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("dimension"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("max_edges"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("max_candidates"));
    } else if constexpr (type == OpShow::Type::TEXT_INDEX) {
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("name"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("property"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("normalization"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("tokenization"));
    } else {
        throw std::runtime_error("Unhandled Show::Type");
    }
}

template<ResponseType ret, OpShow::Type type>
uint64_t ShowExecutor<ret, type>::execute(std::ostream& os)
{
    constexpr char delim = ret == ResponseType::CSV ? ',' : '\t';

    uint64_t res { 0 };

    // Header
    auto it = projection_vars.cbegin();
    if (it != projection_vars.cend()) {
        os << get_query_ctx().get_var_name(*it);
        ++it;
    }

    while (it != projection_vars.cend()) {
        if constexpr (ret == ResponseType::CSV) {
            os << ',';
        } else {
            static_assert(ret == ResponseType::TSV);
            os << '\t';
        }
        os << get_query_ctx().get_var_name(*it);
        ++it;
    }
    os << '\n';

    if constexpr (type == OpShow::Type::HNSW_INDEX) {
        assert(projection_vars.size() == 6);
        auto& hnsw_index_manager = rdf_model.catalog.hnsw_index_manager;
        const auto name2metadata = hnsw_index_manager.get_name2metadata();
        for (const auto& [name, metadata] : name2metadata) {
            auto* hnsw_index_ptr = hnsw_index_manager.get_hnsw_index(name);
            auto& params = hnsw_index_ptr->get_params();
            os << '"' << name << '"';
            os << delim;
            os << '<' << metadata.predicate << '>';
            os << delim;
            os << '"' << metadata.metric_type << '"';
            os << delim;
            os << params.dimensions;
            os << delim;
            os << params.M;
            os << delim;
            os << params.ef_construction;
            os << '\n';
        }
    } else if constexpr (type == OpShow::Type::TEXT_INDEX) {
        assert(projection_vars.size() == 4);
        const auto name2metadata = rdf_model.catalog.text_index_manager.get_name2metadata();
        res = name2metadata.size();
        for (const auto& [name, metadata] : name2metadata) {
            os << '"' << name << '"';
            os << delim;
            os << '<' << metadata.predicate << '>';
            os << delim;
            os << '"' << metadata.normalization_type << '"';
            os << delim;
            os << '"' << metadata.tokenization_type << '"';
            os << '\n';
        }
    } else {
        throw std::runtime_error("Unhandled Show::Type");
    }

    return res;
}

template<ResponseType res, OpShow::Type type>
void ShowExecutor<res, type>::analyze(std::ostream& os, bool /*print_stats*/, int indent) const
{
    os << std::string(indent, ' ');
    os << "ShowExecutor(";
    os << OpShow::get_type_string(type);
    os << ")\n";
}

template class SPARQL::ShowExecutor<ResponseType::CSV, OpShow::Type::HNSW_INDEX>;
template class SPARQL::ShowExecutor<ResponseType::CSV, OpShow::Type::TEXT_INDEX>;

template class SPARQL::ShowExecutor<ResponseType::TSV, OpShow::Type::HNSW_INDEX>;
template class SPARQL::ShowExecutor<ResponseType::TSV, OpShow::Type::TEXT_INDEX>;
