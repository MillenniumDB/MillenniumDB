#include "show_executor.h"

#include "graph_models/rdf_model/rdf_catalog.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "storage/index/text_search/text_search_index_manager.h"

#include <stdexcept>

using namespace SPARQL;

template<ResponseType res, OpShow::Type type>
ShowExecutor<res, type>::ShowExecutor()
{
    if constexpr (type == OpShow::Type::TEXT_SEARCH_INDEX) {
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("name"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("predicate"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("normalization"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("tokenization"));
    } else {
        throw std::runtime_error("Unhandled Show::Type");
    }
}

template<ResponseType res, OpShow::Type type>
uint64_t ShowExecutor<res, type>::execute(std::ostream& os)
{
    constexpr char delim = res == ResponseType::CSV ? ',' : '\t';

    uint64_t num_res { 0 };

    // Header
    auto it = projection_vars.cbegin();
    if (it != projection_vars.cend()) {
        os << get_query_ctx().get_var_name(*it);
        ++it;
    }

    while (it != projection_vars.cend()) {
        if constexpr (res == ResponseType::CSV) {
            os << ',';
        } else {
            static_assert(res == ResponseType::TSV);
            os << '\t';
        }
        os << get_query_ctx().get_var_name(*it);
        ++it;
    }
    os << '\n';

    if constexpr (type == OpShow::Type::TEXT_SEARCH_INDEX) {
        assert(projection_vars.size() == 4);
        const auto& name2metadata = rdf_model.catalog.text_search_index_manager.get_name2metadata();
        num_res = name2metadata.size();
        for (const auto& [name, metadata] : name2metadata) {
            os << '"' << name << '"';
            os << delim;
            os << '"' << metadata.predicate << '"';
            os << delim;
            os << '"' << to_string(metadata.normalization_type) << '"';
            os << delim;
            os << '"' << to_string(metadata.tokenization_type) << '"';
            os << '\n';
        }
    } else {
        throw std::runtime_error("Unhandled Show::Type");
    }

    return num_res;
}

template<ResponseType res, OpShow::Type type>
void ShowExecutor<res, type>::analyze(std::ostream& os, bool /*print_stats*/, int indent) const
{
    os << std::string(indent, ' ');
    os << "ShowExecutor(";
    os << OpShow::get_type_string(type);
    os << ")\n";
}

template class SPARQL::ShowExecutor<ResponseType::CSV, OpShow::Type::TEXT_SEARCH_INDEX>;
template class SPARQL::ShowExecutor<ResponseType::TSV, OpShow::Type::TEXT_SEARCH_INDEX>;