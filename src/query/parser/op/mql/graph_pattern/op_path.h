#pragma once

#include <memory>
#include <string>

#include "query/id.h"
#include "query/parser/op/op.h"
#include "query/parser/paths/regular_path_expr.h"

namespace MQL {

class OpPath : public Op {
public:
    const VarId var;
    const Id    from;
    const Id    to;
    const PathSemantic semantic;

    std::unique_ptr<RegularPathExpr> path;

    OpPath(
        VarId                            var,
        Id                               from,
        Id                               to,
        PathSemantic                     semantic,
        std::unique_ptr<RegularPathExpr> path
    ) :
        var      (var),
        from     (from),
        to       (to),
        semantic (semantic),
        path     (std::move(path)) { }

    OpPath(const OpPath& other) :
        var      (other.var),
        from     (other.from),
        to       (other.to),
        semantic (other.semantic),
        path     (other.path->clone()) { }

    virtual std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpPath>(*this);
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool operator<(const OpPath& other) const {
        // TODO: rethink
        if (from.get_OID() < other.from.get_OID()) {
            return true;
        } else if (to.get_OID() < other.to.get_OID()) {
            return true;
        } else if (*path < *other.path) {
            return true;
        }
        return false;
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;
        if (from.is_var()) {
            res.insert(from.get_var());
        }
        if (to.is_var()) {
            res.insert(to.get_var());
        }
        res.insert(var);
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override {
        return get_all_vars();
    }

    std::set<VarId> get_fixable_vars() const override {
        return get_all_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ')
           << "OpPath(" << Paths::get_semantic_str(semantic)
           << ", " << get_query_ctx().get_var_name(var)
           << ", " << from
           << ", " << to
           << ", " << path->to_string()
           << ")\n";
        return os;
    }
};
} // namespace MQL
