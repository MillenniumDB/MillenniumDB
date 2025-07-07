#pragma once

#include <map>
#include <memory>

#include "graph_models/object_id.h"
#include "query/parser/op/mql/op_visitor.h"
#include "query/var_id.h"

class StreamingQueryExecutor;

namespace MQL {

class StreamingExecutorConstructor : public OpVisitor {
public:
    std::unique_ptr<StreamingQueryExecutor> executor;

    // Possible Logical Plan roots
    void visit(OpDescribe&) override;
    void visit(OpReturn&) override;
    void visit(OpShow&) override;
};
} // namespace MQL
