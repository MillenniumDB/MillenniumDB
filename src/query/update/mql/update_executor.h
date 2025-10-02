#pragma once

#include <boost/unordered/unordered_flat_map.hpp>

#include "query/executor/binding_iter.h"
#include "query/executor/query_executor/query_executor.h"
#include "query/executor/query_executor/streaming_query_executor.h"
#include "query/update/mql/update_action.h"

namespace MQL {

class UpdateExecutor : public QueryExecutor, public StreamingQueryExecutor {
public:
    UpdateExecutor(
        std::unique_ptr<BindingIter> iter,
        std::unique_ptr<UpdateContext> update_context,
        std::vector<std::unique_ptr<UpdateAction>>&& update_actions
    ) :
        iter(std::move(iter)),
        update_context(std::move(update_context)),
        update_actions(std::move(update_actions))
    { }

    uint64_t execute();

    uint64_t execute(std::ostream&) override
    {
        return execute();
    }

    uint64_t execute(MDBServer::StreamingResponseWriter&) override
    {
        return execute();
    }

    void analyze(std::ostream& os, bool print_stats, int indent = 0) const override;

private:
    std::unique_ptr<BindingIter> iter;

    std::unique_ptr<UpdateContext> update_context;

    std::vector<std::unique_ptr<UpdateAction>> update_actions;
};
} // namespace MQL
