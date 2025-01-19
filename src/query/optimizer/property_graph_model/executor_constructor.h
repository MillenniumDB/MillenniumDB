#pragma once

#include <memory>

#include "query/executor/query_executor/gql/return_executor.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/parser/op/op_visitor.h"

namespace GQL {

class ExecutorConstructor : public OpVisitor {
public:
    std::unique_ptr<QueryExecutor> executor;

    ExecutorConstructor(ReturnType return_type) :
        return_type(return_type)
    { }

    void visit(GQL::OpReturn&) override;
    void visit(GQL::OpGraphPattern&) override;
    void visit(GQL::OpBasicGraphPattern&) override;
    void visit(GQL::OpGraphPatternList&) override;
    void visit(GQL::OpFilter&) override;

private:
    ReturnType return_type;
};
} // namespace GQL
