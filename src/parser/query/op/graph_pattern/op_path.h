#pragma once

#include <memory>
#include <string>

#include "base/query/query_element.h"
#include "parser/query/paths/path.h"
#include "parser/query/op/op.h"

enum class PathSemantic {
    ALL,
    ANY
};


class OpPath : public Op {
public:
    const Var          var;
    const QueryElement from;
    const QueryElement to;
    const PathSemantic semantic;

    std::unique_ptr<IPath> path;

    OpPath(const std::string& var,
            const std::string& from,
            const std::string& to,
            PathSemantic semantic,
            std::unique_ptr<IPath> path) :
        var      (Var(var)),
        from     (QueryElement::deduce(from)),
        to       (QueryElement::deduce(to)),
        semantic (semantic),
        path     (std::move(path)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool operator<(const OpPath& other) const {
        if (from < other.from) {
            return true;
        } else if (to < other.to) {
            return true;
        } else if (*path < *other.path) {
            return true;
        }
        return false;
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        if (from.is_var()) {
            res.insert(from.to_var());
        }
        if (to.is_var()) {
            res.insert(to.to_var());
        }
        res.insert(var);
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ')
           << "OpPath("
           << (semantic == PathSemantic::ALL ? "ALL" : "ANY") << ", "
           << var << ", " << from << ", " << to << ", " << path->to_string() << ")\n";
        return os;
    };
};
