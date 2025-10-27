#include "select_streaming_executor.h"

using namespace SPARQL;

SelectStreamingExecutor::SelectStreamingExecutor(
    std::unique_ptr<BindingIter> iter_,
    std::vector<VarId>&& projection_vars_
) :
    iter (std::move(iter_))
{
    projection_vars = std::move(projection_vars_);
}

uint64_t SelectStreamingExecutor::execute(MDBServer::StreamingResponseWriter& response_writer)
{
    Binding binding { get_query_ctx().get_var_size() };

    uint64_t result_count = 0;
    iter->begin(binding);
    while (iter->next()) {
        ++result_count;
        response_writer.write_record(projection_vars, binding);
    }
    return result_count;
}

void SelectStreamingExecutor::analyze(std::ostream& os, bool print_stats, int indent) const
{
    os << std::string(indent, ' ') << "SelectStreamingExecutor()\n";
    iter->print(os, indent + 2, print_stats);
}
