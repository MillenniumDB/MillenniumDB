#pragma once

#include "query/parser/op/mql/op.h"

namespace MQL {

class OpUnitTable : public Op {
public:
    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpUnitTable>();
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return {};
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ') << "OpUnitTable()\n";
        return os;
    }
};
} // namespace MQL
