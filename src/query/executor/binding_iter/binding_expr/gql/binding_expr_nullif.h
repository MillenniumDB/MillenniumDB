#pragma once

#include <memory>

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {

class BindingExprNullIf : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprNullIf(std::unique_ptr<BindingExpr> lhs, std::unique_ptr<BindingExpr> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto lhs_oid = lhs->eval(binding);
        auto rhs_oid = rhs->eval(binding);

        auto lhs_subtype = GQL_OID::get_generic_sub_type(lhs_oid);
        auto rhs_subtype = GQL_OID::get_generic_sub_type(rhs_oid);

        auto lhs_generic_type = GQL_OID::get_generic_type(lhs_oid);
        auto rhs_generic_type = GQL_OID::get_generic_type(rhs_oid);

        if (lhs_oid.is_null()) {
            return ObjectId::get_null();
        }

        if (rhs_oid.is_null()) {
            return lhs_oid;
        }

        if (lhs_oid == rhs_oid) {
            return ObjectId::get_null();
        }

        if (lhs_generic_type == GQL_OID::GenericType::NUMERIC
            && rhs_generic_type == GQL_OID::GenericType::NUMERIC)
        {
            auto optype = GQL::Conversions::calculate_optype(lhs_oid, rhs_oid);
            switch (optype) {
            case GQL::Conversions::OpType::INTEGER: {
                auto lhs = GQL::Conversions::to_integer(lhs_oid);
                auto rhs = GQL::Conversions::to_integer(rhs_oid);
                if (lhs == rhs) {
                    return ObjectId::get_null();
                }
                return GQL::Conversions::pack_int(lhs);
            }
            case GQL::Conversions::OpType::DECIMAL: {
                auto lhs = GQL::Conversions::to_decimal(lhs_oid);
                auto rhs = GQL::Conversions::to_decimal(rhs_oid);
                if (lhs == rhs) {
                    return ObjectId::get_null();
                }
                return GQL::Conversions::pack_decimal(lhs);
            }
            case GQL::Conversions::OpType::FLOAT: {
                auto lhs = GQL::Conversions::to_float(lhs_oid);
                auto rhs = GQL::Conversions::to_float(rhs_oid);
                if (lhs == rhs) {
                    return ObjectId::get_null();
                }
                return GQL::Conversions::pack_float(lhs);
            }
            case GQL::Conversions::OpType::DOUBLE: {
                auto lhs = GQL::Conversions::to_double(lhs_oid);
                auto rhs = GQL::Conversions::to_double(rhs_oid);
                if (lhs == rhs) {
                    return ObjectId::get_null();
                }
                return GQL::Conversions::pack_double(lhs);
            }
            case GQL::Conversions::OpType::INVALID: {
                return ObjectId::get_null();
            }
            default:
                throw std::runtime_error("This should never happen.");
            }
        }

        // Handle date, time, dateTime and dateTimeStamp
        if (lhs_generic_type == GQL_OID::GenericType::DATE && rhs_generic_type == GQL_OID::GenericType::DATE)
        {
            bool error;
            auto res = DateTime(lhs_oid.id)
                           .compare<DateTimeComparisonMode::StrictEquality>(DateTime(rhs_oid.id), &error)
                    == 0;
            if (error) {
                return ObjectId::get_null();
            }
            if (res) {
                return ObjectId::get_null();
            }
            return GQL::Conversions::pack_date(DateTime(lhs_oid.id));
        }

        if (lhs_subtype == GQL_OID::GenericSubType::STRING_SIMPLE
            && rhs_subtype == GQL_OID::GenericSubType::STRING_SIMPLE)
        {
            auto equals = GQL::Conversions::to_lexical_str(lhs_oid)
                       == GQL::Conversions::to_lexical_str(rhs_oid);
            if (equals) {
                return ObjectId::get_null();
            }
            return lhs_oid;
        }

        if (lhs_subtype == GQL_OID::GenericSubType::LIST && rhs_subtype == GQL_OID::GenericSubType::LIST) {
            std::vector<ObjectId> lhs_list = Conversions::unpack_list(lhs_oid);
            std::vector<ObjectId> rhs_list = Conversions::unpack_list(rhs_oid);
            if (lhs_list == rhs_list) {
                return ObjectId::get_null();
            }
        }

        if (lhs_subtype == GQL_OID::GenericSubType::DICTIONARY
            && rhs_subtype == GQL_OID::GenericSubType::DICTIONARY)
        {
            std::unique_ptr<Dictionary> lhs_dict;
            std::unique_ptr<Dictionary> rhs_dict;
            Common::Conversions::unpack_dictionary(lhs_oid, lhs_dict);
            Common::Conversions::unpack_dictionary(rhs_oid, rhs_dict);

            Dictionary& lhs(*lhs_dict);
            Dictionary& rhs(*rhs_dict);
            if (lhs == rhs) {
                return ObjectId::get_null();
            }
        }

        return lhs_oid;
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "NULLIF(";
        lhs->print(os, ops);
        os << ", ";
        rhs->print(os, ops);
        os << ')';
    }
};
} // namespace GQL
