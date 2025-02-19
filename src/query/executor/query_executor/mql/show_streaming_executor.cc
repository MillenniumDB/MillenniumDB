#include "show_streaming_executor.h"

#include "graph_models/quad_model/quad_model.h"
#include "storage/index/tensor_store/tensor_store.h"
#include "storage/index/tensor_store/tensor_store_manager.h"
#include "storage/index/text_search/text_search_index_manager.h"

using namespace MQL;

template<OpShow::Type type>
ShowStreamingExecutor<type>::ShowStreamingExecutor()
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
        throw std::runtime_error("Invalid Show::Type");
    }
}

template<OpShow::Type type>
const std::vector<VarId>& ShowStreamingExecutor<type>::get_projection_vars() const
{
    return projection_vars;
}

template<OpShow::Type type>
uint64_t ShowStreamingExecutor<type>::execute(MDBServer::StreamingResponseWriter& response_writer)
{
    uint64_t res { 0 };

    if constexpr (type == OpShow::Type::TENSOR_STORE) {
        assert(projection_vars.size() == 3);
        auto write_record = [&](const std::string& name, uint64_t tensors_dim, uint64_t num_entries) {
            response_writer.write_map_header(2UL);
            response_writer.write_string("type", MDBServer::Protocol::DataType::STRING);
            response_writer.write_uint8(static_cast<uint8_t>(MDBServer::Protocol::ResponseType::RECORD));

            response_writer.write_string("payload", MDBServer::Protocol::DataType::STRING);
            response_writer.write_list_header(projection_vars.size());
            response_writer.write_string(name, MDBServer::Protocol::DataType::STRING);
            response_writer.write_uint64(tensors_dim);
            response_writer.write_uint64(num_entries);
            response_writer.seal();
        };

        auto& tensor_store_manager = quad_model.catalog.tensor_store_manager;
        const auto& name2metadata = tensor_store_manager.get_name2metadata();
        res = name2metadata.size();

        for (const auto& [name, metadata] : name2metadata) {
            TensorStore* tensor_store;
            [[maybe_unused]] const bool found = tensor_store_manager.get_tensor_store(name, &tensor_store);
            assert(found && "TensorStore not found");
            const auto tensors_dim = tensor_store->tensors_dim();
            const auto num_entries = tensor_store->size();
            write_record(name, tensors_dim, num_entries);
        }
    } else if constexpr (type == OpShow::Type::TEXT_SEARCH_INDEX) {
        assert(projection_vars.size() == 4);
        using TSIMetadata = TextSearch::TextSearchIndexManager::TextSearchIndexMetadata;

        auto write_record = [&](const std::string& name, const TSIMetadata& metadata) {
            response_writer.write_map_header(2UL);
            response_writer.write_string("type", MDBServer::Protocol::DataType::STRING);
            response_writer.write_uint8(static_cast<uint8_t>(MDBServer::Protocol::ResponseType::RECORD));

            response_writer.write_string("payload", MDBServer::Protocol::DataType::STRING);
            response_writer.write_list_header(projection_vars.size());
            response_writer.write_string(name, MDBServer::Protocol::DataType::STRING);
            response_writer.write_string(metadata.predicate, MDBServer::Protocol::DataType::STRING);
            response_writer.write_string(
                to_string(metadata.normalization_type),
                MDBServer::Protocol::DataType::STRING
            );
            response_writer.write_string(
                to_string(metadata.tokenization_type),
                MDBServer::Protocol::DataType::STRING
            );

            response_writer.seal();
        };

        const auto& name2metadata = quad_model.catalog.text_search_index_manager.get_name2metadata();
        res = name2metadata.size();

        for (const auto& [name, metadata] : name2metadata) {
            write_record(name, metadata);
        }
    } else {
        throw std::runtime_error("Invalid Show::Type");
    }

    return res;
}

template<OpShow::Type type>
void ShowStreamingExecutor<type>::analyze(std::ostream& os, bool /*print_stats*/, int indent) const
{
    os << std::string(indent, ' ');
    os << "ShowStreamingExecutor(";
    os << OpShow::get_type_string(type);
    os << ")\n";
}

template class MQL::ShowStreamingExecutor<MQL::OpShow::Type::TENSOR_STORE>;
template class MQL::ShowStreamingExecutor<MQL::OpShow::Type::TEXT_SEARCH_INDEX>;