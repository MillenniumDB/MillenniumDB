#pragma once

#include <memory>

#include "query/executor/binding_iter.h"
#include "query/executor/query_executor/query_executor.h"
#include "network/sparql/response_type.h"

namespace SPARQL {

class AskExecutor : public QueryExecutor {
public:
    AskExecutor(
        std::unique_ptr<BindingIter> root,
        SPARQL::ResponseType         response_type
    ) :
        root          (std::move(root)),
        response_type (response_type) { }

    uint64_t execute(std::ostream&) override;

    void analyze(std::ostream&, bool print_stats = false, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> root;
    std::unique_ptr<Binding>     binding;
    SPARQL::ResponseType         response_type;
};
} // namespace SPARQL
