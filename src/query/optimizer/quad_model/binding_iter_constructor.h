#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "query/executor/binding_iter/aggregation/agg.h"
#include "query/query_context.h"
#include "query/parser/op/op.h"

class BindingIter;
namespace MQL {

class BindingIterConstructor : public OpVisitor {
public:
    BindingIterConstructor(std::map<VarId, ObjectId>& setted_vars);

    std::vector<VarId> projected_vars;

    std::vector<VarId> group_vars_vector;

    // same elements of group_vars_vector but in a set to easier search
    std::set<VarId> group_vars;

    // variables saved in the OrderBy that makes the groups
    // contains all group_vars and other used variables
    std::set<VarId> group_saved_vars;

    std::map<VarId, std::unique_ptr<Agg>> aggregations;

    std::vector<VarId> order_by_vars;
    std::vector<bool> order_by_ascending;
    std::set<VarId> order_by_saved_vars;

    // Variables that are assigned when evaluating the basic graph pattern, and the optimizer know the value.
    // May come from properties or the SET/WHERE clauses
    std::map<VarId, ObjectId>& setted_vars;

    // Properties from a where clause that are mandatory (so they are pushed into the basic graph pattern)
    // var, key, value, value_var
    std::vector<std::tuple<VarId, ObjectId, ObjectId, VarId>> fixed_properties;

    // For path_manager to print in the correct direction
    std::vector<bool> begin_at_left;

    // The subset of possible assigned vars that always have a non-null value while visiting
    // the current operator and should be fixed in its leaf operators such as IndexScans
    std::set<VarId> safe_assigned_vars;

    // After visiting an Op, the result must be written into tmp
    std::unique_ptr<BindingIter> tmp;

    // We add the fixed properties to the first BGP visited, this attribute avoid adding it multiple times
    // in different BGPs
    bool pushed_fixed_properties = false;

    // True when a group is needed (Aggregation or Group By are present)
    bool grouping = false;

    void visit(MQL::OpMatch&)             override;
    void visit(MQL::OpBasicGraphPattern&) override;
    void visit(MQL::OpOptional&)          override;
    void visit(MQL::OpWhere&)             override;
    void visit(MQL::OpGroupBy&)           override;
    void visit(MQL::OpOrderBy&)           override;
    void visit(MQL::OpProjectSimilarity&) override;
    void visit(MQL::OpReturn&)            override;

    /* These are processed in BindingIterVisitor */
    void visit(MQL::OpSet&)      override { throw LogicException("OpSet must be processed outside"); }
    void visit(MQL::OpDescribe&) override { throw LogicException("OpDescribe must be processed outside"); }

    /* These are processed inside OpBasicGraphPattern */
    void visit(MQL::OpEdge&)             override { }
    void visit(MQL::OpDisjointTerm&)     override { }
    void visit(MQL::OpDisjointVar&)      override { }
    void visit(MQL::OpLabel&)            override { }
    void visit(MQL::OpPath&)             override { }
    void visit(MQL::OpProperty&)         override { }

private:
    bool term_exists(ObjectId) const;

    // transform Vars in setted_vars to the assigned ObjectId
    Id replace_setted_var(Id) const;
};
} // namespace MQL
