#include "streaming_executor_constructor.h"

#include "query/exceptions.h"
#include "query/executor/query_executor/sparql/show_streaming_executor.h"
#include "system/path_manager.h"
#include "query/executor/query_executor/sparql/ask_streaming_executor.h"
#include "query/executor/query_executor/sparql/select_streaming_executor.h"
#include "query/optimizer/rdf_model/binding_iter_constructor.h"
#include "query/parser/op/sparql/ops.h"

using namespace SPARQL;

void StreamingExecutorConstructor::visit(OpSelect& op_select) {
    BindingIterConstructor visitor;
    op_select.accept_visitor(visitor);

    std::vector<VarId> projection_vars = op_select.vars;

    path_manager.begin(std::move(visitor.begin_at_left));

    executor = std::make_unique<SelectStreamingExecutor>(std::move(visitor.tmp), std::move(projection_vars));
}


void StreamingExecutorConstructor::visit(OpAsk& op_ask) {
    BindingIterConstructor visitor;
    op_ask.op->accept_visitor(visitor);

    std::unique_ptr<BindingIter> binding_iter = std::move(visitor.tmp);

    path_manager.begin(std::move(visitor.begin_at_left));

    executor = std::make_unique<AskStreamingExecutor>(std::move(binding_iter));
}


void StreamingExecutorConstructor::visit(OpShow& op_show) {
    switch (op_show.type) {
    case OpShow::Type::TEXT_SEARCH_INDEX:
        executor = std::make_unique<ShowStreamingExecutor<OpShow::Type::TEXT_SEARCH_INDEX>>();
        break;
    default:
        throw NotSupportedException("SPARQL::StreamingExecutorConstructor::visit(OpShow&): Unhandled SHOW");
    }
}

void StreamingExecutorConstructor::visit(OpDescribe&) {
    // TODO: Implement this
    throw NotSupportedException("SPARQL::StreamingExecutorConstructor::visit(OpDescribe&) not implemented");
}


void StreamingExecutorConstructor::visit(OpConstruct&) {
    // TODO: Implement this
    throw NotSupportedException("SPARQL::StreamingExecutorConstructor::visit(OpConstruct&) not implemented");
}

