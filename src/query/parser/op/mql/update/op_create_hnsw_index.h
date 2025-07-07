#pragma once

#include <string>

#include "query/parser/op/mql/op.h"
#include "storage/index/hnsw/hnsw_metric.h"

namespace MQL {

class OpCreateHNSWIndex : public Op {
public:
    const std::string index_name;
    const std::string property;
    const uint64_t dimension;
    const uint64_t max_edges;
    const uint64_t max_candidates;
    const HNSW::MetricType metric_type;

    OpCreateHNSWIndex(
        std::string&& index_name,
        std::string&& property,
        uint64_t dimension,
        uint64_t num_edges,
        uint64_t num_candidates,
        HNSW::MetricType metric_type
    ) :
        index_name { std::move(index_name) },
        property { std::move(property) },
        dimension { dimension },
        max_edges { num_edges },
        max_candidates { num_candidates },
        metric_type { metric_type }
    { }

    std::unique_ptr<Op> clone() const override
    {
        auto text_search_index_name_clone = index_name;
        auto property_clone = property;
        return std::make_unique<OpCreateHNSWIndex>(
            std::move(text_search_index_name_clone),
            std::move(property_clone),
            dimension,
            max_edges,
            max_candidates,
            metric_type
        );
    }

    bool read_only() const override
    {
        return false;
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpCreateHNSWIndex(index_name: " << index_name << ", property: " << property
           << ", dimension: " << dimension << ", num_edges: " << max_edges
           << ", num_candidates: " << max_candidates << ", metric_type: " << metric_type << ")\n";
        return os;
    }
};
} // namespace MQL
