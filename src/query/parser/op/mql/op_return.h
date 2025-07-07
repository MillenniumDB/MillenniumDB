#pragma once

#include <optional>
#include <vector>

#include "query/parser/expr/mql/expr.h"
#include "query/parser/expr/mql/expr_printer.h"
#include "query/parser/op/mql/op.h"

namespace MQL {

class OpReturn : public Op {
public:
    std::unique_ptr<Op> op;

    std::vector<std::pair<std::unique_ptr<Expr>, VarId>> projection;

    const bool distinct;

    const uint64_t limit;

    const uint64_t offset;

    OpReturn(
        std::unique_ptr<Op> op,
        std::vector<std::pair<std::unique_ptr<Expr>, VarId>>&& projection,
        bool distinct,
        uint64_t limit,
        uint64_t offset
    ) :
        op(std::move(op)),
        projection(std::move(projection)),
        distinct(distinct),
        limit(limit),
        offset(offset)
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<std::pair<std::unique_ptr<Expr>, VarId>> projection_clone;
        for (auto&& [expr, var] : projection_clone) {
            projection_clone.push_back({ expr->clone(), var });
        }
        return std::make_unique<OpReturn>(op->clone(), std::move(projection_clone), distinct, limit, offset);
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        auto res = op->get_all_vars();
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpReturn(";
        if (distinct) {
            os << "DISTINCT ";
        }
        bool first = true;
        for (auto&& [expr, var] : projection) {
            if (!first) {
                os << ", ";
            }
            first = false;

            ExprPrinter printer(os);
            expr->accept_visitor(printer);
            os << " AS " << var; // TODO: ?x AS ?x is weird
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
