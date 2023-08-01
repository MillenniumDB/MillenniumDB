#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/expr.h"

namespace MQL {
class ExprAnd : public Expr {
public:
    std::vector<std::unique_ptr<Expr>> and_list;

    ExprAnd(std::vector<std::unique_ptr<Expr>>&& and_list) :
        and_list (std::move(and_list)) { }

    virtual std::unique_ptr<Expr> clone() const override {
        std::vector<std::unique_ptr<Expr>> and_list_clone;
        and_list_clone.reserve(and_list.size());
        for (auto& child_expr : and_list) {
            and_list_clone.push_back(child_expr->clone());
        }
        return std::make_unique<ExprAnd>(std::move(and_list_clone));
    }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    bool has_aggregation() const override {
        for (auto& expr : and_list) {
            if (expr->has_aggregation())
                return true;
        }
        return false;
    }

    std::set<VarId> get_all_vars() const override {
        std::set<VarId> res;
        for (auto& expr: and_list) {
            for (auto& var : expr->get_all_vars()) {
                res.insert(var);
            }
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ')
           << '(' << *and_list[0];
        for (std::size_t i = 1; i < and_list.size(); i++) {
            os << " AND " << *and_list[i];
        }
        return os << ')';
    }
};
} // namespace MQL
