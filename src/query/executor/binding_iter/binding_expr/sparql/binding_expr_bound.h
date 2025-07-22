#pragma once

#include "graph_models/rdf_model/conversions.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"
#include "query/query_context.h"

namespace SPARQL {
class BindingExprBound : public BindingExpr {
public:
    VarId var_id;

    BindingExprBound(VarId var_id) :
        var_id(var_id)
    { }

    ObjectId eval(const Binding& binding) override
    {
        ObjectId oid = binding[var_id];
        if (oid.is_null())
            return Conversions::pack_bool(false);
        else
            return Conversions::pack_bool(true);
    }

    void accept_visitor(BindingExprVisitor& visitor) override
    {
        visitor.visit(*this);
    }

    void print(std::ostream& os, std::vector<BindingIter*>) const override
    {
        os << "BOUND(" << var_id << ')';
    }
};
} // namespace SPARQL
