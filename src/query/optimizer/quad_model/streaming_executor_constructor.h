#pragma once

#include <map>
#include <memory>

#include "graph_models/object_id.h"
#include "query/parser/op/op_visitor.h"
#include "query/var_id.h"

class StreamingQueryExecutor;

namespace MQL {

class StreamingExecutorConstructor : public OpVisitor {
public:
    std::unique_ptr<StreamingQueryExecutor> executor;

    // Contains mandatory equalities of variables with constants
    // obtained from SET statement
    std::map<VarId, ObjectId> set_vars;

    // Possible Logical Plan roots
    void visit(OpDescribe&) override;
    void visit(OpReturn&) override;
    void visit(OpSet&) override;
    void visit(OpShow&) override;
};
} // namespace MQL
