#pragma once

#include "parser/query/op/op.h"
#include "parser/query/op/sparql/op_triple.h"

namespace SPARQL {

class OpTriples : public Op {
public:
    std::vector<OpTriple> triples;

    OpTriples(std::vector<OpTriple>&& triples) : triples(std::move(triples)) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        for (auto& triple : triples) {
            auto vars = triple.get_vars();
            res.insert(vars.begin(), vars.end());
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpTriples(\n";
        for (auto& triple : triples) {
            os << std::string(indent + 2, ' ');
            os << triple << "\n";
        }
        os << std::string(indent, ' ');
        os << ")\n";
        return os;
    };
};
} // namespace SPARQL