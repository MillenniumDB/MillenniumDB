#pragma once

#include <memory>
#include <vector>

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

    void print(std::ostream& os, std::vector<BindingIter*> ops) const override
    {
        os << "CASE ";
        for (const auto& clause : when_clauses) {
            os << " WHEN ";
            clause.first->print(os, ops);
            os << " THEN ";
            clause.first->print(os, ops);
        }
        os << " ELSE ";
        if (else_clause == nullptr) {
            os << "NULL";
        } else {
            else_clause->print(os, ops);
        }
    }
};
} // namespace GQL
