#pragma once

#include "query/executor/binding_iter.h"
#include "query/executor/query_executor/query_executor.h"

namespace MQL {
class DescribeExecutor : public QueryExecutor {
public:
    DescribeExecutor(
        std::unique_ptr<BindingIter> labels,
        std::unique_ptr<BindingIter> properties,
        std::unique_ptr<BindingIter> outgoing_connections,
        std::unique_ptr<BindingIter> incoming_connections,
        uint64_t labels_limit,
        uint64_t properties_limit,
        uint64_t outgoing_limit,
        uint64_t incoming_limit
    ) :
        labels               (std::move(labels)),
        properties           (std::move(properties)),
        outgoing_connections (std::move(outgoing_connections)),
        incoming_connections (std::move(incoming_connections)),
        labels_limit         (labels_limit),
        properties_limit     (properties_limit),
        outgoing_limit       (outgoing_limit),
        incoming_limit       (incoming_limit) { }

    // returns how many results were obtained
    uint64_t execute(std::ostream&) override;

    void analyze(std::ostream&, bool print_stats = false, int indent = 0) const override;

private:
    Binding binding = Binding(3);

    // 0: label, 1: properties, 2: outer connections, 3: inner connections
    int status = 0;

    std::unique_ptr<BindingIter> labels;
    std::unique_ptr<BindingIter> properties;
    std::unique_ptr<BindingIter> outgoing_connections;
    std::unique_ptr<BindingIter> incoming_connections;

    // Limits
    const uint64_t labels_limit;
    const uint64_t properties_limit;
    const uint64_t outgoing_limit;
    const uint64_t incoming_limit;

    uint_fast32_t count_labels = 0;
    uint_fast32_t count_properties = 0;
    uint_fast32_t count_outgoing = 0;
    uint_fast32_t count_incoming = 0;
};
} // namespace MQL