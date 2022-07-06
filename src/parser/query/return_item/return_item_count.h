#pragma once

#include "parser/query/return_item/return_item.h"

class ReturnItemCount : public ReturnItem {
public:
    bool distinct;

    std::string inside_var;

    ReturnItemCount(bool distinct, std::string&& inside_var) :
        distinct   (distinct),
        inside_var (std::move(inside_var)) { }

    Var get_var() const override {
        if (distinct) {
            return Var("COUNT(DISTINCT " + inside_var + ")");
        } else {
            return Var("COUNT(" + inside_var + ")");
        }
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res { Var(inside_var) };
        if (distinct) {
            res.emplace("COUNT(DISTINCT " + inside_var + ')');
        } else {
            res.emplace("COUNT(" + inside_var + ')');
        }
        return res;
    }

    void accept_visitor(ReturnItemVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(' ', indent) << "COUNT" << '(' << (distinct ? "DISTINCT " : "") << inside_var << ')';
    }
};
