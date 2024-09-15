#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/executor/query_executor/query_executor.h"

namespace SPARQL {

class TSVSelectExecutor : public QueryExecutor {
public:
    TSVSelectExecutor(
        std::unique_ptr<BindingIter> root,
        std::vector<VarId>           projection_vars
    ) :
        root            (std::move(root)),
        projection_vars (std::move(projection_vars)) { }

    uint64_t execute(std::ostream&) override;

    void analyze(std::ostream&, bool print_stats = false, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> root;
    std::unique_ptr<Binding>     binding;
    std::vector<VarId>           projection_vars;

    uint64_t execute_empty_binding(std::ostream&);

    static void print(std::ostream& os, std::ostream& escaped_os, ObjectId);

    static void print_path_node(std::ostream& os, ObjectId node_id);

    static void print_path_edge(std::ostream& os, ObjectId edge_id, bool inverse);
};
} // namespace SPARQL
