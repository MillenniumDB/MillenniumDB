#pragma once

#include "graph_models/gql/gql_model.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
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
        return binding[property_var_id];
    }
};
} // namespace GQL
