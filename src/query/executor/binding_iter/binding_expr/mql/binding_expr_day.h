#pragma once

#include "graph_models/common/datatypes/datetime.h"
#include "graph_models/quad_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprDay : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprDay(std::unique_ptr<BindingExpr> expr_) :
        expr(std::move(expr_))
    { }

    ObjectId eval(const Binding& binding) override
    {
        auto expr_oid = expr->eval(binding);
        const auto gen_sub_t = expr_oid.id & ObjectId::SUB_TYPE_MASK;

        switch (gen_sub_t) {
        case ObjectId::MASK_DT_DATE:
        case ObjectId::MASK_DT_TIME:
        case ObjectId::MASK_DT_DATETIME:
        case ObjectId::MASK_DT_DATETIMESTAMP: {
            bool error;
            auto res = DateTime(expr_oid).get_day(&error);
            if (error) {
                return ObjectId::get_null();
            }
            return Conversions::pack_int(res);
        }
        case ObjectId::MASK_ANON:
        case ObjectId::MASK_NAMED_NODE:
        case ObjectId::MASK_STRING_SIMPLE:
        case ObjectId::MASK_INT:
        case ObjectId::MASK_DECIMAL:
        case ObjectId::MASK_FLOAT:
        case ObjectId::MASK_BOOL:
        case ObjectId::MASK_EDGE:
        case ObjectId::MASK_TENSOR_FLOAT:
        case ObjectId::MASK_TENSOR_DOUBLE:
        case ObjectId::MASK_PATH:
        case ObjectId::MASK_NULL:
            return ObjectId::get_null();
        default:
            assert(false);
            return ObjectId::get_null();
        }
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "DAY(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace MQL
