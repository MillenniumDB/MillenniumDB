#pragma once

#include <string>

#include "query/parser/op/sparql/op.h"
#include "storage/index/hnsw/hnsw_metric.h"

namespace SPARQL {

class OpCreateHNSWIndex : public Op {
public:
    const std::string index_name;
    const std::string predicate;
    const uint64_t dimension;
    const uint64_t max_edges;
    const uint64_t num_candidates;
    const HNSW::MetricType metric_type;

    OpCreateHNSWIndex(
        std::string&& index_name_,
        std::string&& predicate_,
        uint64_t dimension_,
        uint64_t max_edges_,
        uint64_t num_candidates_,
        HNSW::MetricType metric_type_
    ) :
        index_name { std::move(index_name_) },
        predicate { std::move(predicate_) },
        dimension { dimension_ },
        max_edges { max_edges_ },
        num_candidates { num_candidates_ },
        metric_type { metric_type_ }
    { }

    std::unique_ptr<Op> clone() const override
    {
        auto index_name_clone = index_name;
        auto predicate_clone = predicate;
        return std::make_unique<OpCreateHNSWIndex>(
            std::move(index_name_clone),
            std::move(predicate_clone),
            dimension,
            max_edges,
            num_candidates,
            metric_type
        );
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return {};
    }

    std::set<VarId> get_scope_vars() const override
    {
        return {};
    }

    std::set<VarId> get_safe_vars() const override
    {
        return {};
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpCreateHnswIndex(index_name: " << index_name;
        os << ", predicate: " << predicate << ", dimension: " << dimension << ", max_edges: " << max_edges
           << ", num_candidates: " << num_candidates << ", metric_type: " << metric_type << ")\n";
        return os;
    }
};
} // namespace SPARQL
