#pragma once

#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>
#include <string>

namespace GQL {

class BindingExprCast : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> operand;
    ObjectGenType targetType;

    BindingExprCast(std::unique_ptr<BindingExpr> operand, ObjectGenType targetType) :
        operand(std::move(operand)),
        targetType(std::move(targetType))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto operand_oid = operand->eval(binding);
        if (operand_oid.is_null()) {
            return ObjectId::get_null();
        }

        auto sourceType = operand_oid.generic_type();

        if (sourceType == targetType) {
            return operand_oid;
        }

        switch (targetType) {
        case ObjectGenType::Bool:
            switch (sourceType) {
            case ObjectGenType::Numeric: {
                auto number = GQL::Conversions::to_integer(operand_oid);
                auto boolean = number != 0;
                return GQL::Conversions::pack_bool(boolean);
            }
            case ObjectGenType::String:
                return GQL::Conversions::pack_bool(GQL::Conversions::to_lexical_str(operand_oid) == "true");
            default:
                return ObjectId::get_null();
            }

        case ObjectGenType::Numeric:
            switch (sourceType) {
            case ObjectGenType::Numeric:
                return operand_oid;
            case ObjectGenType::Bool: {
                auto boolean = GQL::Conversions::to_boolean(operand_oid);
                auto boolean_value = boolean == GQL::Conversions::pack_bool(true);
                auto number = boolean_value ? 1 : 0;
                return GQL::Conversions::pack_int(number);
            }
            case ObjectGenType::String: {
                std::string str = GQL::Conversions::to_lexical_str(operand_oid);
                double val;
                auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), val);

                if (ec == std::errc() && ptr == str.data() + str.size()) {
                    return GQL::Conversions::pack_int(val);
                } else {
                    return ObjectId::get_null();
                }
            }
            default:
                return ObjectId::get_null();
            }

        case ObjectGenType::TemporalLiteral:
            if (sourceType == ObjectGenType::String) {
                return ObjectId(DateTime::from_dateTime(Conversions::to_lexical_str(operand_oid)));
            }
            return ObjectId::get_null();

        case ObjectGenType::String:
            switch (sourceType) {
            case ObjectGenType::Numeric:
            case ObjectGenType::Bool:
            case ObjectGenType::TemporalLiteral:
                return GQL::Conversions::pack_string(GQL::Conversions::to_lexical_str(operand_oid));
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

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "CAST(";
        operand->print(os, ops);
        os << " AS ";
        if (targetType == ObjectGenType::Bool) {
            os << "BOOL";
        } else if (targetType == ObjectGenType::Numeric) {
            os << "NUMERIC";
        } else if (targetType == ObjectGenType::TemporalLiteral) {
            os << "DATE";
        } else if (targetType == ObjectGenType::String) {
            os << "STRING";
        }
        os << ")";
    }
};

} // namespace GQL
