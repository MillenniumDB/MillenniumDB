#pragma once

#include <memory>
#include <vector>

#include "parser/query/expr/expr.h"

class ExprOr : public Expr {
public:
    std::vector<std::unique_ptr<Expr>> or_list;

    ExprOr(std::vector<std::unique_ptr<Expr>>&& or_list) :
        or_list (std::move(or_list)) { }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        for (auto& or_element: or_list) {
            for (auto& var : or_element->get_vars()) {
                res.insert(var);
            }
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(' ', indent)
           << '(' << *or_list[0];
        for (std::size_t i = 1; i < or_list.size(); i++) {
            os << " OR " << *or_list[i];
        }
        return os << ')';
    }
};
