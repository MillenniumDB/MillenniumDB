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
    void visit(OpUpdate&) override;

    // Impossible Logical Plan roots
    void visit(OpBasicGraphPattern&) override { }
    void visit(OpCall&) override { }
    void visit(OpGroupBy&) override { }
    void visit(OpHaving&) override { }
    void visit(OpLet&) override { }
    void visit(OpOptional&) override { }
    void visit(OpOrderBy&) override { }
    void visit(OpSequence&) override { }
    void visit(OpUnitTable&) override { }
    void visit(OpWhere&) override { }
};
} // namespace MQL
