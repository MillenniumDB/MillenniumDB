#pragma once

#include <variant>

#include "query/parser/op/sparql/op.h"

namespace SPARQL {

class OpGraph : public Op {
public:
    std::variant<VarId, std::string> graph;
    std::unique_ptr<Op> op;

    OpGraph(VarId var, std::unique_ptr<Op> op) :
        graph (var),
        op    (std::move(op)) { }

    OpGraph(std::string iri, std::unique_ptr<Op> op) :
        graph (iri),
        op    (std::move(op)) { }

    std::unique_ptr<Op> clone() const override {
        if (std::holds_alternative<VarId>(graph)) {
            return std::make_unique<OpGraph>(std::get<VarId>(graph), op->clone());
        }
        else {
            return std::make_unique<OpGraph>(std::get<std::string>(graph), op->clone());
        }
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return op->get_all_vars();
    }

    std::set<VarId> get_scope_vars() const override {
        return op->get_scope_vars();
    }

    std::set<VarId> get_safe_vars() const override {
        return op->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override {
        return op->get_fixable_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpGraph(";

        if (std::holds_alternative<VarId>(graph)) {
            os << get_query_ctx().get_var_name(std::get<VarId>(graph));
        } else {
            os << '<' << std::get<std::string>(graph);
        }
        os << ")\n";
        op->print_to_ostream(os, indent + 2);
        return os;
    }
};
} // namespace SPARQL
