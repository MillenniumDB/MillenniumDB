#pragma once

#include <ostream>
#include <string>

#include "base/query/sparql/sparql_element.h"
#include "parser/query/op/op.h"

namespace SPARQL {

class OpTriple : public Op {
public:
    SparqlElement subject;
    SparqlElement predicate;
    SparqlElement object;

    OpTriple(SparqlElement _subject, SparqlElement _predicate, SparqlElement _object) :
        subject(std::move(_subject)), predicate(std::move(_predicate)), object(std::move(_object)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        if (subject.is_var()) {
            res.insert(subject.to_var());
        }
        if (predicate.is_var()) {
            res.insert(predicate.to_var());
        }
        if (object.is_var()) {
            res.insert(object.to_var());
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpTriple(";
        os << subject << "\t";
        os << predicate << "\t";
        os << object << "\t";
        os << ")\n";
        return os;
    };
};
} // namespace SPARQL