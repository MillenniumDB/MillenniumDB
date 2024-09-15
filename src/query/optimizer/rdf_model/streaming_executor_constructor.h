#pragma once

#include <memory>

#include "query/parser/op/op_visitor.h"


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
};
} // namespace MQL
