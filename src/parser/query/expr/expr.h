#pragma once

#include <set>

#include "base/query/var.h"
#include "parser/query/expr/expr_visitor.h"

class Expr {
public:
    virtual ~Expr() = default;

    virtual void accept_visitor(ExprVisitor&) = 0;

    virtual std::set<Var> get_vars() const = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Expr& b) {
        return b.print_to_ostream(os);
    }
};
