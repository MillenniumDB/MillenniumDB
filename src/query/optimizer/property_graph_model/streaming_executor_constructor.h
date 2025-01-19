#pragma once

#include <memory>

#include "query/parser/op/op_visitor.h"

class StreamingQueryExecutor;

namespace GQL {

class StreamingExecutorConstructor : public OpVisitor {
public:
    std::unique_ptr<StreamingQueryExecutor> executor;

    void visit(OpReturn&) override;
};
} // namespace GQL
