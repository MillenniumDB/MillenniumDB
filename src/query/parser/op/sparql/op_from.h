#pragma once

#include "query/parser/op/op.h"

namespace SPARQL {

class OpFrom : public Op {
public:
    std::vector<std::string> from_graphs;
    std::vector<std::string> from_named_graphs;
    std::unique_ptr<Op> op;

    OpFrom(
        std::vector<std::string> from_graphs,
        std::vector<std::string> from_named_graphs,
        std::unique_ptr<Op> op
    ) :
        from_graphs(from_graphs),
        from_named_graphs(from_named_graphs),
        op(std::move(op))
    { }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpFrom>(from_graphs, from_named_graphs, op->clone());
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return op->get_all_vars();
    }

    std::set<VarId> get_scope_vars() const override
    {
        return op->get_scope_vars();
    }

    std::set<VarId> get_safe_vars() const override
    {
        return op->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return op->get_fixable_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ') << "OpFrom(";
        bool from_printed = false;
        if (from_graphs.size() > 0){
            from_printed = true;
            for (auto& graph : from_graphs){
                os << graph << ",";
            }
        }

        if (from_named_graphs.size() > 0) {
            if (from_printed) {
                os << " ";
            }
            os << "NAMED: ";
            for (auto& named_graph : from_named_graphs){
                os << named_graph << ",";
            }
        }

        os << ")\n";
        op->print_to_ostream(os, indent + 2);
        return os;
    }
};
} // namespace SPARQL
