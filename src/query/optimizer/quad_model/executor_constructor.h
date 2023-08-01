#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "graph_models/object_id.h"
#include "query/var_id.h"
#include "query/query_context.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/parser/op/mql/ops.h"

namespace MQL {
class ExecutorConstructor : public OpVisitor {
public:
    std::unique_ptr<QueryExecutor> executor;

    // Contains mandatory equalities of variables with constants
    // obtained from SET statement
    std::map<VarId, ObjectId> set_vars;

    // // properties used in RETURN and ORDER BY. We need to remember them to add optional children in the OpMatch
    // std::set<std::pair<Var, std::string>> var_properties;

    // std::vector<VarId> projection_vars;

    // std::vector<VarId> group_vars;

    // std::set<VarId> group_saved_vars;

    // // Aggregates from RETURN and ORDER BY
    // std::map<VarId, std::unique_ptr<Agg>> aggs;

    // // Contains mandatory equalities of properties with constants (to push them from WHERE into the Binding phase).
    // std::vector<std::tuple<Var, std::string, QueryElement>> where_properties;

    // // When true, DistinctHash will be applied in visit(OpMatch&) to remove duplicates
    // bool distinct_into_id = false;

    // bool need_materialize_paths = false;

    // bool distinct_ordered_possible = false;

    // // True if query contains a group by
    // bool group = false;

    // possible Logical Plan roots
    void visit(OpDescribe&) override;
    void visit(OpReturn&)   override;
    void visit(OpSet&)      override;
};
} // namespace MQL
