#pragma once

#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/query_context.h"
#include "storage/index/bplus_tree/bplus_tree.h"

namespace GQL {

class BindingExprEdgeProperty : public BindingExpr {
public:
    VarId var_id;
    ObjectId property;
    VarId property_var_id;

    BindingExprEdgeProperty(VarId var_id, ObjectId property, VarId property_var_id) :
        var_id(var_id),
        property(property),
        property_var_id(property_var_id)
    { }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    ObjectId eval(const Binding& binding) override
    {
        bool interruption = false;

        ObjectId var_oid = binding[var_id];

        BptIter<3> it = gql_model.edge_key_value->get_range(
            &interruption,
            { var_oid.id, property.id, 0 },
            { var_oid.id, property.id, UINT64_MAX }
        );

        auto record = it.next();
        if (record != nullptr) {
            ObjectId value((*record)[2]);
            return value;
        }
        return ObjectId::get_null();
    }

    void print(std::ostream& os, std::vector<BindingIter*>) const override
    {
        os << '(';
        os << property_var_id;
        os << ')';
    }
};
} // namespace GQL
