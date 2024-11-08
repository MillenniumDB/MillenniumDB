#pragma once

#include <memory>

#include "query/exceptions.h"
#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprDivision : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprDivision(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)), rhs(std::move(rhs)) { }

    ObjectId eval(const Binding& binding) override {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto optype = SPARQL::Conversions::calculate_optype(lhs_oid, rhs_oid);

        switch (optype) {
        case SPARQL::Conversions::OPTYPE_INTEGER: {
            // In SPARQL-1.1 integer division is done using decimals, and the result is a decimal
            auto rhs = SPARQL::Conversions::to_decimal(rhs_oid);
            if (rhs == 0) {
                return ObjectId::get_null();
            }
            auto lhs = SPARQL::Conversions::to_decimal(lhs_oid);
            return SPARQL::Conversions::pack_decimal(lhs / rhs);
        }
        case SPARQL::Conversions::OPTYPE_DECIMAL: {
            auto rhs = SPARQL::Conversions::to_decimal(rhs_oid);
              if (rhs == 0) {
                return ObjectId::get_null();
            }
            auto lhs = SPARQL::Conversions::to_decimal(lhs_oid);
            return SPARQL::Conversions::pack_decimal(lhs / rhs);
        }
        case SPARQL::Conversions::OPTYPE_FLOAT: {
            auto lhs = SPARQL::Conversions::to_float(lhs_oid);
            auto rhs = SPARQL::Conversions::to_float(rhs_oid);
            // Division by zero, etc is handle by the floating point implementation.
            // SPARQL 1.1 follows IEEE 754-2008.
            return SPARQL::Conversions::pack_float(lhs / rhs);
        }
        case SPARQL::Conversions::OPTYPE_DOUBLE: {
            auto lhs = SPARQL::Conversions::to_double(lhs_oid);
            auto rhs = SPARQL::Conversions::to_double(rhs_oid);
            // Division by zero, etc is handle by the floating point implementation.
            // SPARQL 1.1 follows IEEE 754-2008.
            return SPARQL::Conversions::pack_double(lhs / rhs);
        }
        case SPARQL::Conversions::OPTYPE_INVALID: {
            return ObjectId::get_null();
        }
        default:
            throw LogicException("This should never happen");
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace MQL
