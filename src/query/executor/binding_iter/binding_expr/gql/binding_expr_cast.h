#pragma once

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include <memory>
#include <string>

namespace GQL {

class BindingExprCast : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> operand;
    GQL_OID::GenericType targetType;

    BindingExprCast(std::unique_ptr<BindingExpr> operand, GQL_OID::GenericType targetType) :
        operand(std::move(operand)),
        targetType(std::move(targetType))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto operand_oid = operand->eval(binding);
        if (operand_oid.is_null()) {
            return ObjectId::get_null();
        }

        auto sourceType = GQL_OID::get_generic_type(operand_oid);

        if (sourceType == targetType) {
            return operand_oid;
        }

        switch (targetType) {
        case GQL_OID::GenericType::BOOL:
            switch (sourceType) {
            case GQL_OID::GenericType::NUMERIC: {
                auto number = GQL::Conversions::to_integer(operand_oid);
                auto boolean = number != 0;
                return GQL::Conversions::pack_bool(boolean);
            }
            case GQL_OID::GenericType::STRING:
                return GQL::Conversions::pack_bool(GQL::Conversions::to_lexical_str(operand_oid) == "true");
            default:
                return ObjectId::get_null();
            }

        case GQL_OID::GenericType::NUMERIC:
            switch (sourceType) {
            case GQL_OID::GenericType::NUMERIC:
                return operand_oid;
            case GQL_OID::GenericType::BOOL: {
                auto boolean = GQL::Conversions::to_boolean(operand_oid);
                auto boolean_value = boolean == GQL::Conversions::pack_bool(true);
                auto number = boolean_value ? 1 : 0;
                return GQL::Conversions::pack_int(number);
            }
            case GQL_OID::GenericType::STRING: {
                std::string str = GQL::Conversions::to_lexical_str(operand_oid);
                try {
                    return GQL::Conversions::pack_int(std::stod(str));
                } catch (...) {
                    return ObjectId::get_null();
                }
            }
            default:
                return ObjectId::get_null();
            }

        case GQL_OID::GenericType::DATE:
            if (sourceType == GQL_OID::GenericType::STRING) {
                try {
                    return ObjectId(DateTime::from_dateTime(Conversions::to_lexical_str(operand_oid)));
                } catch (...) {
                    return ObjectId::get_null();
                }
            }
            return ObjectId::get_null();

        case GQL_OID::GenericType::STRING:
            switch (sourceType) {
            case GQL_OID::GenericType::NUMERIC:
            case GQL_OID::GenericType::BOOL:
            case GQL_OID::GenericType::DATE:
                return GQL::Conversions::pack_string_simple(GQL::Conversions::to_lexical_str(operand_oid));
            default:
                return ObjectId::get_null();
            }

        default:
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "CAST(";
        operand->print(os, ops);
        os << " AS ";
        if (targetType == GQL_OID::GenericType::BOOL) {
            os << "BOOL";
        } else if (targetType == GQL_OID::GenericType::NUMERIC) {
            os << "NUMERIC";
        } else if (targetType == GQL_OID::GenericType::DATE) {
            os << "DATE";
        } else if (targetType == GQL_OID::GenericType::STRING) {
            os << "STRING";
        }
        os << ")";
    }
};

} // namespace GQL
