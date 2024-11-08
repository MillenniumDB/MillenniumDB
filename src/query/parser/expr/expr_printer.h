#pragma once

#include <vector>

#include "query/parser/expr/expr_visitor.h"

class Op;

class AbstractExprPrinter : public ExprVisitor {
public:
    std::ostream& os;
    std::vector<Op*> ops;

    AbstractExprPrinter(std::ostream& os) : os(os){ }
    virtual ~AbstractExprPrinter() = default;
};
