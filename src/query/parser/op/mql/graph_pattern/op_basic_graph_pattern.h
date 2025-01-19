#pragma once

#include <set>
#include <vector>

#include "query/parser/expr/mql/function/expr_text_search.h"
#include "query/parser/op/mql/graph_pattern/op_disjoint_term.h"
#include "query/parser/op/mql/graph_pattern/op_disjoint_var.h"
#include "query/parser/op/mql/graph_pattern/op_edge.h"
#include "query/parser/op/mql/graph_pattern/op_label.h"
#include "query/parser/op/mql/graph_pattern/op_path.h"
#include "query/parser/op/mql/graph_pattern/op_property.h"
#include "query/parser/op/op.h"

namespace MQL {

enum class PropertyType : uint64_t {
    TYPE_NULL = 1 << 4,    // 10000
    TYPE_STRING = 1 << 3,  // 01000
    TYPE_INTEGER = 1 << 2, // 00100
    TYPE_BOOL = 1 << 1,    // 00010
    TYPE_FLOAT = 1 << 0,   // 00001
};

enum class OperatorType {
    EQUALS,
    NOT_EQUALS,
    LESS,
    GREATER,
    LESS_OR_EQUALS,
    GREATER_OR_EQUALS,
};

class PropertyTypeConstraint {
public:
    VarId var_without_property;
    ObjectId key;
    VarId var_with_property;
    uint64_t type_bitmap;

    PropertyTypeConstraint(VarId v, ObjectId k, VarId val, uint64_t bitmap = 0) :
        var_without_property(v),
        key(k),
        var_with_property(val),
        type_bitmap(bitmap)
    { }
};

class PropertyOperatorConstraint {
public:
    VarId var;
    ObjectId key;
};

class OpBasicGraphPattern : public Op {
public:
    std::set<OpLabel>        labels;
    std::set<OpProperty>     properties;
    std::set<OpEdge>         edges;
    std::set<OpPath>         paths;
    std::set<OpDisjointVar>  disjoint_vars;
    std::set<OpDisjointTerm> disjoint_terms;
    std::set<ExprTextSearch> expr_text_searches;

    std::set<VarId> vars; // contains declared variables and anonymous (auto-generated in the constructor)
    std::vector<OpProperty> optional_properties;

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpBasicGraphPattern>(*this);
    }

    void add_label(OpLabel&& op_label)
    {
        for (auto& var : op_label.get_all_vars()) {
            vars.insert(var);
        }
        labels.insert(std::move(op_label));
    }

    void add_property(OpProperty op_property)
    {
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

    void add_edge(OpEdge&& op_edge)
    {
        for (auto& var : op_edge.get_all_vars()) {
            vars.insert(var);
        }
        edges.insert(std::move(op_edge));
    }

    void add_path(OpPath&& op_path)
    {
        for (auto& var : op_path.get_all_vars()) {
            vars.insert(var);
        }
        paths.insert(std::move(op_path));
    }

    void add_text_search(ExprTextSearch&& expr_text_search)
    {
        vars.insert(expr_text_search.object_var);
        vars.insert(expr_text_search.match_var);
        expr_text_searches.insert(std::move(expr_text_search));
    }

    void add_disjoint_var(OpDisjointVar&& op_disjoint_var)
    {
        vars.insert(op_disjoint_var.var);
        disjoint_vars.insert(std::move(op_disjoint_var));
    }

    void try_add_possible_disjoint_var(VarId var)
    {
        if (vars.insert(var).second) {
            disjoint_vars.insert(OpDisjointVar(var));
        }
    }

    void add_disjoint_term(ObjectId term)
    {
        disjoint_terms.insert(OpDisjointTerm(term));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        auto res = vars;
        for (auto& property : optional_properties) {
            assert(property.value.is_var());
            res.insert(property.value.get_var());
        }

        return res;
    }

    std::set<VarId> get_scope_vars() const override
    {
        return vars;
    }

    std::set<VarId> get_safe_vars() const override
    {
        return vars;
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return vars;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpBasicGraphPattern()\n";

        for (auto& op_property : optional_properties) {
            op_property.print_to_ostream(os, indent);
        }

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
        for (const auto& expr_text_search : expr_text_searches) {
            expr_text_search.print_to_ostream(os, indent + 2);
        }

        return os;
    }
};
} // namespace MQL
