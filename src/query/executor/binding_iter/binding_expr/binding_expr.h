#pragma once

#include <ostream>
#include <vector>

#include "graph_models/object_id.h"
#include "query/executor/binding.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_visitor.h"

class BindingIter;

// Abstract class
class BindingExpr {
public:
    virtual ~BindingExpr() = default;

    virtual void accept_visitor(BindingExprVisitor&) = 0;

    virtual ObjectId eval(const Binding&) = 0;

    virtual void print(std::ostream& os, std::vector<BindingIter*> ops) const = 0;
};
