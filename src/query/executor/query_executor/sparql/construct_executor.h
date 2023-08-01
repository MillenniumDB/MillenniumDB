#pragma once

#include <cstdint>
#include <memory>
#include <variant>

#include "query/executor/binding_iter.h"
#include "graph_models/object_id.h"
#include "query/executor/query_executor/query_executor.h"
#include "network/sparql/response_type.h"
#include "query/optimizer/rdf_model/plan/triple_plan.h"
#include "storage/index/hash/distinct_binding_hash/distinct_binding_hash.h"

namespace SPARQL {

struct Triple {
    Id subject;
    Id predicate;
    Id object;

    Triple(Id subject, Id predicate, Id object) : subject(subject), predicate(predicate), object(object) { }
};

enum class Idx {subject=0, predicate=1, object=2};

typedef std::unordered_map<std::string, ObjectId> BlankNodeMap;

class ConstructExecutor : public QueryExecutor {
public:
    ConstructExecutor(
        std::unique_ptr<BindingIter> child_iter,
        std::vector<Triple>            triples);

    uint64_t execute(std::ostream&) override;

    void analyze(std::ostream&, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> child_iter;
    std::unique_ptr<Binding>     child_binding;

    std::vector<Triple> triples;

    DistinctBindingHash<ObjectId> extendable_table;
    std::vector<ObjectId>         current_triple;

    uint64_t triple_count = 0;

    bool current_triple_distinct();
    bool process_element(Id id, Idx idx, BlankNodeMap& blank_nodes);
    uint64_t output_triple(std::ostream& os, BlankNodeMap blank_nodes, Binding& binding, Triple triple);
};
} // namespace SPARQL
