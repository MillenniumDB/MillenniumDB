#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/mql/expr.h"

namespace MQL {
class ExprOr : public Expr {
public:
    std::vector<std::unique_ptr<Expr>> or_list;

    ExprOr(std::vector<std::unique_ptr<Expr>>&& or_list) :
        or_list(std::move(or_list))
    { }

    void accept_visitor(ExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    bool has_aggregation() const override
    {
        for (auto& expr : or_list) {
            if (expr->has_aggregation())
                return true;
        }
        return false;
    }

    virtual std::unique_ptr<Expr> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> or_list_clone;
        or_list_clone.reserve(or_list.size());
        for (auto& child_expr : or_list) {
            or_list_clone.push_back(child_expr->clone());
        }
        return std::make_unique<ExprOr>(std::move(or_list_clone));
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;
        for (auto& expr : or_list) {
            for (auto& var : expr->get_all_vars()) {
                res.insert(var);
            }
        }
        return res;
    }

    std::set<VarId> get_input_vars() const override
    {
        std::set<VarId> res;
        for (auto& expr : or_list) {
            for (auto& var : expr->get_all_vars()) {
                res.insert(var);
            }
        }
        return res;
    }

    void print(std::ostream& os) const override
    {
        os << '(';
        os << *or_list[0];
        for (size_t i = 1; i < or_list.size(); i++) {
            os << " OR ";
            os << *or_list[i];
        }
        os << ')';
    }
};
} // namespace MQL
