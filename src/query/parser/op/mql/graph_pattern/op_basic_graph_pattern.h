#pragma once

#include <set>

#include "query/parser/op/mql/graph_pattern/disjoint_term.h"
#include "query/parser/op/mql/graph_pattern/disjoint_var.h"
#include "query/parser/op/mql/graph_pattern/edge.h"
#include "query/parser/op/mql/graph_pattern/label.h"
#include "query/parser/op/mql/graph_pattern/path.h"
#include "query/parser/op/mql/graph_pattern/property.h"
#include "query/parser/op/mql/op.h"

namespace MQL {

enum class PropertyType : uint64_t {
    TYPE_NULL = 1 << 4, // 10000
    TYPE_STRING = 1 << 3, // 01000
    TYPE_INTEGER = 1 << 2, // 00100
    TYPE_BOOL = 1 << 1, // 00010
    TYPE_FLOAT = 1 << 0, // 00001
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

// class PropertyOperatorConstraint {
// public:
//     VarId var;
//     ObjectId key;
// };

class OpBasicGraphPattern : public Op {
public:
    std::set<Label> labels;
    std::set<Property> properties;
    std::set<Edge> edges;
    std::set<Path> paths;
    std::set<DisjointVar> disjoint_vars;
    std::set<DisjointTerm> disjoint_terms;

    std::set<VarId> vars; // contains declared variables and anonymous (auto-generated in the constructor)

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpBasicGraphPattern>(*this);
    }

    void add_label(Id node, ObjectId label)
    {
        if (node.is_var()) {
            vars.insert(node.get_var());
        }
        labels.emplace(node, label);
    }

    void add_property(Id obj, ObjectId key, Id value)
    {
        if (obj.is_var()) {
            vars.insert(obj.get_var());
        }
        properties.emplace(obj, key, value);
    }

    void add_edge(Id from, Id to, Id type, Id edge)
    {
        if (from.is_var()) {
            vars.insert(from.get_var());
        }
        if (to.is_var()) {
            vars.insert(to.get_var());
        }
        if (type.is_var()) {
            vars.insert(type.get_var());
        }
        if (edge.is_var()) {
            vars.insert(edge.get_var());
        }

        edges.emplace(from, to, type, edge);
    }

    void add_path(
        VarId var,
        Id from,
        Id to,
        PathSemantic semantic,
        Path::Direction direction,
        uint64_t K,
        std::unique_ptr<RegularPathExpr> path
    )
    {
        vars.insert(var);
        if (from.is_var()) {
            vars.insert(from.get_var());
        }
        if (to.is_var()) {
            vars.insert(to.get_var());
        }
        paths.emplace(var, from, to, semantic, direction, K, std::move(path));
    }

    void add_disjoint_var(VarId var)
    {
        vars.insert(var);
        disjoint_vars.emplace(var);
    }

    void try_add_possible_disjoint_var(VarId var)
    {
        if (vars.insert(var).second) {
            disjoint_vars.emplace(var);
        }
    }

    void add_disjoint_term(ObjectId term)
    {
        disjoint_terms.emplace(term);
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return vars;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpBasicGraphPattern()\n";

        for (auto& label : labels) {
            label.print(os, indent + 2);
        }
        for (auto& property : properties) {
            property.print(os, indent + 2);
        }
        for (auto& edge : edges) {
            edge.print(os, indent + 2);
        }
        for (auto& path : paths) {
            path.print(os, indent + 2);
        }
        for (auto& disjoint_var : disjoint_vars) {
            disjoint_var.print(os, indent + 2);
        }
        for (auto& disjoint_term : disjoint_terms) {
            disjoint_term.print(os, indent + 2);
        }

        return os;
    }
};
} // namespace MQL
