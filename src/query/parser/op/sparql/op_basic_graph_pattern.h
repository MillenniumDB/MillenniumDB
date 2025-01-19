#pragma once

#include "query/parser/op/op.h"
#include "query/parser/op/sparql/op_path.h"
#include "query/parser/op/sparql/op_text_search_index.h"
#include "query/parser/op/sparql/op_triple.h"

namespace SPARQL {

class OpBasicGraphPattern : public Op {
public:
    std::vector<OpTriple> triples;
    std::vector<OpPath> paths;
    std::vector<OpTextSearchIndex> text_searches;

    OpBasicGraphPattern(
        std::vector<OpTriple> triples,
        std::vector<OpPath> paths
    ) :
        triples (std::move(triples)),
        paths   (std::move(paths)) { }

    OpBasicGraphPattern(
        std::vector<OpTriple> triples,
        std::vector<OpPath> paths,
        std::vector<OpTextSearchIndex> text_searches
    ) :
        triples         (std::move(triples)),
        paths           (std::move(paths)),
        text_searches   (std::move(text_searches)) { }

    std::unique_ptr<Op> clone() const override
    {
        std::vector<OpTriple> new_triples;
        std::vector<OpPath> new_paths;
        std::vector<OpTextSearchIndex> new_text_searches;
        for (const auto& triple : triples) {
            new_triples.emplace_back(triple);
        }
        for (auto& path : paths) {
            new_paths.emplace_back(path);
        }
        for (auto& text_search : text_searches) {
            new_text_searches.emplace_back(text_search);
        }
        return std::make_unique<OpBasicGraphPattern>(
            std::move(new_triples),
            std::move(new_paths),
            std::move(new_text_searches)
        );
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
        for (auto& text_search : text_searches) {
            auto vars = text_search.get_all_vars();
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
        for (auto& text_search : text_searches) {
            auto vars = text_search.get_scope_vars();
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
        for (auto& text_search : text_searches) {
            auto vars = text_search.get_safe_vars();
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
        for (auto& text_search : text_searches) {
            auto vars = text_search.get_fixable_vars();
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
        for (auto& text_search : text_searches) {
            text_search.print_to_ostream(os, indent + 2);
        }
        return os;
    }
};
} // namespace SPARQL
