#include "return_streaming_executor.h"

#include "query/executor/binding_iter_printer.h"

using namespace MQL;


ReturnStreamingExecutor::ReturnStreamingExecutor(std::unique_ptr<BindingIter> iter_,
                                                 std::map<VarId, ObjectId>&&  set_vars_,
                                                 std::vector<VarId>&&         projection_vars_) :
    iter { std::move(iter_) }, set_vars { std::move(set_vars_) }, projection_vars { std::move(projection_vars_) } { }


const std::vector<VarId>& ReturnStreamingExecutor::get_projection_vars() const {
    return projection_vars;
}


uint64_t ReturnStreamingExecutor::execute(MDBServer::StreamingResponseWriter& response_writer) {
    uint64_t result_count = 0;

    Binding binding { get_query_ctx().get_var_size() };
    for (auto&& [var, value] : set_vars) {
        binding.add(var, value);
    }

    iter->begin(binding);
    while (iter->next()) {
        ++result_count;
        response_writer.write_record(projection_vars, binding);
    }

    return result_count;
}


void ReturnStreamingExecutor::analyze(std::ostream& os, bool print_stats, int indent) const {
    os << std::string(indent, ' ') << "ReturnStreamingExecutor()\n";
    BindingIterPrinter printer(os, print_stats, indent + 2);
    iter->accept_visitor(printer);
}
