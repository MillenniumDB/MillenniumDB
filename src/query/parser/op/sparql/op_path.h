#pragma once

#include "query/id.h"
#include "query/parser/op/op.h"
#include "query/parser/paths/regular_path_expr.h"

namespace SPARQL {

class OpPath : public Op {
public:
    VarId         var;
    Id            subject;
    Id            object;

    PathSemantic semantic;
    std::unique_ptr<RegularPathExpr> path;

    OpPath(
        VarId                            var,
        Id                               subject,
        Id                               object,
        PathSemantic                     semantic,
        std::unique_ptr<RegularPathExpr> path) :
        var       (var),
        subject   (subject),
        object    (object),
        semantic  (semantic),
        path      (std::move(path)) { }

    OpPath(const OpPath& op_path) :
        var       (op_path.var),
        subject   (op_path.subject),
        object    (op_path.object),
        semantic  (op_path.semantic),
        path      (op_path.path->clone()) { }

    void operator=(OpPath&& other) {
        var      = std::move(other.var);
        subject  = std::move(other.subject);
        object   = std::move(other.object);
        semantic = std::move(other.semantic);
        path     = std::move(other.path);
    }

    std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpPath>(*this);
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;
        if (subject.is_var()) {
            res.insert(subject.get_var());
        }
        if (object.is_var()) {
            res.insert(object.get_var());
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
        std::set<VarId> res;
        if (subject.is_var()) {
            res.insert(subject.get_var());
        }
        if (object.is_var()) {
            res.insert(object.get_var());
        }
        // path cannot be fixed
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpPath(";

        os << Paths::get_semantic_str(semantic)
           << ", " << get_query_ctx().get_var_name(var)
           << ", " << subject
           << ", " << object
           << ", " << path->to_string();

        os << ")\n";
        return os;
    }
};
} // namespace SPARQL
