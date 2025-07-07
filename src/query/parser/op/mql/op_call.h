#pragma once

#include "query/parser/expr/mql/expr.h"
#include "query/parser/expr/mql/expr_printer.h"
#include "query/parser/op/mql/op.h"

namespace MQL {

class OpCall : public Op {
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

    static inline std::vector<std::string>
        get_procedure_available_yield_variable_names(ProcedureType procedure_type)
    {
        switch (procedure_type) {
        case ProcedureType::HNSW_TOP_K:
        case ProcedureType::HNSW_SCAN:
            return { "object", "distance" };
        case ProcedureType::TEXT_SEARCH:
            return { "object", "match" };
        default:
            throw NotSupportedException(
                "OpCall::get_procedure_available_yield_variable_names: Unhandled procedure type: "
                + std::to_string(static_cast<uint8_t>(procedure_type))
            );
        }
    }

    ProcedureType procedure_type;

    std::vector<std::unique_ptr<Expr>> argument_exprs;
    std::vector<VarId> yield_vars;

    OpCall(
        ProcedureType procedure_type_,
        std::vector<std::unique_ptr<Expr>>&& argument_exprs_,
        std::vector<VarId>&& yield_vars_
    ) :
        procedure_type { procedure_type_ },
        argument_exprs { std::move(argument_exprs_) },
        yield_vars { std::move(yield_vars_) }
    {
        assert(!yield_vars.empty());
    }

    std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> argument_exprs_clone;
        argument_exprs_clone.reserve(argument_exprs.size());
        for (const auto& expr : argument_exprs) {
            argument_exprs_clone.emplace_back(expr->clone());
        }

        auto yield_vars_clone = yield_vars;

        return std::make_unique<OpCall>(
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
        std::set<VarId> res(yield_vars.begin(), yield_vars.end());
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        auto expr_printer = ExprPrinter(os);

        os << std::string(indent, ' ');
        os << "OpCall(";
        os << get_procedure_string(procedure_type) << ", ";
        argument_exprs[0]->accept_visitor(expr_printer);
        for (std::size_t i = 1; i < argument_exprs.size(); ++i) {
            os << ", ";
            argument_exprs[i]->accept_visitor(expr_printer);
        }
        os << ") -> (";
        os << yield_vars[0];
        for (std::size_t i = 1; i < yield_vars.size(); ++i) {
            os << ", " << yield_vars[i];
        }
        return os << ")\n";
    }
};
} // namespace MQL
