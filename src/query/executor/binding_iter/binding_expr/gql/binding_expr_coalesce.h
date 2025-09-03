#pragma once

#include <memory>
#include <vector>

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/gql/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace GQL {
class BindingExprCoalesce : public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> expressions;

    BindingExprCoalesce(std::vector<std::unique_ptr<BindingExpr>> expressions) :
        expressions(std::move(expressions))
    { }

    ObjectId eval(const Binding& binding) override
    {
        for (const auto& expr : expressions) {
            auto value = expr->eval(binding);
            if (!value.is_null()) {
                return value;
            }
        }
        return ObjectId::get_null();
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "COALESCE(";
        for (const auto& expr : expressions) {
            expr->print(os, ops);
            os << ", ";
        }
        os << ')';
    }
};
} // namespace GQL
