#include "ask_streaming_executor.h"

#include "query/executor/binding_iter_printer.h"

using namespace SPARQL;

AskStreamingExecutor::AskStreamingExecutor(std::unique_ptr<BindingIter> iter_) :
    iter { std::move(iter_) }, projection_vars { { get_query_ctx().get_or_create_var("askResult") } } { }


const std::vector<VarId>& AskStreamingExecutor::get_projection_vars() const {
    return projection_vars;
}


uint64_t AskStreamingExecutor::execute(MDBServer::StreamingResponseWriter& response_writer) {
    Binding binding { get_query_ctx().get_var_size() };
    iter->begin(binding);
    const bool has_result = iter->next();

    response_writer.write_map_header(2UL);
    response_writer.write_string("type", MDBServer::Protocol::DataType::STRING);
    response_writer.write_uint8(static_cast<uint8_t>(MDBServer::Protocol::ResponseType::RECORD));

    response_writer.write_string("payload", MDBServer::Protocol::DataType::STRING);
    response_writer.write_list_header(1);
    response_writer.write_bool(has_result);

    response_writer.seal();

    return has_result ? 1 : 0;
}


void AskStreamingExecutor::analyze(std::ostream& os, bool print_stats, int indent) const {
    os << std::string(indent, ' ') << "AskStreamingExecutor()";
    BindingIterPrinter printer(os, print_stats, indent + 2);
    iter->accept_visitor(printer);
}