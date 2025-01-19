#pragma once

#include "query/parser/op/op.h"

namespace GQL {

struct Repetition {
    Repetition(uint64_t lower, std::optional<uint64_t> upper = std::nullopt) :
        lower(lower),
        upper(upper)
    { }
    uint64_t lower;
    std::optional<uint64_t> upper;
};

class OpRepetition : public Op {
public:
    std::unique_ptr<Op> op;
    uint64_t lower;
    std::optional<uint64_t> upper;

    OpRepetition(std::unique_ptr<Op>&& op, uint64_t lower, std::optional<uint64_t> upper = std::nullopt) :
        op(std::move(op)),
        lower(lower),
        upper(upper)
    { }

    OpRepetition(std::unique_ptr<Op>&& op, std::unique_ptr<Repetition>&& repetition) :
        op(std::move(op)),
        lower(repetition->lower),
        upper(repetition->upper)
    { }

    std::unique_ptr<Op> clone() const override
    {
        return std::make_unique<OpRepetition>(op->clone(), lower, upper);
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return op->get_all_vars();
    }

    std::set<VarId> get_scope_vars() const override
    {
        return get_all_vars();
    }

    std::set<VarId> get_safe_vars() const override
    {
        return get_all_vars();
    }

    std::set<VarId> get_fixable_vars() const override
    {
        return get_all_vars();
    }

    std::map<VarId, std::unique_ptr<GQL::VarType>> get_var_types() const override
    {
        std::map<VarId, std::unique_ptr<GQL::VarType>> res = op->get_var_types();

        for (auto& [var, type] : res) {
            if (!type->is_group()) {
                res[var] = std::make_unique<Group>(std::move(type));
            }
        }

        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpRepetition(" << lower << ", ";
        if (upper.has_value()) {
            os << upper.value();
        }
        os << ")\n";
        op->print_to_ostream(os, indent + 2);

        return os;
    }
};

} // namespace GQL
