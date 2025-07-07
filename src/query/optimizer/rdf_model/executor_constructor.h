#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include "query/exceptions.h"
#include "query/query_context.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/parser/op/sparql/op_visitor.h"
#include "query/parser/op/sparql/ops.h"
#include "network/sparql/response_type.h"

namespace SPARQL {

class ExecutorConstructor : public OpVisitor {
public:
    std::unique_ptr<QueryExecutor> executor;

    ExecutorConstructor(SPARQL::ResponseType response_type) :
        response_type (response_type) { }

    void visit(OpSelect&) override;
    void visit(OpAsk&) override;
    void visit(OpDescribe&) override;
    void visit(OpConstruct&) override;
    void visit(OpShow&) override;

private:
    SPARQL::ResponseType response_type;
};
} // namespace SPARQL
