#pragma once

#include <set>
#include <memory>
#include <ostream>

#include "query/var_id.h"
#include "query/parser/expr/expr_visitor.h"

class Expr {
public:
    virtual ~Expr() = default;

    virtual std::unique_ptr<Expr> clone() const = 0;

    virtual void accept_visitor(ExprVisitor&) = 0;

    virtual std::set<VarId> get_all_vars() const = 0;

    virtual bool has_aggregation() const = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Expr& b) {
        return b.print_to_ostream(os);
    }
};
