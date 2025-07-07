#pragma once

#include "query/parser/op/sparql/op.h"
#include "query/parser/op/sparql/op_path.h"
#include "query/parser/op/sparql/op_triple.h"

namespace SPARQL {

class OpBasicGraphPattern : public Op {
public:
    std::vector<OpTriple> triples;
    std::vector<OpPath> paths;

    OpBasicGraphPattern(std::vector<OpTriple> triples, std::vector<OpPath> paths) :
        triples(std::move(triples)),
        paths(std::move(paths))
    { }

    std::unique_ptr<Op> clone() const override
    {
        std::vector<OpTriple> new_triples;
        std::vector<OpPath> new_paths;
        for (const auto& triple : triples) {
            new_triples.emplace_back(triple);
        }
        for (const auto& path : paths) {
            new_paths.emplace_back(path);
        }
        return std::make_unique<OpBasicGraphPattern>(std::move(new_triples), std::move(new_paths));
    }

    void merge(std::unique_ptr<OpBasicGraphPattern> other)
    {
        for (const auto& triple : other->triples) {
            triples.emplace_back(triple);
        }
        for (const auto& path : other->paths) {
            paths.emplace_back(path);
        }
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;
        for (auto& triple : triples) {
            auto vars = triple.get_all_vars();
            res.insert(vars.begin(), vars.end());
        }
        for (auto& path : paths) {
            auto vars = path.get_all_vars();
            res.insert(vars.begin(), vars.end());
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override
    {
        std::set<VarId> res;
        for (auto& triple : triples) {
            auto vars = triple.get_scope_vars();
            res.insert(vars.begin(), vars.end());
        }
        for (auto& path : paths) {
            auto vars = path.get_scope_vars();
            res.insert(vars.begin(), vars.end());
        }
        return res;
    }

    std::set<VarId> get_safe_vars() const override
    {
        std::set<VarId> res;
        for (auto& triple : triples) {
            auto vars = triple.get_safe_vars();
            res.insert(vars.begin(), vars.end());
        }
        for (auto& path : paths) {
            auto vars = path.get_safe_vars();
            res.insert(vars.begin(), vars.end());
        }
        return res;
    }

    std::set<VarId> get_fixable_vars() const override
    {
        std::set<VarId> res;
        for (auto& triple : triples) {
            auto vars = triple.get_fixable_vars();
            res.insert(vars.begin(), vars.end());
        }
        for (auto& path : paths) {
            auto vars = path.get_fixable_vars();
            res.insert(vars.begin(), vars.end());
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ') << "OpBasicGraphPattern()\n";

        for (auto& triple : triples) {
            triple.print_to_ostream(os, indent + 2);
        }
        for (auto& path : paths) {
            path.print_to_ostream(os, indent + 2);
        }
        return os;
    }
};
} // namespace SPARQL
