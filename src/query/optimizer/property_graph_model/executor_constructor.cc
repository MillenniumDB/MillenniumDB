#include "executor_constructor.h"
#include "query/executor/query_executor/gql/return_executor.h"
#include "query/optimizer/property_graph_model/binding_iter_constructor.h"
#include "query/parser/op/gql/op_return.h"
#include "system/path_manager.h"

using namespace GQL;

void ExecutorConstructor::visit(OpReturn& op_return)
{
    BindingIterConstructor visitor;
    op_return.accept_visitor(visitor);

    std::unique_ptr<BindingIter> binding_iter = std::move(visitor.tmp);

    std::vector<VarId> projection_vars;

    for (auto& var : op_return.get_expr_vars()) {
        projection_vars.push_back(var);
    }

    path_manager.begin(std::move(visitor.begin_at_left));

    executor = std::make_unique<ReturnExecutor>(
        std::move(binding_iter),
        std::move(projection_vars),
        return_type
    );
}

void ExecutorConstructor::visit(OpGraphPattern&) { }

void ExecutorConstructor::visit(OpGraphPatternList&) { }

void ExecutorConstructor::visit(OpFilter&) { }

void ExecutorConstructor::visit(OpBasicGraphPattern&) { }
