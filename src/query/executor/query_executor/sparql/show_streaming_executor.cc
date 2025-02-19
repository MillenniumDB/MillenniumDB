#include "show_streaming_executor.h"

#include "graph_models/rdf_model/rdf_catalog.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "storage/index/text_search/text_search_index_manager.h"

using namespace SPARQL;

template<OpShow::Type type>
ShowStreamingExecutor<type>::ShowStreamingExecutor()
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

template<OpShow::Type type>
const std::vector<VarId>& ShowStreamingExecutor<type>::get_projection_vars() const
{
    return projection_vars;
}

template<OpShow::Type type>
uint64_t ShowStreamingExecutor<type>::execute(MDBServer::StreamingResponseWriter& response_writer)
{
    uint64_t res { 0 };

    if constexpr (type == OpShow::Type::TEXT_SEARCH_INDEX) {
        assert(projection_vars.size() == 4);
        using TSIMetadata = TextSearch::TextSearchIndexManager::TextSearchIndexMetadata;

        auto write_record = [&](const std::string& name, const TSIMetadata& metadata) {
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

        const auto& name2metadata = rdf_model.catalog.text_search_index_manager.get_name2metadata();
        res = name2metadata.size();

        for (const auto& [name, metadata] : name2metadata) {
            write_record(name, metadata);
        }
    } else {
        throw std::runtime_error("Unhandled Show::Type");
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

template class SPARQL::ShowStreamingExecutor<SPARQL::OpShow::Type::TEXT_SEARCH_INDEX>;