#pragma once

#include <memory>
#include <ostream>
#include <set>

#include "query/parser/op/gql/op_visitor.h"
#include "query/query_context.h" // IWYU pragma: export
#include "query/rewriter/gql/op/var_type.h"

namespace GQL {

class Op {
public:
    static constexpr uint64_t DEFAULT_LIMIT = UINT64_MAX;
    static constexpr uint64_t DEFAULT_OFFSET = 0;

    virtual ~Op() = default;

    virtual std::unique_ptr<Op> clone() const = 0;

    virtual void accept_visitor(OpVisitor&) = 0;

    // contains all the variables mentioned in the operator and its sub operators
    virtual std::set<VarId> get_all_vars() const = 0;

    virtual std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const = 0;

    virtual bool read_only() const
    {
        return true;
    }

    virtual std::map<VarId, GQL::VarType> get_var_types() const
    {
        return {};
    }

    friend std::ostream& operator<<(std::ostream& os, const Op& op)
    {
        return op.print_to_ostream(os);
    }
};
} // namespace GQL
