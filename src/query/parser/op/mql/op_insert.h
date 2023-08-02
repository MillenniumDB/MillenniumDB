#pragma once

#include <vector>

#include "query/parser/op/op.h"
#include "query/parser/op/mql/graph_pattern/op_edge.h"
#include "query/parser/op/mql/graph_pattern/op_label.h"
#include "query/parser/op/mql/graph_pattern/op_property.h"

namespace MQL {
class OpInsert : public Op {
public:
    // graph pattern
    std::vector<OpLabel> labels;
    std::vector<OpProperty> properties;
    std::vector<OpEdge> edges;

    OpInsert(
        std::vector<OpLabel>&&    labels,
        std::vector<OpProperty>&& properties,
        std::vector<OpEdge>&&     edges
    ) :
        labels     (std::move(labels)),
        properties (std::move(properties)),
        edges      (std::move(edges)) { }

    virtual std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpInsert>(*this);
    }

    bool read_only() const override { return false; }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return { };
    }

    std::set<VarId> get_scope_vars() const override {
        return { };
    }

    std::set<VarId> get_safe_vars() const override {
        return { };
    }

    std::set<VarId> get_fixable_vars() const override {
        return { };
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpInsert()\n";
        return os;
    }
};
} // namespace MQL
