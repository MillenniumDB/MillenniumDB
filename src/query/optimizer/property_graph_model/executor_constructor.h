#pragma once

#include <memory>

#include "query/executor/query_executor/gql/return_executor.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/parser/op/gql/op_visitor.h"

namespace GQL {

class ExecutorConstructor : public OpVisitor {
public:
    std::unique_ptr<QueryExecutor> executor;

    ExecutorConstructor(ReturnType return_type) :
        return_type(return_type)
    { }

    void visit(OpReturn&) override;
    void visit(OpGraphPattern&) override;
    void visit(OpBasicGraphPattern&) override;
    void visit(OpGraphPatternList&) override;
    void visit(OpWhere&) override;

    void visit(OpNode&) override;
    void visit(OpEdge&) override;
    void visit(OpPathUnion&) override;
    void visit(OpRepetition&) override;
    void visit(OpLinearPattern&) override;
    void visit(OpFilter&) override;
    void visit(OpLet&) override;
    void visit(OpOrderBy&) override;
    void visit(OpQueryStatements&) override;
    void visit(OpGroupBy&) override;
    void visit(OpUnitTable&) override;
    void visit(OpEmpty&) override;

private:
    ReturnType return_type;
};
} // namespace GQL
