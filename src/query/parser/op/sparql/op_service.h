#pragma once

#include <variant>

#include "query/query_context.h"
#include "query/parser/op/sparql/op.h"

namespace SPARQL {

class OpService : public Op {
public:
    bool silent;
    std::variant<VarId, std::string> var_or_iri;
    std::string query;
    std::string prefixes;
    std::unique_ptr<Op> op;

    OpService(
        bool                             silent,
        std::variant<VarId, std::string> var_or_iri,
        std::string                      query,
        std::string                      prefixes,
        std::unique_ptr<Op>              op
    ) :
        silent(silent),
        var_or_iri(var_or_iri),
        query(std::move(query)),
        prefixes(std::move(prefixes)),
        op(std::move(op)) { }

    std::unique_ptr<Op> clone() const override {
        return std::make_unique<OpService>(silent, var_or_iri, query, prefixes, op->clone());
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        auto res = op->get_all_vars();

        if (std::holds_alternative<VarId>(var_or_iri)) {
            auto var = std::get<VarId>(var_or_iri);
            res.insert(var);
        }

        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        return op->get_scope_vars();
    }

    std::set<VarId> get_safe_vars() const override {
        return op->get_safe_vars();
    }

    std::set<VarId> get_fixable_vars() const override {
        auto res = op->get_fixable_vars();

        if (std::holds_alternative<VarId>(var_or_iri)) {
            auto var = std::get<VarId>(var_or_iri);
            res.insert(var);
        }

        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ') << "OpService(";
        if (silent) os << "SILENT ";
        if (std::holds_alternative<VarId>(var_or_iri)) {
            os << get_query_ctx().get_var_name(std::get<VarId>(var_or_iri));
        } else {
            os << '<' << std::get<std::string>(var_or_iri) << ">";
        }

        os << ")\n" << std::string(indent + 2, ' ');

        auto last_char_newline = false;
        for (auto& cha: query) {
            if (cha != '\r') os << cha;
            if (cha == '\n') {
                os << std::string(indent - 2, ' ');
                last_char_newline = true;
            } else {
                last_char_newline = false;
            }
        }

        if (!last_char_newline) {
            os << '\n';
        }

        return os;
    }
};
} // namespace SPARQL
