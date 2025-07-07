#pragma once

#include "query/parser/expr/gql/expr.h"
#include "query/parser/expr/gql/expr_printer.h"
#include "query/parser/op/gql/op.h"

namespace GQL {

struct LetItem {
    VarId var_id;
    std::unique_ptr<Expr> expr;

    LetItem(VarId var_id, std::unique_ptr<Expr> expr) :
        var_id(var_id),
        expr(std::move(expr))
    { }
};

class OpLet : public Op {
public:
    std::vector<LetItem> items;

    OpLet(std::vector<LetItem> items) :
        items(std::move(items))
    { }

    virtual std::unique_ptr<Op> clone() const override
    {
        std::vector<LetItem> items_clone;
        for (auto& item : items) {
            items_clone.emplace_back(item.var_id, item.expr->clone());
        }
        return std::make_unique<OpLet>(std::move(items_clone));
    }

    void accept_visitor(OpVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    std::set<VarId> get_all_vars() const override
    {
        std::set<VarId> result;
        for (auto& item : items) {
            result.insert(item.var_id);
        }
        return result;
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent = 0) const override
    {
        os << std::string(indent, ' ');
        os << "OpLet(";

        bool first = true;

        ExprPrinter expr_printer(os);

        for (auto& item : items) {
            if (first) {
                first = false;
            } else {
                os << ", ";
            }
            os << item.var_id << "=";
            item.expr->accept_visitor(expr_printer);
        }

        os << ")\n";
        return os;
    }
};
} // namespace GQL
