#include "return_streaming_executor.h"

using namespace GQL;

ReturnStreamingExecutor::ReturnStreamingExecutor(
    std::unique_ptr<BindingIter> iter,
    std::vector<VarId>&& projection_vars
) :
    iter(std::move(iter)),
    projection_vars(projection_vars)
{ }

const std::vector<VarId>& ReturnStreamingExecutor::get_projection_vars() const
{
    return projection_vars;
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
