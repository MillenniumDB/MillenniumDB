#pragma once

#include <memory>
#include <vector>

#include "query/parser/expr/expr.h"
#include "query/parser/op/op.h"

namespace MQL {

class OpReturn : public Op {
public:
    std::unique_ptr<Op> op;

    // Expr is nullptr when a single var is being projected
    std::vector<std::pair<VarId, std::unique_ptr<Expr>>> projection;

    const bool distinct;

    const uint64_t limit;

    const uint64_t offset;

    OpReturn(
        std::unique_ptr<Op>                                   op,
        std::vector<std::pair<VarId,std::unique_ptr<Expr>>>&& projection,
        bool                                                  distinct,
        uint64_t                                              limit,
        uint64_t                                              offset
    ) :
        op         (std::move(op)),
        projection (std::move(projection)),
        distinct   (distinct),
        limit      (limit),
        offset     (offset) { }

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
            limit,
            offset
        );
    }

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override {
        auto res = op->get_all_vars();
        return res;
    }

    std::set<VarId> get_scope_vars() const override {
        std::set<VarId> res;
        for (auto&& [var, _] : projection) {
            res.insert(var);
        }
        return res;
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
        bool first = true;
        for (auto&& [var, expr] : projection) {
            if (!first) {
                os << ", ";
            }
            first = false;

            if (expr != nullptr) {
                ExprPrinter printer(os);
                expr->accept_visitor(printer);
            } else {
                os << var;
            }
        }

        if (limit != DEFAULT_LIMIT) {
            os << "; LIMIT " << limit;
        }
        if (offset != DEFAULT_OFFSET) {
            os << "; OFFSET " << offset;
        }
        os << ")";
        os << "\n";
        return op->print_to_ostream(os, indent);
    }
};
} // namespace MQL
