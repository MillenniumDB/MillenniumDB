#pragma once

#include <memory>
#include <string>

#include "base/ids/var_id.h"
#include "execution/binding_iter/binding_expr/binding_expr.h"
#include "query_optimizer/quad_model/query_element_to_graph_object.h"

class BindingExprAtomConstant : public BindingExpr {
public:
    QueryElement query_element;

    GraphObject value;

    BindingExprAtomConstant(QueryElement _query_element) :
        query_element (std::move(_query_element))
    {
        QueryElementToGraphObject visitor;
        value = visitor(query_element);
    }

    GraphObject eval(const BindingIter&) const override {
        return value;
    }
};
