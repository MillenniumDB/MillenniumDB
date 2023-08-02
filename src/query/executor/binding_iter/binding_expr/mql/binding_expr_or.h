#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {

class BindingExprOr : public BindingExpr {
public:
    std::vector<std::unique_ptr<BindingExpr>> or_list;

    BindingExprOr(std::vector<std::unique_ptr<BindingExpr>>&& or_list) :
        or_list (std::move(or_list)) { }


    ObjectId eval(const Binding& binding) override {
        for (auto& expr : or_list) {
            auto oid = expr->eval(binding);

            if (oid == ObjectId(ObjectId::BOOL_FALSE)) {
                continue;
            } else if (oid == ObjectId(ObjectId::BOOL_TRUE)) {
                return oid;
            } else {
                return ObjectId::get_null();
            }

        }
        return ObjectId(ObjectId::BOOL_FALSE);
    }

    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << *or_list[0];
        for (size_t i = 1; i < or_list.size(); i++) {
            auto& expr = or_list[0];
            os << " OR " << *expr;
        }
        return os;
    }
};
} // namespace MQL
