#pragma once

#include <functional>
#include <set>
#include <string>
#include <vector>

#include "base/ids/var_id.h"
#include "base/query/var.h"
#include "execution/binding_iter/aggregation/agg.h"
#include "parser/query/return_item/return_item_visitor.h"
#include "query_optimizer/quad_model/binding_iter_visitor.h"

/*
used to visit elements of RETURN and ORDER BY, populating:
- binding_iter_visitor.group_saved_vars
- binding_iter_visitor.aggs
- binding_iter_visitor.var_properties
*/
class ReturnItemVisitorImpl : public ReturnItemVisitor {
public:
    ReturnItemVisitorImpl(BindingIterVisitor& binding_iter_visitor) :
        binding_iter_visitor (binding_iter_visitor) { }

    void visit(ReturnItemAgg&)   override;
    void visit(ReturnItemCount&) override;
    void visit(ReturnItemVar&)   override;

private:
    BindingIterVisitor& binding_iter_visitor;
};
