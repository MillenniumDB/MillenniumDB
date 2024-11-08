#pragma once

#include <set>
#include <memory>
#include <ostream>

#include "query/parser/expr/mql_expr_printer.h"
#include "query/parser/expr/sparql_expr_printer.h"
#include "query/parser/op/op_visitor.h"
#include "query/query_context.h"

class Op {
public:
    static constexpr uint64_t DEFAULT_LIMIT  = UINT64_MAX;
    static constexpr uint64_t DEFAULT_OFFSET = 0;

    virtual ~Op() = default;

    virtual std::unique_ptr<Op> clone() const = 0;

    virtual void accept_visitor(OpVisitor&) = 0;

    // contains all the variables mentioned in the operator and its sub operators
    virtual std::set<VarId> get_all_vars() const = 0;

    // subset of all_vars that are in the scope after evaluating the operator
    virtual std::set<VarId> get_scope_vars() const = 0;

    // subset of scope variables that will always have a non-null value
    virtual std::set<VarId> get_safe_vars() const = 0;

    // subset of safe variables that can be fixed from the outside when evaluating the operator
    virtual std::set<VarId> get_fixable_vars() const = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const = 0;

    virtual bool read_only() const { return true; }

    friend std::ostream& operator<<(std::ostream& os, const Op& op) {
        return op.print_to_ostream(os);
    }
};
