#pragma once

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

#include <memory>
#include <vector>

namespace GQL {
class BindingExprSimpleCase : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> case_operand;

    std::vector<std::pair<
        std::pair<std::string, std::vector<std::unique_ptr<BindingExpr>>>,
        std::unique_ptr<BindingExpr>>>
        when_clauses;

    std::unique_ptr<BindingExpr> else_expr;

    BindingExprSimpleCase(
        std::unique_ptr<BindingExpr> case_operand,
        std::vector<std::pair<
            std::pair<std::string, std::vector<std::unique_ptr<BindingExpr>>>,
            std::unique_ptr<BindingExpr>>> when_clauses,
        std::unique_ptr<BindingExpr> else_expr
    ) :
        case_operand(std::move(case_operand)),
        when_clauses(std::move(when_clauses)),
        else_expr(std::move(else_expr))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto case_value = case_operand->eval(binding);

        auto case_type = case_value.generic_type();

        for (const auto& clause : when_clauses) {
            for (const auto& when_expr : clause.first.second) {
                auto when_value = when_expr->eval(binding);

                auto when_type = when_value.generic_type();

                bool is_match = false;

                if (case_type == when_type) {
                    switch (case_type) {
                    case ObjectGenType::String:
                        is_match =
                            (Conversions::unpack_string(case_value)
                             == Conversions::unpack_string(when_value));
                        break;
                    case ObjectGenType::Numeric: {
                        auto numeric_type = case_value.subtype();
                        if (numeric_type == ObjectSubType::Int) {
                            if (clause.first.first == "=") {
                                is_match =
                                    (Conversions::to_integer(case_value)
                                     == Conversions::to_integer(when_value));
                            } else if (clause.first.first == "!=") {
                                is_match =
                                    (Conversions::to_integer(case_value)
                                     != Conversions::to_integer(when_value));
                            } else if (clause.first.first == ">") {
                                is_match =
                                    (Conversions::to_integer(case_value)
                                     > Conversions::to_integer(when_value));
                            } else if (clause.first.first == "<") {
                                is_match =
                                    (Conversions::to_integer(case_value)
                                     < Conversions::to_integer(when_value));
                            } else if (clause.first.first == ">=") {
                                is_match =
                                    (Conversions::to_integer(case_value)
                                     >= Conversions::to_integer(when_value));
                            } else if (clause.first.first == "<=") {
                                is_match =
                                    (Conversions::to_integer(case_value)
                                     <= Conversions::to_integer(when_value));
                            }
                        } else if (numeric_type == ObjectSubType::Float) {
                            if (clause.first.first == "=") {
                                is_match =
                                    (Conversions::to_float(case_value) == Conversions::to_float(when_value));
                            } else if (clause.first.first == "!=") {
                                is_match =
                                    (Conversions::to_float(case_value) != Conversions::to_float(when_value));
                            } else if (clause.first.first == ">") {
                                is_match =
                                    (Conversions::to_float(case_value) > Conversions::to_float(when_value));
                            } else if (clause.first.first == "<") {
                                is_match =
                                    (Conversions::to_float(case_value) < Conversions::to_float(when_value));
                            } else if (clause.first.first == ">=") {
                                is_match =
                                    (Conversions::to_float(case_value) >= Conversions::to_float(when_value));
                            } else if (clause.first.first == "<=") {
                                is_match =
                                    (Conversions::to_float(case_value) <= Conversions::to_float(when_value));
                            }
                        } else if (numeric_type == ObjectSubType::Double) {
                            if (clause.first.first == "=") {
                                is_match =
                                    (Conversions::to_double(case_value)
                                     == Conversions::to_double(when_value));
                            } else if (clause.first.first == "!=") {
                                is_match =
                                    (Conversions::to_double(case_value)
                                     != Conversions::to_double(when_value));
                            } else if (clause.first.first == ">") {
                                is_match =
                                    (Conversions::to_double(case_value) > Conversions::to_double(when_value));
                            } else if (clause.first.first == "<") {
                                is_match =
                                    (Conversions::to_double(case_value) < Conversions::to_double(when_value));
                            } else if (clause.first.first == ">=") {
                                is_match =
                                    (Conversions::to_double(case_value)
                                     >= Conversions::to_double(when_value));
                            } else if (clause.first.first == "<=") {
                                is_match =
                                    (Conversions::to_double(case_value)
                                     <= Conversions::to_double(when_value));
                            }
                        } else if (numeric_type == ObjectSubType::Decimal) {
                            if (clause.first.first == "=") {
                                is_match =
                                    (Conversions::to_decimal(case_value)
                                     == Conversions::to_decimal(when_value));
                            } else if (clause.first.first == "!=") {
                                is_match =
                                    (Conversions::to_decimal(case_value)
                                     != Conversions::to_decimal(when_value));
                            } else if (clause.first.first == ">") {
                                is_match =
                                    (Conversions::to_decimal(case_value)
                                     > Conversions::to_decimal(when_value));
                            } else if (clause.first.first == "<") {
                                is_match =
                                    (Conversions::to_decimal(case_value)
                                     < Conversions::to_decimal(when_value));
                            } else if (clause.first.first == ">=") {
                                is_match =
                                    (Conversions::to_decimal(case_value)
                                     >= Conversions::to_decimal(when_value));
                            } else if (clause.first.first == "<=") {
                                is_match =
                                    (Conversions::to_decimal(case_value)
                                     <= Conversions::to_decimal(when_value));
                            }
                        }
                        break;
                    }
                    case ObjectGenType::Bool:
                        is_match =
                            (Conversions::to_boolean(case_value) == Conversions::to_boolean(when_value));
                        break;
                    case ObjectGenType::TemporalLiteral: {
                        bool error;
                        is_match = DateTime(case_value)
                                       .compare<DTCompare::StrictEq>(DateTime(when_value), &error)
                                == 0;
                        break;
                    }
                    default:
                        break;
                    }
                } else if (clause.first.first == "!=") {
                    is_match = true;
                }

                if (is_match) {
                    return clause.second->eval(binding);
                }
            }
        }

        return else_expr ? else_expr->eval(binding) : ObjectId::get_null();
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "CASE ";
        case_operand->print(os, ops);
        for (const auto& clauses : when_clauses) {
            os << " WHEN ";
            for (const auto& clause : clauses.first.second) {
                os << clauses.first.first;
                clause->print(os, ops);
                os << ", ";
            }
            os << " THEN ";
            clauses.second->print(os, ops);
        }
        os << " ELSE ";
        if (else_expr == nullptr) {
            os << "NULL";
        } else {
            else_expr->print(os, ops);
        }
    }
};
} // namespace GQL
