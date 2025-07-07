#pragma once

#include <cstdint>

#include "network/sparql/service/response_parser.h"
#include "query/executor/binding_iter.h"

class SparqlService : public BindingIter {
public:
    SparqlService(
        bool silent,
        std::string&& query,
        std::string&& prefixes,
        std::variant<VarId, std::string> var_or_iri,
        std::set<VarId> _scope_vars,
        std::set<VarId> _fixed_vars,
        std::set<VarId> fixed_join_vars
    ) :
        scope_vars(std::move(_scope_vars)),
        fixed_vars(std::move(_fixed_vars)),
        fixed_join_vars(std::move(fixed_join_vars)),
        silent(silent),
        response_parser(
            ResponseParser(std::move(query), std::move(prefixes), var_or_iri, scope_vars, fixed_vars)
        )
    { }

    void print(std::ostream& os, int indent, bool stats) const override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    // Variables that are in scope after evaluating this service
    std::set<VarId> scope_vars;

    // Variables fixed by parent
    std::set<VarId> fixed_vars;

    // Variables fixed by parent that are involved in joins with other operators
    std::set<VarId> fixed_join_vars;

    bool silent;

    ResponseParser response_parser;

    uint64_t network_requests = 0;

private:
    Binding* parent_binding;

    bool failed; // Used when the SILENT token is present
    bool first_next; // Try to consume API in the first next
};
