#pragma once

#include <vector>

#include "query/executor/binding_iter/binding_expr/binding_expr_term.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_var.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_visitor.h"
#include "query/query_context.h"

class BindingIter;

class BindingExprPrinter : public BindingExprVisitor {
public:
    std::ostream& os;

    // Expressions like EXISTS and NOT EXISTS may add elements to ops
    std::vector<BindingIter*> ops;

    BindingExprPrinter(std::ostream& os) :
        os(os)
    { }

    virtual ~BindingExprPrinter() = default;

    void visit(BindingExprTerm& expr) override
    {
        os << expr.object_id;
    }

    void visit(BindingExprVar& expr) override
    {
        os << expr.var;
    }
};
