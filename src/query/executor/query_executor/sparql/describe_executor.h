#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/id.h"
#include "query/executor/query_executor/query_executor.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace SPARQL {

class DescribeExecutor : public QueryExecutor {
public:
    DescribeExecutor(
        std::unique_ptr<BindingIter> child_iter,
        std::vector<VarId>           vars,
        std::vector<ObjectId>        iris);

    uint64_t execute(std::ostream&) override;

    void analyze(std::ostream&, bool print_stats = false, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> child_iter;

    std::vector<VarId> vars;

    std::vector<ObjectId> iris;

    DistinctBindingHash extendable_table;

    std::vector<ObjectId> current_triple;

    uint64_t triple_count;

    uint64_t output_triples(std::ostream& os, Id subject, Id predicate, Id object);
    bool current_triple_distinct();
};
} // namespace SPARQL
