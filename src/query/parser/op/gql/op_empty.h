#pragma once

#include "query/parser/op/gql/op.h"

namespace GQL {

class OpEmpty : public Op {
public:
    OpEmpty() { }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpEmpty>();
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return {};
    }

    std::map<VarId, GQL::VarType> get_var_types() const override
    {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ') << "OpEmpty()\n";
        return os;
    }
};
} // namespace GQL
