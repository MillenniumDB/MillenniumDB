#include "show_streaming_executor.h"

#include "graph_models/rdf_model/rdf_catalog.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "storage/index/text_search/text_index_manager.h"

using namespace SPARQL;

template<OpShow::Type type>
ShowStreamingExecutor<type>::ShowStreamingExecutor()
{
    if constexpr (type == OpShow::Type::HNSW_INDEX) {
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("name"));
        projection_vars.emplace_back(get_query_ctx().get_or_create_var("predicate"));
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

template<OpShow::Type type>
const std::vector<VarId>& ShowStreamingExecutor<type>::get_projection_vars() const
{
    return projection_vars;
}

template<OpShow::Type type>
uint64_t ShowStreamingExecutor<type>::execute(MDBServer::StreamingResponseWriter& response_writer)
{
    uint64_t res { 0 };

    if constexpr (type == OpShow::Type::HNSW_INDEX) {
        assert(projection_vars.size() == 6);
        using HIMeta = HNSW::HNSWIndexManager::HNSWIndexMetadata;

        auto write_record = [&](const std::string& name,
                                const HIMeta& metadata,
                                const HNSW::HNSWIndex::HNSWIndexParams& params) {
            response_writer.write_map_header(2UL);
            response_writer.write_string("type", MDBServer::Protocol::DataType::STRING);
            response_writer.write_uint8(static_cast<uint8_t>(MDBServer::Protocol::ResponseType::RECORD));

            response_writer.write_string("payload", MDBServer::Protocol::DataType::STRING);
            response_writer.write_list_header(projection_vars.size());
            response_writer.write_string(name, MDBServer::Protocol::DataType::STRING);
            response_writer.write_string(metadata.predicate, MDBServer::Protocol::DataType::IRI);
            response_writer.write_string(
                to_string(metadata.metric_type),
                MDBServer::Protocol::DataType::STRING
            );
            response_writer.write_uint64(params.dimensions);
            response_writer.write_uint64(params.M);
            response_writer.write_uint64(params.ef_construction);

            response_writer.seal();
        };

        auto& hnsw_index_manager = rdf_model.catalog.hnsw_index_manager;
        const auto name2metadata = hnsw_index_manager.get_name2metadata();
        res = name2metadata.size();

        for (const auto& [name, metadata] : name2metadata) {
            auto* hnsw_index_ptr = hnsw_index_manager.get_hnsw_index(name);
            auto& params = hnsw_index_ptr->get_params();
            write_record(name, metadata, params);
        }
    } else if constexpr (type == OpShow::Type::TEXT_INDEX) {
        assert(projection_vars.size() == 4);
        using TIMeta = TextSearch::TextIndexManager::TextIndexMetadata;

        auto write_record = [&](const std::string& name, const TIMeta& metadata) {
            response_writer.write_map_header(2UL);
            response_writer.write_string("type", MDBServer::Protocol::DataType::STRING);
            response_writer.write_uint8(static_cast<uint8_t>(MDBServer::Protocol::ResponseType::RECORD));

            response_writer.write_string("payload", MDBServer::Protocol::DataType::STRING);
            response_writer.write_list_header(projection_vars.size());
            response_writer.write_string(name, MDBServer::Protocol::DataType::STRING);
            response_writer.write_string(metadata.predicate, MDBServer::Protocol::DataType::IRI);
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

        auto& text_index_manager = rdf_model.catalog.text_index_manager;
        const auto name2metadata = text_index_manager.get_name2metadata();
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

template class SPARQL::ShowStreamingExecutor<OpShow::Type::TEXT_INDEX>;
template class SPARQL::ShowStreamingExecutor<OpShow::Type::HNSW_INDEX>;
