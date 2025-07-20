#pragma once

#include <memory>

#include "query/parser/op/sparql/op_visitor.h"


class StreamingQueryExecutor;

namespace SPARQL {

class StreamingExecutorConstructor : public OpVisitor {
public:
    std::unique_ptr<StreamingQueryExecutor> executor;

    // Possible Logical Plan roots
    void visit(OpSelect&) override;
    void visit(OpAsk&) override;
    void visit(OpDescribe&) override;
    void visit(OpConstruct&) override;
    void visit(OpShow&) override;

    // Impossible Logical Plan roots
    void visit(OpUpdate&) override { }
    void visit(OpInsertData&) override { }
    void visit(OpDeleteData&) override { }
    void visit(OpCreateTextIndex&) override { }
    void visit(OpCreateHNSWIndex&) override { }
    void visit(OpOptional&) override { }
    void visit(OpOrderBy&) override { }
    void visit(OpGroupBy&) override { }
    void visit(OpHaving&) override { }
    void visit(OpBasicGraphPattern&) override { }
    void visit(OpTriple&) override { }
    void visit(OpPath&) override { }
    void visit(OpProcedure&) override { }
    void visit(OpUnitTable&) override { }
    void visit(OpJoin&) override { }
    void visit(OpSemiJoin&) override { }
    void visit(OpUnion&) override { }
    void visit(OpMinus&) override { }
    void visit(OpNotExists&) override { }
    void visit(OpSequence&) override { }
    void visit(OpService&) override { }
    void visit(OpFrom&) override { }
    void visit(OpGraph&) override { }
    void visit(OpFilter&) override { }
    void visit(OpEmpty&) override { }
    void visit(OpBind&) override { }
    void visit(OpValues&) override { }
};
} // namespace MQL
