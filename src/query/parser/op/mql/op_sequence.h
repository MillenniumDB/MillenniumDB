#pragma once

#include "query/parser/op/mql/op.h"

namespace MQL {

class OpSequence : public Op {
public:
    std::vector<std::unique_ptr<Op>> sequence;

    OpSequence(std::vector<std::unique_ptr<Op>>&& sequence) :
        sequence(std::move(sequence))
    { }

    std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Op>> sequence_clone;
        sequence_clone.reserve(sequence.size());
        for (const auto& op : sequence) {
            sequence_clone.push_back(op->clone());
        }
        return std::make_unique<OpSequence>(std::move(sequence_clone));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        auto res = sequence[0]->get_all_vars();
        for (std::size_t i = 1; i < sequence.size(); ++i) {
            res.merge(sequence[i]->get_all_vars());
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpSequence()\n";
        for (const auto& op_procedure : sequence) {
            op_procedure->print_to_ostream(os, indent + 2);
        }
        return os;
    }
};
} // namespace MQL
