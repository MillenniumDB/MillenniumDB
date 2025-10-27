#include "return_streaming_executor.h"

using namespace GQL;

ReturnStreamingExecutor::ReturnStreamingExecutor(
    std::unique_ptr<BindingIter> iter,
    std::vector<VarId>&& _projection_vars
) :
    iter(std::move(iter))
{
    projection_vars = std::move(_projection_vars);
}

uint64_t ReturnStreamingExecutor::execute(MDBServer::StreamingResponseWriter& response_writer)
{
    uint64_t result_count = 0;

    Binding binding { get_query_ctx().get_var_size() };

    iter->begin(binding);
    while (iter->next()) {
        ++result_count;
        response_writer.write_record(projection_vars, binding);
    }

    return result_count;
}

void ReturnStreamingExecutor::analyze(std::ostream& os, bool print_stats, int indent) const
{
    os << std::string(indent, ' ') << "SelectStreamingExecutor()\n";
    iter->print(os, indent + 2, print_stats);
}
