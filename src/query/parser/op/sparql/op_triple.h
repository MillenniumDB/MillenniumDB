#pragma once

#include <cassert>

#include "query/id.h"
#include "query/parser/op/op.h"

namespace SPARQL {

class OpTriple : public Op {
public:
    Id subject;
    Id predicate;
    Id object;

    OpTriple(Id subject, Id predicate, Id object) :
        subject   (subject),
        predicate (predicate),
        object    (object) { }

    OpTriple(const OpTriple& triple) :
        subject   (triple.subject),
        predicate (triple.predicate),
        object    (triple.object) { }

    void operator=(OpTriple&& other) {
        subject   = other.subject;
        predicate = other.predicate;
        object    = other.object;
    }

    std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpTriple>(*this);
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;
        if (subject.is_var()) {
            res.insert(subject.get_var());
        }
        if (predicate.is_var()) {
            res.insert(predicate.get_var());
        }
        if (object.is_var()) {
            res.insert(object.get_var());
        }
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
        os << std::string(indent, ' ');
        os << "OpTriple(";
        os << subject << ' ';
        os << predicate << ' ';
        os << object;
        os << ")\n";
        return os;
    }
};
} // namespace SPARQL
