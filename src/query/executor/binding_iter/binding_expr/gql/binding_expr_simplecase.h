#pragma once

#include <memory>
#include <vector>

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

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

        auto case_type = GQL_OID::get_generic_type(case_value);

        for (const auto& clause : when_clauses) {
            for (const auto& when_expr : clause.first.second) {
                auto when_value = when_expr->eval(binding);

                auto when_type = GQL_OID::get_generic_type(when_value);

                bool is_match = false;

                if (case_type == when_type) {
                    switch (case_type) {
                    case GQL_OID::GenericType::STRING:
                        is_match =
                            (Conversions::unpack_string(case_value) == Conversions::unpack_string(when_value)
                            );
                        break;
                    case GQL_OID::GenericType::NUMERIC: {
                        auto numeric_type = GQL_OID::get_generic_sub_type(case_value);
                        if (numeric_type == GQL_OID::GenericSubType::INTEGER) {
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
                                    (Conversions::to_integer(case_value) > Conversions::to_integer(when_value)
                                    );
                            } else if (clause.first.first == "<") {
                                is_match =
                                    (Conversions::to_integer(case_value) < Conversions::to_integer(when_value)
                                    );
                            } else if (clause.first.first == ">=") {
                                is_match =
                                    (Conversions::to_integer(case_value)
                                     >= Conversions::to_integer(when_value));
                            } else if (clause.first.first == "<=") {
                                is_match =
                                    (Conversions::to_integer(case_value)
                                     <= Conversions::to_integer(when_value));
                            }
                        } else if (numeric_type == GQL_OID::GenericSubType::FLOAT) {
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
                        } else if (numeric_type == GQL_OID::GenericSubType::DOUBLE) {
                            if (clause.first.first == "=") {
                                is_match =
                                    (Conversions::to_double(case_value) == Conversions::to_double(when_value)
                                    );
                            } else if (clause.first.first == "!=") {
                                is_match =
                                    (Conversions::to_double(case_value) != Conversions::to_double(when_value)
                                    );
                            } else if (clause.first.first == ">") {
                                is_match =
                                    (Conversions::to_double(case_value) > Conversions::to_double(when_value));
                            } else if (clause.first.first == "<") {
                                is_match =
                                    (Conversions::to_double(case_value) < Conversions::to_double(when_value));
                            } else if (clause.first.first == ">=") {
                                is_match =
                                    (Conversions::to_double(case_value) >= Conversions::to_double(when_value)
                                    );
                            } else if (clause.first.first == "<=") {
                                is_match =
                                    (Conversions::to_double(case_value) <= Conversions::to_double(when_value)
                                    );
                            }
                        } else if (numeric_type == GQL_OID::GenericSubType::DECIMAL) {
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
                                    (Conversions::to_decimal(case_value) > Conversions::to_decimal(when_value)
                                    );
                            } else if (clause.first.first == "<") {
                                is_match =
                                    (Conversions::to_decimal(case_value) < Conversions::to_decimal(when_value)
                                    );
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
                    case GQL_OID::GenericType::BOOL:
                        is_match =
                            (Conversions::to_boolean(case_value) == Conversions::to_boolean(when_value));
                        break;
                    case GQL_OID::GenericType::DATE: {
                        bool error;
                        is_match = DateTime(case_value)
                                       .compare<DateTimeComparisonMode::StrictEquality>(
                                           DateTime(when_value),
                                           &error
                                       )
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
};
} // namespace GQL
