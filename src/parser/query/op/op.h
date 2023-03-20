#pragma once

#include <set>

#include "base/query/var.h"
#include "parser/query/op/op_visitor.h"

class Op {
public:
    virtual ~Op() = default;

    virtual void accept_visitor(OpVisitor&) = 0;

    virtual std::set<Var> get_vars() const = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const = 0;

    virtual bool read_only() const { return true; }

    friend std::ostream& operator<<(std::ostream& os, const Op& op) {
        return op.print_to_ostream(os);
    }
};
