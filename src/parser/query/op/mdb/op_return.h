#pragma once

#include <memory>
#include <vector>

#include "parser/query/op/op.h"
#include "parser/query/return_item/return_item.h"

namespace MDB {

class OpReturn : public Op {
public:
    static constexpr uint64_t DEFAULT_LIMIT = UINT64_MAX;

    const std::unique_ptr<Op> op;

    const std::vector<std::unique_ptr<ReturnItem>> return_items;

    const bool distinct;

    const uint64_t limit;

    OpReturn(std::unique_ptr<Op>                        op,
             std::vector<std::unique_ptr<ReturnItem>>&& return_items,
             bool                                       distinct,
             uint64_t                                   limit) :
        op(std::move(op)), return_items(std::move(return_items)), distinct(distinct), limit(limit) { }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<Var> get_vars() const override {
        auto res = op->get_vars();
        for (auto& return_item : return_items) {
            for (auto& var : return_item->get_vars()) {
                res.insert(var);
            }
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpReturn(";
        if (distinct) {
            os << "DISTINCT ";
        }
        bool first = true;
        for (const auto& return_item : return_items) {
            if (!first)
                os << ", ";
            first = false;
            os << *return_item;
        }

        if (limit != DEFAULT_LIMIT) {
            os << "; LIMIT " << limit;
        }
        os << ")";
        os << "\n";
        return op->print_to_ostream(os, indent);
    }
};
} // namespace MDB