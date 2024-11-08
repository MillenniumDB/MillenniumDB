#pragma once

#include "query/parser/op/op.h"
#include "query/parser/op/sparql/op_triple.h"

namespace SPARQL {

class OpConstruct : public Op {
public:
    std::vector<OpTriple> triples;
    std::unique_ptr<Op>   op;

    const uint64_t offset;
    const uint64_t limit;

    OpConstruct(
        std::vector<OpTriple>&& triples,
        std::unique_ptr<Op> op,
        uint64_t offset,
        uint64_t limit
    ) :
        triples (std::move(triples)),
        op      (std::move(op)),
        offset  (offset),
        limit   (limit) { }


    std::unique_ptr<Op> clone() const override {
        std::vector<OpTriple> new_triples;

        for (const auto& triple : triples) {
            new_triples.push_back(OpTriple(triple));
        }

        return std::make_unique<OpConstruct>(
            std::move(new_triples),
            op->clone(),
            offset,
            limit
        );
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;

        if (op) {
            res = op->get_all_vars();
        }

        for (auto& triple : triples) {
            auto vars = triple.get_all_vars();
            res.insert(vars.begin(), vars.end());
        }

        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        return {};
    }

    std::set<VarId> get_safe_vars() const override {
        return {};
    }

    std::set<VarId> get_fixable_vars() const override {
        std::set<VarId> res;

        if (op) {
            res = op->get_fixable_vars();
        }

        for (auto& triple : triples) {
            auto vars = triple.get_fixable_vars();
            res.insert(vars.begin(), vars.end());
        }

        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpConstruct()\n";

        for (auto& triple : triples) {
            triple.print_to_ostream(os, indent + 2);
        }

        return os;
    }
};
} // namespace SPARQL
