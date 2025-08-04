#pragma once

#include <memory>

#include "query/parser/op//gql/op_visitor.h"

class StreamingQueryExecutor;

namespace GQL {

class StreamingExecutorConstructor : public OpVisitor {
public:
    std::unique_ptr<StreamingQueryExecutor> executor;

    void visit(OpReturn&) override;

    void visit(OpGraphPattern&) override;
    void visit(OpBasicGraphPattern&) override;
    void visit(OpGraphPatternList&) override;
    void visit(OpNode&) override;
    void visit(OpEdge&) override;
    void visit(OpWhere&) override;
    void visit(OpPathUnion&) override;
    void visit(OpRepetition&) override;
    void visit(OpLinearPattern&) override;
    void visit(OpFilter&) override;
    void visit(OpLet&) override;
    void visit(OpOrderBy&) override;
    void visit(OpQueryStatements&) override;
    void visit(OpGroupBy&) override;
};
} // namespace GQL
