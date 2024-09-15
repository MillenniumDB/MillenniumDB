#pragma once

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

#include "misc/set_operations.h"
#include "query/parser/op/op.h"
#include "query/parser/op/sparql/op_triple.h"

namespace SPARQL {
class OpDeleteData : public Op {
public:
    std::vector<OpTriple> triples;

    OpDeleteData(std::vector<OpTriple>&& triples) :
        triples (std::move(triples)) { }

    std::unique_ptr<Op> clone() const override {
        std::vector<OpTriple> new_triples;
        new_triples.reserve(triples.size());
        for (auto& triple : triples) {
            new_triples.push_back(triple);
        }
        return std::make_unique<OpDeleteData>(std::move(new_triples));
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;
        for (auto& triple : triples) {
            for (auto& child_var : triple.get_all_vars()) {
                res.insert(child_var);
            }
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        std::set<VarId> res;
        // for (auto& child : updates) {
        //     for (auto& child_var : child->get_scope_vars()) {
        //         res.insert(child_var);
        //     }
        // }
        return res;
    }

    std::set<VarId> get_safe_vars() const override {
        std::set<VarId> res;
        // for (auto& child : updates) {
        //     for (auto& child_var : child->get_safe_vars()) {
        //         res.insert(child_var);
        //     }
        // }
        return res;
    }

    std::set<VarId> get_fixable_vars() const override {
        std::set<VarId> res;
        // for (auto& child : updates) {
        //     for (auto& child_var : child->get_fixable_vars()) {
        //         res.insert(child_var);
        //     }
        // }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpDeleteData()\n";

        for (auto& triple : triples) {
            triple.print_to_ostream(os, indent + 2);
        }

        return os;
    }
};
} // namespace SPARQL
