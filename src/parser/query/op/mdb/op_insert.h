#pragma once

#include <vector>

#include "parser/query/op/op.h"
#include "parser/query/op/mdb/graph_pattern/op_edge.h"
#include "parser/query/op/mdb/graph_pattern/op_label.h"
#include "parser/query/op/mdb/graph_pattern/op_property.h"

namespace MDB {
class OpInsert : public Op {
public:
    // graph pattern
    std::vector<OpLabel> labels;
    std::vector<OpProperty> properties;
    std::vector<OpEdge> edges;

    OpInsert(std::vector<OpLabel> labels, std::vector<OpProperty> properties, std::vector<OpEdge> edges) :
        labels     (std::move(labels)),
        properties (std::move(properties)),
        edges      (std::move(edges)) { }

    bool read_only() const override { return false; }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        return std::set<Var>();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpInsert()\n";
        return os;
    };
};
} // namespace MDB