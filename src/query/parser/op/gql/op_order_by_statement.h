#pragma once

#include <cstddef>
#include <vector>

#include "query/parser/expr/gql/expr.h"
#include "query/parser/expr/gql/expr_printer.h"
#include "query/parser/expr/gql/expr_var.h"
#include "query/parser/op/gql/op.h"

namespace GQL {

class OpOrderByStatement : public Op {
public:
    std::vector<std::unique_ptr<Expr>> items;

    std::vector<bool> ascending_order;
    std::vector<bool> null_order;

    uint64_t offset;
    uint64_t limit;

    OpOrderByStatement(
        std::vector<std::unique_ptr<Expr>>&& _items,
        std::vector<bool>&& ascending_order,
        std::vector<bool>&& null_order,
        uint64_t offset,
        uint64_t limit
    ) :
        ascending_order(std::move(ascending_order)),
        null_order(std::move(null_order)),
        offset(offset),
        limit(limit)
    {
        for (auto& item : _items) {
            auto casted_expr_var = dynamic_cast<ExprVar*>(item.get());
            if (casted_expr_var) {
                items.push_back(std::make_unique<ExprVar>(casted_expr_var->id));
                continue;
            }
            items.push_back(std::move(item));
        }
    }

    std::unique_ptr<Op> clone() const override
    {
        std::vector<std::unique_ptr<Expr>> new_items;

        new_items.reserve(items.size());
        for (const auto& item : items) {
            new_items.push_back(item->clone());
        }

        std::vector<bool> new_ascending_order = ascending_order;
        std::vector<bool> new_null_order = null_order;

        return std::make_unique<OpOrderByStatement>(
            std::move(new_items),
            std::move(new_ascending_order),
            std::move(new_null_order),
            offset,
            limit
        );
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> res;
        for (auto& item : items) {
            for (auto& v : item->get_all_vars()) {
                res.insert(v);
            }
        }
        return res;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ') << "OpOrderByStatement(";

        ExprPrinter printer(os);

        for (unsigned i = 0; i < items.size(); i++) {
            if (i != 0)
                os << ", ";

            os << (ascending_order[i] ? "ASC " : "DESC ");
            os << (null_order[i] ? "NULLS FIRST " : "NULLS LAST ");

            auto& item = items[i];

            auto expr = item.get();
            expr->accept_visitor(printer);
        }
        os << ", " << "offset:" << offset << ", limit:" << limit << ")\n";

        for (size_t i = 0; i < printer.ops.size(); i++) {
            os << std::string(indent + 2, ' ') << "_Op_" << i << "_:\n";
            printer.ops[i]->print_to_ostream(os, indent + 4);
        }

        return os;
    }
};
} // namespace GQL
