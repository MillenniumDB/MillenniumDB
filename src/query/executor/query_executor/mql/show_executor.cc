#include "show_executor.h"

#include "graph_models/quad_model/quad_model.h"
#include "storage/index/tensor_store/tensor_store.h"
#include "storage/index/tensor_store/tensor_store_manager.h"
#include "storage/index/text_search/text_search_index_manager.h"

using namespace MQL;

template<ReturnType ret, OpShow::Type type>
ShowExecutor<ret, type>::ShowExecutor()
{
    if constexpr (type == OpShow::Type::TENSOR_STORE) {
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("name"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("tensors_dim"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("num_entries"));
    } else if constexpr (type == OpShow::Type::TEXT_SEARCH_INDEX) {
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("name"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("predicate"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("normalization"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("tokenization"));
    } else {
        throw std::runtime_error("Unhandled Show::Type");
    }
}

template<ReturnType ret, OpShow::Type type>
uint64_t ShowExecutor<ret, type>::execute(std::ostream& os)
{
    constexpr char delim = ret == ReturnType::CSV ? ',' : '\t';

    uint64_t res { 0 };

    // Header
    auto it = projection_vars.cbegin();
    if (it != projection_vars.cend()) {
        os << get_query_ctx().get_var_name(*it);
        ++it;
    }

    while (it != projection_vars.cend()) {
        os << delim;
        os << get_query_ctx().get_var_name(*it);
        ++it;
    }
    os << '\n';

    if constexpr (type == OpShow::Type::TENSOR_STORE) {
        assert(projection_vars.size() == 3);
        auto& tensor_store_manager = quad_model.catalog.tensor_store_manager;
        const auto& name2metadata = tensor_store_manager.get_name2metadata();
        res = name2metadata.size();
        for (const auto& [name, metadata] : name2metadata) {
            TensorStore* tensor_store;
            [[maybe_unused]] const bool found = tensor_store_manager.get_tensor_store(name, &tensor_store);
            assert(found && "TensorStore not found");
            os << '"' << name << '"';
            os << delim;
            os << tensor_store->tensors_dim();
            os << delim;
            os << tensor_store->size();
            os << '\n';
        }
    } else if constexpr (type == OpShow::Type::TEXT_SEARCH_INDEX) {
        assert(projection_vars.size() == 4);
        const auto& name2metadata = quad_model.catalog.text_search_index_manager.get_name2metadata();
        res = name2metadata.size();
        for (const auto& [name, metadata] : name2metadata) {
            os << '"' << name << '"';
            os << delim;
            os << '"' << metadata.predicate << '"';
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

template<ReturnType ret, OpShow::Type type>
void ShowExecutor<ret, type>::analyze(std::ostream& os, bool /*print_stats*/, int indent) const
{
    os << std::string(indent, ' ');
    os << "ShowExecutor(";
    os << OpShow::get_type_string(type);
    os << ")\n";
}

template class MQL::ShowExecutor<MQL::ReturnType::CSV, MQL::OpShow::Type::TENSOR_STORE>;
template class MQL::ShowExecutor<MQL::ReturnType::TSV, MQL::OpShow::Type::TENSOR_STORE>;
template class MQL::ShowExecutor<MQL::ReturnType::CSV, MQL::OpShow::Type::TEXT_SEARCH_INDEX>;
template class MQL::ShowExecutor<MQL::ReturnType::TSV, MQL::OpShow::Type::TEXT_SEARCH_INDEX>;
