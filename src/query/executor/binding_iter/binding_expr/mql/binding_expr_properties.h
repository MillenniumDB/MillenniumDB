#pragma once

#include "graph_models/quad_model/conversions.h"
#include "graph_models/quad_model/quad_model.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace MQL {
class BindingExprProperties : public BindingExpr {
public:
    VarId var;

    BindingExprProperties(VarId var) :
        var(var)
    { }

    ObjectId eval(const Binding& binding) override
    {
        ObjectId object_id = binding[var];

        bool interruption = false;
        BptIter<3> it = quad_model.object_key_value->get_range(
            &interruption,
            { object_id.id, 0, 0 },
            { object_id.id, UINT64_MAX, UINT64_MAX }
        );

        auto record = it.next();
        std::map<ObjectId, std::unique_ptr<DictionaryItem>> properties_map;

        while (record != nullptr) {
            ObjectId key_oid((*record)[1]);
            ObjectId value_oid((*record)[2]);

            properties_map.emplace(key_oid, std::make_unique<DictionaryLiteral>(value_oid));
            record = it.next();
        }

        auto dict = std::make_unique<Dictionary>(std::move(properties_map));
        ObjectId dict_oid = Conversions::pack_dictionary(dict);
        return dict_oid;
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>&) const override
    {
        os << "PROPERTIES(" << var << ')';
    }
};
} // namespace MQL
