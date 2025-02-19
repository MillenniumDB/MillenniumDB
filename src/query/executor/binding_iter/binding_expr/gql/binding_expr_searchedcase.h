#pragma once

#include <vector>
#include <memory>

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprSearchedCase : public BindingExpr {
public:
    std::vector<std::pair<std::unique_ptr<BindingExpr>, std::unique_ptr<BindingExpr>>> when_clauses;
    std::unique_ptr<BindingExpr> else_clause;

    BindingExprSearchedCase(
        std::vector<std::pair<std::unique_ptr<BindingExpr>, std::unique_ptr<BindingExpr>>> when_clauses,
        std::unique_ptr<BindingExpr> else_clause
        ) :
        when_clauses(std::move(when_clauses)),
        else_clause(std::move(else_clause))
    { }

    ObjectId eval(const Binding& binding) override
    {
        for (const auto& clause : when_clauses) {
            auto condition = clause.first->eval(binding);
            if (condition == ObjectId(ObjectId::BOOL_TRUE)) {
                return clause.second->eval(binding);
            }
        }
        return else_clause ? else_clause->eval(binding) : ObjectId::get_null();
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};
} // namespace GQL
