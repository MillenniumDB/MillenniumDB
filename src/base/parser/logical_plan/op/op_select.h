#ifndef BASE__OP_SELECT_H_
#define BASE__OP_SELECT_H_

#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <memory>

#include "base/parser/grammar/query/query_ast.h"
#include "base/parser/logical_plan/op/op.h"


class OpSelect : public Op {
public:
    const uint64_t limit;
    const std::unique_ptr<Op> op;

    // pair <var_name, key_name>
    // or   <var_name, ""> (empty string when selecting just the variable)
    // empty vector means SELECT *
    const std::vector<query::ast::SelectItem> select_items;

    OpSelect(std::unique_ptr<Op> op, std::vector<query::ast::SelectItem> select_items, uint64_t limit) :
        limit        (limit),
        op           (std::move(op)),
        select_items (std::move(select_items)) { }

    ~OpSelect() = default;

    void accept_visitor(OpVisitor& visitor) override {
        visitor.visit(*this);
    }

    std::ostream& print_to_ostream(std::ostream& os, int indent=0) const override{
        os << std::string(indent, ' ');
        os << "OpSelect(";
        if (select_items.size() == 0) {
            os << "*";
        } else {
            bool first = true;
            for (auto & select_item : select_items) {
                if (!first)
                    os << ", ";
                first = false;
                if (select_item.key) {
                    os << select_item.var << "." << select_item.key.get();
                } else {
                    os << select_item.var;
                }
            }
        }

        if (limit) {
            os << "; LIMIT " << limit;
        }
        os << ")";
        os << "\n";
        return op->print_to_ostream(os, indent + 2);
    };

    std::set<Var> get_vars() const override {
        auto res = op->get_vars();
        for (const auto& select_item : select_items) {
            if (select_item.key) {
                res.emplace(select_item.var + '.' + select_item.key.get());
            }
        }
        return res;
    }
};

#endif // BASE__OP_SELECT_H_
