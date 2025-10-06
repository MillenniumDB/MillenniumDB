#pragma once

#include <memory>

#include "query/parser/op/mql/op_visitor.h"

class StreamingQueryExecutor;

namespace MQL {

class StreamingExecutorConstructor : public OpVisitor {
public:
    std::unique_ptr<StreamingQueryExecutor> executor;

    // Possible Logical Plan roots
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
