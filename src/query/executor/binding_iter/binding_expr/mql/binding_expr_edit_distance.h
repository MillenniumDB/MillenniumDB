#pragma once

#include <memory>

#include "graph_models/quad_model/conversions.h"
#include "misc/edit_distance.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {

// based on: https://en.wikipedia.org/wiki/Levenshtein_distance
// NOTE: if we need to support transpositions, we could use the following algorithm:
// https://en.wikipedia.org/wiki/Damerau%E2%80%93Levenshtein_distance
class BindingExprEditDistance : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> lhs;
    std::unique_ptr<BindingExpr> rhs;

    BindingExprEditDistance(std::unique_ptr<BindingExpr> lhs_, std::unique_ptr<BindingExpr> rhs_) :
        lhs(std::move(lhs_)),
        rhs(std::move(rhs_))
    { }

    ObjectId eval(const Binding& binding) override
    {
        const auto lhs_oid = lhs->eval(binding);

        if ((lhs_oid.id & ObjectId::GENERIC_TYPE_MASK) != ObjectId::MASK_STRING_SIMPLE) {
            return ObjectId::get_null();
        }

        const auto rhs_oid = rhs->eval(binding);
        if ((rhs_oid.id & ObjectId::GENERIC_TYPE_MASK) != ObjectId::MASK_STRING_SIMPLE) {
            return ObjectId::get_null();
        }

        const std::string lhs_str = Conversions::unpack_string(lhs_oid);
        const std::string rhs_str = Conversions::unpack_string(rhs_oid);

        const uint64_t res = edit_distance(lhs_str, rhs_str);

        return Conversions::pack_int(res);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "EDIT_DISTANCE(";
        lhs->print(os, ops);
        os << ", ";
        rhs->print(os, ops);
        os << ')';
    }
};
} // namespace MQL