#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/parser/expr/mql/atom_expr/expr_var_property.h"
#include "query/parser/op/mql/ops.h"

class BindingIter;
namespace MQL {

class BindingIterConstructor : public OpVisitor {
public:
    BindingIterConstructor();

    std::vector<VarId> projected_vars;

    // The expressions present in OpReturn and OpOrderBy
    // The the expressions of Return are before expression from OrderBy
    // Because the evaluation is from left to right
    std::vector<std::pair<VarId, std::unique_ptr<BindingExpr>>> projection_order_exprs;

    // same elements of group_vars_vector but in a set to easier search
    std::set<VarId> group_vars;

    // variables saved in the OrderBy that makes the groups
    // contains all group_vars and other used variables
    std::set<VarId> group_saved_vars;

    std::map<VarId, std::unique_ptr<Agg>> aggregations;

    std::vector<VarId> order_by_vars;
    std::set<VarId> order_by_saved_vars;

    // second component is a state to tell if it is assigned
    std::map<ExprVarProperty, bool> used_properties;

    // properties in query with one type fixed (not null).
    // example: (?x {value IS INTEGER}). value only has one type in query fixed, and is not null.
    // var, key, bitmap_final_types
    std::vector<PropertyTypeConstraint> fixed_types_properties;

    // Properties info in type queries
    // (?x {value IS INTEGER})
    // (?x) WHERE ?x.value IS INTEGER
    // var_without_propertyId, keyId, var_with_propertyId, type_bitmap
    std::vector<PropertyTypeConstraint> properties_types_queries;

    // Properties info in queries with operators (==, !=, >, <, >=, <=)
    // (?x {value == 4})
    // var_without_propertyId, keyId
    std::vector<PropertyOperatorConstraint> properties_operators;

    // For path_manager to print in the correct direction
    std::vector<bool> begin_at_left;

    // The subset of possible assigned vars that always have a non-null value while visiting
    // the current operator and should be fixed in its leaf operators such as IndexScans
    std::set<VarId> safe_assigned_vars;

    // After visiting an Op, the result must be written into tmp
    std::unique_ptr<BindingIter> tmp;

    OpGroupBy* op_group_by = nullptr;
    OpOrderBy* op_order_by = nullptr;

    // We add the fixed properties to the first BGP visited, this attribute avoid adding it multiple times
    // in different BGPs
    bool pushed_fixed_properties = false;

    // True when a group is needed (Aggregation or Group By are present)
    bool grouping = false;

    void visit(OpBasicGraphPattern&) override;
    void visit(OpCall&) override;
    void visit(OpLet&) override;
    void visit(OpOptional&) override;
    void visit(OpWhere&) override;
    void visit(OpGroupBy&) override;
    void visit(OpOrderBy&) override;
    void visit(OpReturn&) override;
    void visit(OpSequence&) override;

    void visit(OpDescribe&) override
    {
        throw LogicException("OpDescribe must be processed outside");
    }

    void visit(OpShow&) override
    {
        throw LogicException("OpShow must be processed outside");
    }

    /* These are processed inside OpBasicGraphPattern */
    void visit(OpEdge&) override { }
    void visit(OpDisjointTerm&) override { }
    void visit(OpDisjointVar&) override { }
    void visit(OpLabel&) override { }
    void visit(OpPath&) override { }
    void visit(OpProperty&) override { }

    /* There are impossible to have in a read only query*/
    void visit(OpInsert&) override { }
    void visit(OpUpdate&) override { }
    void visit(OpCreateHNSWIndex&) override { }
    void visit(OpCreateTextIndex&) override { }

private:
    bool term_exists(ObjectId) const;

    std::unique_ptr<BindingIter> get_pending_properties(std::unique_ptr<BindingIter> binding_iter);
};
} // namespace MQL
