#pragma once

#include <map>

#include "query/executor/binding_iter.h"
#include "query/executor/query_executor/query_executor.h"

namespace MQL {
class CSVReturnExecutor : public QueryExecutor {
public:
    CSVReturnExecutor(
        std::unique_ptr<BindingIter> iter,
        std::map<VarId, ObjectId>&& set_vars,
        std::vector<VarId>&& projection_vars,
        uint64_t limit
    ) :
        iter            (std::move(iter)),
        set_vars        (std::move(set_vars)),
        projection_vars (std::move(projection_vars)),
        limit           (limit) { }

    // returns how many results were obtained
    uint64_t execute(std::ostream&) override;

    void analyze(std::ostream&, int indent = 0) const override;

    static void print(std::ostream& os, ObjectId oid);


private:
    std::unique_ptr<BindingIter> iter;

    std::map<VarId, ObjectId> set_vars;

    std::vector<VarId> projection_vars;

    uint64_t limit;

    static void print(std::ostream& os, std::ostream& escaped_os, ObjectId oid);

    static void print_path_node(std::ostream& os, ObjectId node_id);
    static void print_path_edge(std::ostream& os, ObjectId edge_id, bool inverse);
};
} // namespace MQL