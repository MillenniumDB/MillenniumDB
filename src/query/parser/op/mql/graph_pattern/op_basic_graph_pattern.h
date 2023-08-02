#pragma once

#include <set>
#include <vector>

#include "query/exceptions.h"

#include "query/parser/op/mql/graph_pattern/op_edge.h"
#include "query/parser/op/mql/graph_pattern/op_disjoint_term.h"
#include "query/parser/op/mql/graph_pattern/op_disjoint_var.h"
#include "query/parser/op/mql/graph_pattern/op_label.h"
#include "query/parser/op/mql/graph_pattern/op_path.h"
#include "query/parser/op/mql/graph_pattern/op_property.h"
#include "query/parser/op/op.h"

namespace MQL {

class OpBasicGraphPattern : public Op {
public:
    std::set<OpLabel>        labels;
    std::set<OpProperty>     properties;
    std::set<OpEdge>         edges;
    std::set<OpPath>         paths;
    std::set<OpDisjointVar>  disjoint_vars;
    std::set<OpDisjointTerm> disjoint_terms;

    std::set<VarId> vars; // contains declared variables and anonymous (auto-generated in the constructor)

    std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpBasicGraphPattern>(*this);
    }

    void add_label(OpLabel&& op_label) {
        for (auto& var : op_label.get_all_vars()) {
            vars.insert(var);
        }
        labels.insert(std::move(op_label));
    }

    void add_property(OpProperty op_property) {
        // auto property_search = properties.find(op_property);

        // if (property_search != properties.end()) {
        //     auto old_property = *property_search;
        //     if (!old_property.value.is_var()
        //         && !op_property.value.is_var()
        //         && old_property.value.get_OID() != op_property.value.get_OID())
        //     {
        //         throw QuerySemanticException("Property its declared with different values in MATCH");
        //     }
        // } else {
            for (auto& var : op_property.get_all_vars()) {
                vars.insert(var);
            }
            properties.insert(std::move(op_property));
        // }
    }

    void add_edge(OpEdge&& op_edge) {
        for (auto& var : op_edge.get_all_vars()) {
            vars.insert(var);
        }
        edges.insert(std::move(op_edge));
    }

    void add_path(OpPath&& op_path) {
        for (auto& var : op_path.get_all_vars()) {
            vars.insert(var);
        }
        paths.insert(std::move(op_path));
    }

    void add_disjoint_var(OpDisjointVar&& op_disjoint_var) {
        vars.insert(op_disjoint_var.var);
        disjoint_vars.insert(std::move(op_disjoint_var));
    }

    void try_add_possible_disjoint_var(VarId var) {
        if (vars.insert(var).second) {
            disjoint_vars.insert(OpDisjointVar(var));
        }
    }

    void add_disjoint_term(ObjectId term) {
        disjoint_terms.insert(OpDisjointTerm(term));
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        return vars;
    }

    std::set<VarId> get_scope_vars() const override {
        return vars;
    }

    std::set<VarId> get_safe_vars() const override {
        return vars;
    }

    std::set<VarId> get_fixable_vars() const override {
        return vars;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
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
        for (auto& disjoint_var : disjoint_vars) {
            disjoint_var.print_to_ostream(os, indent + 2);
        }
        for (auto& disjoint_term : disjoint_terms) {
            disjoint_term.print_to_ostream(os, indent + 2);
        }
        return os;
    }
};
} // namespace MQL
