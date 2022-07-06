#pragma once

#include <set>

#include "base/query/var.h"
#include "parser/query/op_visitor/op_visitor.h"

class Op {
public:
    virtual ~Op() = default;

    virtual void accept_visitor(OpVisitor&) = 0;

    virtual std::set<Var> get_vars() const = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Op& b) {
        return b.print_to_ostream(os);
    }
};
