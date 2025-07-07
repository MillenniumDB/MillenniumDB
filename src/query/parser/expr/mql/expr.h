#pragma once

#include <memory>
#include <set>

#include "query/parser/expr/mql/expr_visitor.h"
#include "query/var_id.h"

namespace MQL {
class Expr {
public:
    virtual ~Expr() = default;

    virtual std::unique_ptr<Expr> clone() const = 0;

    virtual void accept_visitor(ExprVisitor&) = 0;

    virtual std::set<VarId> get_all_vars() const = 0;

    virtual bool has_aggregation() const = 0;
};
} // namespace MQL
