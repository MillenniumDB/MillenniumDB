#pragma once

#include <memory>

#include "query/executor/query_executor/mql/return_type.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/parser/op/mql/ops.h"

namespace MQL {
class ExecutorConstructor : public OpVisitor {
private:
    ReturnType ret;

public:
    ExecutorConstructor(ReturnType ret) :
        ret(ret)
    { }

    std::unique_ptr<QueryExecutor> executor;

    // possible Logical Plan roots
    void visit(OpDescribe&) override;
    void visit(OpReturn&) override;
    void visit(OpShow&) override;

    // Impossible Logical Plan roots
    void visit(OpUpdate&) override { }
    void visit(OpBasicGraphPattern&) override { }
    void visit(OpCall&) override { }
    void visit(OpCreateHNSWIndex&) override { }
    void visit(OpCreateTextIndex&) override { }
    void visit(OpEdge&) override { }
    void visit(OpGroupBy&) override { }
    void visit(OpInsert&) override { }
    void visit(OpDisjointTerm&) override { }
    void visit(OpDisjointVar&) override { }
    void visit(OpLabel&) override { }
    void visit(OpLet&) override { }
    void visit(OpOptional&) override { }
    void visit(OpOrderBy&) override { }
    void visit(OpPath&) override { }
    void visit(OpProperty&) override { }
    void visit(OpSequence&) override { }
    void visit(OpWhere&) override { }
};
} // namespace MQL
