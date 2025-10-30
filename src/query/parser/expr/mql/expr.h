#pragma once

#include <memory>
#include <optional>
#include <ostream>
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

    // returns vars needed to evaluate this expression
    virtual std::set<VarId> get_input_vars() const = 0;

    virtual bool has_aggregation() const = 0;

    virtual void print(std::ostream&) const = 0;

    // Overriden by ExprVar and ExprVarProperty
    virtual std::optional<VarId> get_var() const
    {
        return {};
    }
};

inline std::ostream& operator<<(std::ostream& os, const Expr& e)
{
    e.print(os);
    return os;
}

} // namespace MQL
