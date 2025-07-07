#pragma once

#include "query/parser/expr/sparql/expr.h"
#include "query/parser/expr/sparql/expr_printer.h"
#include "query/parser/op/sparql/op.h"

namespace SPARQL {

class OpProcedure : public Op {
public:
    enum class ProcedureType : uint8_t {
        HNSW_TOP_K,
        HNSW_SCAN,
        TEXT_SEARCH
    };

    static inline std::string get_procedure_string(ProcedureType procedure_type)
    {
        switch (procedure_type) {
        case ProcedureType::HNSW_TOP_K:
            return "HNSW_TOP_K";
        case ProcedureType::HNSW_SCAN:
            return "HNSW_SCAN";
        case ProcedureType::TEXT_SEARCH:
            return "TEXT_SEARCH";
        default:
            return "UNKNOWN_PROCEDURE";
        }
    }

    ProcedureType procedure_type;

    std::vector<std::unique_ptr<Expr>> argument_exprs;
    std::vector<VarId> binding_vars;

    OpProcedure(
        ProcedureType procedure_type_,
        std::vector<std::unique_ptr<Expr>>&& argument_exprs_,
        std::vector<VarId>&& binding_vars_
    ) :
        procedure_type { procedure_type_ },
        argument_exprs { std::move(argument_exprs_) },
        binding_vars { std::move(binding_vars_) }
    {
        assert(!binding_vars.empty());
    }

    std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> argument_exprs_clone;
        argument_exprs_clone.reserve(argument_exprs.size());
        for (const auto& expr : argument_exprs) {
            argument_exprs_clone.emplace_back(expr->clone());
        }

        auto yield_vars_clone = binding_vars;

        return std::make_unique<OpProcedure>(
            procedure_type,
            std::move(argument_exprs_clone),
            std::move(yield_vars_clone)
        );
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res(binding_vars.begin(), binding_vars.end());
        for (const auto& expr : argument_exprs) {
            res.merge(expr->get_all_vars());
        }
        return res;
    }

    std::set<VarId> get_scope_vars() const override
    {
        std::set<VarId> res(binding_vars.begin(), binding_vars.end());
        return res;
    }

    std::set<VarId> get_safe_vars() const override
    {
        std::set<VarId> res(binding_vars.begin(), binding_vars.end());
        return res;
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        auto expr_printer = ExprPrinter(os);

        os << std::string(indent, ' ');
        os << "OpProcedure(";
        os << get_procedure_string(procedure_type) << ", ";
        if (!argument_exprs.empty()) {
            argument_exprs[0]->accept_visitor(expr_printer);
        }
        for (std::size_t i = 1; i < argument_exprs.size(); ++i) {
            os << ", ";
            argument_exprs[i]->accept_visitor(expr_printer);
        }
        os << ") -> (";
        os << binding_vars[0];
        for (std::size_t i = 1; i < binding_vars.size(); ++i) {
            os << ", " << binding_vars[i];
        }
        return os << ")\n";
    }
};
} // namespace SPARQL
