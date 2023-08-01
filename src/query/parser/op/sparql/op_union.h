#pragma once

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

#include "query/parser/op/op.h"

namespace SPARQL {
class OpUnion : public Op {
public:
    std::vector<std::unique_ptr<Op>> unions;

    OpUnion(std::vector<std::unique_ptr<Op>>&& unions) :
        unions (std::move(unions)) { }

    std::unique_ptr<Op> clone() const override {
        std::vector<std::unique_ptr<Op>> new_unions;
        new_unions.reserve(unions.size());
        for (auto& op : unions) {
            new_unions.push_back(op->clone());
        }
        return std::make_unique<OpUnion>(std::move(new_unions));
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;
        for (auto& child : unions) {
            for (auto& child_var : child->get_all_vars()) {
                res.insert(child_var);
            }
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        std::set<VarId> res;
        for (auto& child : unions) {
            for (auto& child_var : child->get_scope_vars()) {
                res.insert(child_var);
            }
        }
        return res;
    }

    std::set<VarId> get_safe_vars() const override {
        assert(unions.size() > 0);
        auto res = unions[0]->get_safe_vars();
        for (size_t i = 1; i < unions.size(); i++) {
            auto current_safe_vars = unions[i]->get_safe_vars();
            std::set<VarId> intersection;

            std::set_intersection(
                res.begin(), res.end(),
                current_safe_vars.begin(), current_safe_vars.end(),
                std::inserter(intersection, intersection.begin())
            );

            res = std::move(intersection);
        }
        return res;
    }

    std::set<VarId> get_fixable_vars() const override {
        assert(unions.size() > 0);
        auto res = unions[0]->get_fixable_vars();
        for (size_t i = 1; i < unions.size(); i++) {
            auto current_fixable_vars = unions[i]->get_fixable_vars();
            std::set<VarId> intersection;

            std::set_intersection(
                res.begin(), res.end(),
                current_fixable_vars.begin(), current_fixable_vars.end(),
                std::inserter(intersection, intersection.begin())
            );

            res = std::move(intersection);
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpUnion(\n";
        for (auto& child : unions) {
            child->print_to_ostream(os, indent + 2);
        }
        os << std::string(indent, ' ');
        os << ")\n";
        return os;
    }
};
} // namespace SPARQL
