#pragma once

#include "parser/query/return_item/return_item.h"

class ReturnItemAgg : public ReturnItem {
public:
    std::string aggregate_func;

    std::string inside_var;

    ReturnItemAgg(const std::string& aggregate_func, std::string&& inside_var) :
        aggregate_func (aggregate_func),
        inside_var     (std::move(inside_var)) { }

    Var get_var() const override {
        return Var(aggregate_func + '(' + inside_var + ')');
    }

    std::set<Var> get_vars() const override {
        std::set<Var> res { Var(inside_var), Var(aggregate_func + '(' + inside_var + ')') };
        return res;
    }

    void accept_visitor(ReturnItemVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        return os << std::string(' ', indent) << aggregate_func << '(' << inside_var << ')';
    }
};
