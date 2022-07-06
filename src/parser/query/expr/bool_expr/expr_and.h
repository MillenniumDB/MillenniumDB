#pragma once

#include <memory>
#include <vector>

#include "parser/query/expr/expr.h"

class ExprAnd : public Expr {
public:
    std::vector<std::unique_ptr<Expr>> and_list;

    ExprAnd(std::vector<std::unique_ptr<Expr>>&& and_list) :
        and_list (std::move(and_list)) { }

    void accept_visitor(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res;
        for (auto& or_element: and_list) {
            for (auto& var : or_element->get_vars()) {
                res.insert(var);
            }
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(' ', indent)
           << '(' << *and_list[0];
        for (std::size_t i = 1; i < and_list.size(); i++) {
            os << " AND " << *and_list[i];
        }
        return os << ')';
    }
};
