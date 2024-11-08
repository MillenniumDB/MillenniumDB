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
};
