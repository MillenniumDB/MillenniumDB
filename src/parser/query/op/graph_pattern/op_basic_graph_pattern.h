#pragma once

#include <set>
#include <vector>

#include "base/exceptions.h"
// #include "base/query/query_element.h"

#include "parser/query/op/op.h"
#include "parser/query/op/graph_pattern/op_edge.h"
#include "parser/query/op/graph_pattern/op_isolated_term.h"
#include "parser/query/op/graph_pattern/op_isolated_var.h"
#include "parser/query/op/graph_pattern/op_label.h"
#include "parser/query/op/graph_pattern/op_path.h"
#include "parser/query/op/graph_pattern/op_property.h"

class OpBasicGraphPattern : public Op {
public:
    std::set<OpLabel>        labels;
    std::set<OpProperty>     properties;
    std::set<OpEdge>         edges;
    std::set<OpPath>         paths;
    std::set<OpIsolatedVar>  isolated_vars;
    std::set<OpIsolatedTerm> isolated_terms;

    std::set<Var> vars; // contains declared variables and anonymous (auto-generated in the constructor)

    OpBasicGraphPattern() = default;

    void add_label(OpLabel&& op_label) {
        for (auto& var : op_label.get_vars()) {
            vars.insert(var);
        }
        labels.insert(std::move(op_label));
    }

    void add_property(OpProperty&& op_property) {
        auto property_search = properties.find(op_property);

        if (property_search != properties.end()) {
            auto old_property = *property_search;
            if (old_property.value != op_property.value) {
                throw QuerySemanticException(op_property.node.to_string() + "." + op_property.key
                                             + " its declared with different values in MATCH");
            }
        } else {
            for (auto& var : op_property.get_vars()) {
                vars.insert(var);
            }
            properties.insert(std::move(op_property));
        }
    }

    void add_edge(OpEdge&& op_edge) {
        for (auto& var : op_edge.get_vars()) {
            vars.insert(var);
        }
        edges.insert(std::move(op_edge));
    }

    void add_path(OpPath&& op_path) {
        for (auto& var : op_path.get_vars()) {
            vars.insert(var);
        }
        paths.insert(std::move(op_path));
    }

    void add_isolated_var(OpIsolatedVar&& op_isolated_var) {
        vars.insert(op_isolated_var.var);
        isolated_vars.insert(std::move(op_isolated_var));
    }

    void try_add_possible_isolated_var(const Var& var) {
        if (vars.insert(var).second) {
            isolated_vars.insert(OpIsolatedVar(var));
        }
    }

    void add_isolated_term(const std::string& term) {
        isolated_terms.insert(OpIsolatedTerm(QueryElement::deduce(term)));
    }

    void accept_visitor(OpVisitor &visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        return vars;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override{
        os << std::string(indent, ' ');
        os << "OpBasicGraphPattern()\n";

        for (auto& label : labels) {
            label.print_to_ostream(os, indent + 2);
        }
        for (auto& property : properties) {
            property.print_to_ostream(os, indent + 2);
        }
        for (auto& edge : edges) {
            edge.print_to_ostream(os, indent + 2);
        }
        for (auto& path : paths) {
            path.print_to_ostream(os, indent + 2);
        }
        for (auto& isolated_var : isolated_vars) {
            isolated_var.print_to_ostream(os, indent + 2);
        }
        for (auto& isolated_term : isolated_terms) {
            isolated_term.print_to_ostream(os, indent + 2);
        }
        return os;
    }
};
