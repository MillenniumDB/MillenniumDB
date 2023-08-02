#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/expr.h"
#include "query/parser/op/op.h"

namespace MQL {

class OpReturn : public Op {
public:
    static constexpr uint64_t DEFAULT_LIMIT = UINT64_MAX;

    const std::unique_ptr<Op> op;

    // Expr is nullptr when a single var is being projected
    std::vector<std::pair<VarId, std::unique_ptr<Expr>>> projection;

    const bool distinct;

    const uint64_t limit;

    OpReturn(
        std::unique_ptr<Op>                                   op,
        std::vector<std::pair<VarId,std::unique_ptr<Expr>>>&& projection,
        bool                                                  distinct,
        uint64_t                                              limit
    ) :
        op         (std::move(op)),
        projection (std::move(projection)),
        distinct   (distinct),
        limit      (limit) { }

    virtual std::unique_ptr<Op> clone() const override {
        std::vector<std::pair<VarId,std::unique_ptr<Expr>>> projection_clone;
        projection_clone.reserve(projection.size());
        for (auto&& [var, expr] : projection_clone) {
            if (expr != nullptr) {
                projection_clone.push_back({var, expr->clone()});
            } else {
                projection_clone.push_back({var, nullptr});
            }
        }
        return std::make_unique<OpReturn>(
            op->clone(),
            std::move(projection_clone),
            distinct,
            limit
        );
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        auto res = op->get_all_vars();
        // for (auto& return_item : return_items) {
        //     for (auto& var : return_item->get_vars()) {
        //         res.insert(var);
        //     }
        // }
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        // TODO: mdb
        // std::set<VarId> res;
        // for (auto& return_item : return_items) {
        //     for (auto& var : return_item->get_vars()) {
        //         res.insert(var);
        //     }
        // }
        // return res;
        return { };
    }

    std::set<VarId> get_safe_vars() const override {
        return { };
    }

    std::set<VarId> get_fixable_vars() const override {
        return { };
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override {
        os << std::string(indent, ' ');
        os << "OpReturn(";
        if (distinct) {
            os << "DISTINCT ";
        }
        // bool first = true;
        // for (const auto& return_item : return_items) {
        //     if (!first)
        //         os << ", ";
        //     first = false;
        //     os << *return_item;
        // }

        if (limit != DEFAULT_LIMIT) {
            os << "; LIMIT " << limit;
        }
        os << ")";
        os << "\n";
        return op->print_to_ostream(os, indent);
    }
};
} // namespace MQL
