#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "query/query_context.h"
#include "query/parser/op/mql/ops.h"

class BindingIter;
namespace MQL {

class BindingIterConstructor : public OpVisitor {
public:
    BindingIterConstructor(std::map<VarId, ObjectId>& setted_vars) :
        setted_vars (setted_vars) { }

    std::vector<VarId> projected_vars;

    std::set<VarId> order_saved_vars;


    // Variables that are assigned when evaluating the basic graph pattern, and the optimizer know the value.
    // May come from properties or the SET/WHERE clauses
    std::map<VarId, ObjectId>& setted_vars;

    // Properties from a where clause that are mandatory (so they are pushed into the basic graph pattern)
    std::vector<std::tuple<VarId, ObjectId, ObjectId>> where_property_equalities;

    // properties from RETURN
    // eg: RETURN ?x.name
    std::vector<std::pair<Id, ObjectId>> optional_properties;


    // The subset of possible assigned vars that always have a non-null value while visiting
    // the current operator and should be fixed in its leaf operators such as IndexScans
    std::set<VarId> safe_assigned_vars;

    // After visiting an Op, the result must be written into tmp
    std::unique_ptr<BindingIter> tmp;

    // TODO: implement this
    void visit(MQL::OpGroupBy&)  override { }

    void visit(MQL::OpMatch&)             override;
    void visit(MQL::OpBasicGraphPattern&) override;
    void visit(MQL::OpOptional&)          override;
    void visit(MQL::OpWhere&)             override;
    void visit(MQL::OpOrderBy&)           override;
    void visit(MQL::OpReturn&)            override;

    /* These are processed in BindingIterVisitor */
    void visit(MQL::OpSet&)      override { throw LogicException("OpSet must be processed outside"); }
    void visit(MQL::OpDescribe&) override { throw LogicException("OpSet must be processed outside"); }

    /* These are processed inside OpBasicGraphPattern */
    void visit(MQL::OpEdge&)         override { }
    void visit(MQL::OpDisjointTerm&) override { }
    void visit(MQL::OpDisjointVar&)  override { }
    void visit(MQL::OpLabel&)        override { }
    void visit(MQL::OpPath&)         override { }
    void visit(MQL::OpProperty&)     override { }

private:
    bool term_exists(ObjectId) const;

    // transform Vars in setted_vars to the assigned ObjectId
    Id replace_setted_var(Id) const;
};
} // namespace MQL
