#include "streaming_executor_constructor.h"

#include "query/executor/query_executor/gql/return_streaming_executor.h"
#include "query/optimizer/property_graph_model/binding_list_iter_constructor.h"
#include "query/parser/op/gql/op_return.h"
#include "system/path_manager.h"

using namespace GQL;

void StreamingExecutorConstructor::visit(OpReturn& op_return)
{
    PathBindingIterConstructor visitor;
    op_return.accept_visitor(visitor);

    std::unique_ptr<BindingIter> binding_iter = std::move(visitor.tmp_iter);

    std::vector<VarId> projection_vars;

    for (auto& var : op_return.get_expr_vars()) {
        projection_vars.push_back(var);
    }

    // path_manager.begin(std::move(visitor.begin_at_left));

    executor = std::make_unique<ReturnStreamingExecutor>(std::move(binding_iter), std::move(projection_vars));
}

void StreamingExecutorConstructor::visit(OpGraphPattern&) { }

void StreamingExecutorConstructor::visit(OpBasicGraphPattern&) { }

void StreamingExecutorConstructor::visit(OpGraphPatternList&) { }

void StreamingExecutorConstructor::visit(OpNode&) { }

void StreamingExecutorConstructor::visit(OpEdge&) { }

void StreamingExecutorConstructor::visit(OpFilter&) { }

void StreamingExecutorConstructor::visit(OpPathUnion&) { }

void StreamingExecutorConstructor::visit(OpRepetition&) { }

void StreamingExecutorConstructor::visit(OpOrderBy&) { }

void StreamingExecutorConstructor::visit(OpLinearPattern&) { }

void StreamingExecutorConstructor::visit(OpFilterStatement&) { }

void StreamingExecutorConstructor::visit(OpLet&) { }

void StreamingExecutorConstructor::visit(OpOrderByStatement&) { }

void StreamingExecutorConstructor::visit(OpQueryStatements&) { }

void StreamingExecutorConstructor::visit(OpGroupBy&) { }
