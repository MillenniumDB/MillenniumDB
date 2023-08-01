#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "query/var_id.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprBound : public BindingExpr {
public:
    VarId var_id;

    BindingExprBound(VarId var_id) :
        var_id (var_id) { }

    ObjectId eval(const Binding& binding) override {
        ObjectId oid = binding[var_id];
        if (oid.is_null())
            return ObjectId(ObjectId::BOOL_FALSE);
        else
            return ObjectId(ObjectId::BOOL_TRUE);
    }


    std::ostream& print_to_ostream(std::ostream& os) const override {
        os << "BOUND(VarId(" << var_id.id << "))";
        return os;
    }
};
} // namespace SPARQL
