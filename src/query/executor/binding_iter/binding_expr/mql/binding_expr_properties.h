#pragma once

#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprProperties : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprProperties(std::unique_ptr<BindingExpr> expr_) :
        expr(std::move(expr_))
    { }

    ObjectId eval(const Binding& binding) override
    {
        const ObjectId oid = expr->eval(binding);

        const auto gen_t = oid.id & ObjectId::GENERIC_TYPE_MASK;
        if ((gen_t != ObjectId::MASK_NAMED_NODE) && (gen_t != ObjectId::MASK_EDGE)) {
            return ObjectId::get_null();
        }

        bool interruption = false;
        BptIter<3> it = quad_model.object_key_value
                            ->get_range(&interruption, { oid.id, 0, 0 }, { oid.id, UINT64_MAX, UINT64_MAX });

        auto record = it.next();
        std::map<ObjectId, std::unique_ptr<DictionaryItem>> properties_map;

        while (record != nullptr) {
            ObjectId key_oid((*record)[1]);
            ObjectId value_oid((*record)[2]);

            properties_map.emplace(key_oid, std::make_unique<DictionaryLiteral>(value_oid));
            record = it.next();
        }

        const auto dict = std::make_unique<Dictionary>(std::move(properties_map));
        const ObjectId dict_oid = Conversions::pack_dictionary(dict);
        return dict_oid;
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>& ops) const override
    {
        os << "PROPERTIES(";
        expr->print(os, ops);
        os << ')';
    }
};
} // namespace MQL
