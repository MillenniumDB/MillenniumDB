#pragma once

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <cmath>
#include <memory>

namespace GQL {

class BindingExprEquals : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprEquals(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto lhs_subtype = lhs_oid.subtype();
        auto rhs_subtype = rhs_oid.subtype();

        auto lhs_generic_type = lhs_oid.generic_type();
        auto rhs_generic_type = rhs_oid.generic_type();

        // Nulls are not equal to anything, including other nulls.
        if (lhs_oid.is_null() || rhs_oid.is_null()) {
            return ObjectId::get_null();
        }

        // Check if the ids are equal
        if (lhs_oid == rhs_oid) {
            // For floats NaN != NaN, so we have to check for that case.
            if (lhs_subtype == ObjectSubType::Float) {
                auto f = GQL::Conversions::unpack_float(lhs_oid);
                if (std::isnan(f)) {
                    return GQL::Conversions::pack_bool(false);
                }
            } else if (lhs_subtype == ObjectSubType::Double) {
                auto d = GQL::Conversions::unpack_double(lhs_oid);
                if (std::isnan(d)) {
                    return GQL::Conversions::pack_bool(false);
                }
            }
            return GQL::Conversions::pack_bool(true);
        }

        if (lhs_generic_type == ObjectGenType::Bool && rhs_generic_type == ObjectGenType::Bool) {
            auto lhs = GQL::Conversions::to_boolean(lhs_oid);
            auto rhs = GQL::Conversions::to_boolean(rhs_oid);
            return GQL::Conversions::pack_bool(lhs == rhs);
        }

        // If both types are numeric we need to do a numeric comparison
        if (lhs_generic_type == ObjectGenType::Numeric && rhs_generic_type == ObjectGenType::Numeric) {
            auto optype = GQL::Conversions::calculate_optype(lhs_oid, rhs_oid);
            switch (optype) {
            case GQL::Conversions::OpType::INTEGER: {
                auto lhs = GQL::Conversions::to_integer(lhs_oid);
                auto rhs = GQL::Conversions::to_integer(rhs_oid);
                return GQL::Conversions::pack_bool(lhs == rhs);
            }
            case GQL::Conversions::OpType::DECIMAL: {
                auto lhs = GQL::Conversions::to_decimal(lhs_oid);
                auto rhs = GQL::Conversions::to_decimal(rhs_oid);
                return GQL::Conversions::pack_bool(lhs == rhs);
            }
            case GQL::Conversions::OpType::FLOAT: {
                auto lhs = GQL::Conversions::to_float(lhs_oid);
                auto rhs = GQL::Conversions::to_float(rhs_oid);
                return GQL::Conversions::pack_bool(lhs == rhs);
            }
            case GQL::Conversions::OpType::DOUBLE: {
                auto lhs = GQL::Conversions::to_double(lhs_oid);
                auto rhs = GQL::Conversions::to_double(rhs_oid);
                return GQL::Conversions::pack_bool(lhs == rhs);
            }
            case GQL::Conversions::OpType::INVALID: {
                return ObjectId::get_null();
            }
            default:
                throw std::runtime_error("This should never happen.");
            }
        }

        // Handle date, time, dateTime and dateTimeStamp
        if (lhs_generic_type == ObjectGenType::TemporalLiteral
            && rhs_generic_type == ObjectGenType::TemporalLiteral)
        {
            bool error;
            auto res = DateTime(lhs_oid.id).compare<DTCompare::StrictEq>(DateTime(rhs_oid.id), &error) == 0;
            if (error) {
                return ObjectId::get_null();
            }
            return GQL::Conversions::pack_bool(res);
        }

        // We have to handle simple literals
        if (lhs_subtype == ObjectSubType::String && rhs_subtype == ObjectSubType::String) {
            auto equals = GQL::Conversions::to_lexical_str(lhs_oid)
                       == GQL::Conversions::to_lexical_str(rhs_oid);
            return GQL::Conversions::pack_bool(equals);
        }

        if (lhs_subtype == ObjectSubType::Dictionary && rhs_subtype == ObjectSubType::Dictionary) {
            std::unique_ptr<Dictionary> lhs_dict;
            std::unique_ptr<Dictionary> rhs_dict;
            Common::Conversions::unpack_dictionary(lhs_oid, lhs_dict);
            Common::Conversions::unpack_dictionary(rhs_oid, rhs_dict);
            Dictionary& lhs(*lhs_dict);
            Dictionary& rhs(*rhs_dict);
            return GQL::Conversions::pack_bool(lhs == rhs);
        }

        if (lhs_subtype == ObjectSubType::List && rhs_subtype == ObjectSubType::List) {
            std::vector<ObjectId> lhs_list = Conversions::unpack_list(lhs_oid);
            std::vector<ObjectId> rhs_list = Conversions::unpack_list(rhs_oid);

            return GQL::Conversions::pack_bool(lhs_list == rhs_list);
        }

        return ObjectId::get_null();
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << '(';
        lhs->print(os, ops);
        os << " = ";
        rhs->print(os, ops);
        os << ')';
    }
};
} // namespace GQL
