#include "streaming_executor_constructor.h"

#include "query/executor/query_executor/gql/return_streaming_executor.h"
#include "query/optimizer/property_graph_model/binding_iter_constructor.h"
#include "query/parser/op/gql/op_return.h"
#include "system/path_manager.h"

using namespace GQL;

void GQL::StreamingExecutorConstructor::visit(OpReturn& op_return)
{
    BindingIterConstructor visitor;
    op_return.op->accept_visitor(visitor);

    std::unique_ptr<BindingIter> binding_iter = std::move(visitor.tmp);

    std::vector<VarId> projection_vars;

    for (auto& var : op_return.get_expr_vars()) {
        projection_vars.push_back(var);
    }

    path_manager.begin(std::move(visitor.begin_at_left));

    executor = std::make_unique<ReturnStreamingExecutor>(std::move(binding_iter), std::move(projection_vars));
}
