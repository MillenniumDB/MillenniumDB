#pragma once

#include <memory>
#include <vector>

#include "query/parser/op/mql/op.h"
#include "query/update/mql/update_action.h"

namespace MQL {
class OpUpdate : public Op {
public:
    std::unique_ptr<Op> op;
    std::unique_ptr<UpdateContext> update_ctx;
    std::vector<std::unique_ptr<UpdateAction>> update_actions;

    OpUpdate(
        std::unique_ptr<Op> op,
        std::unique_ptr<UpdateContext> update_ctx,
        std::vector<std::unique_ptr<UpdateAction>> update_actions
    ) :
        op(std::move(op)),
        update_ctx(std::move(update_ctx)),
        update_actions(std::move(update_actions))
    { }

    std::unique_ptr<Op> clone() const override
    {
        throw LogicException("must not clone OpUpdate");
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        return op->get_all_vars();
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ') << "OpUpdate()\n";

        for (auto& update : update_actions) {
            update->print(os, indent + 2);
        }

        return os;
    }
};
} // namespace MQL
